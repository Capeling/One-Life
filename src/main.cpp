#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "OneLifePopup.hpp"
#include "OneLifeManager.hpp"

$on_mod(Loaded) {
    auto olm = OneLifeManager::get();
    olm->init();
}

$on_mod(DataSaved) {
    auto olm = OneLifeManager::get();
    olm->writeSaveData();
}

struct HookMenuLayer : geode::Modify<HookMenuLayer, MenuLayer> {
    bool init() {
        if (!MenuLayer::init())
        return false;
    
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    
    auto spr = cocos2d::CCSprite::createWithSpriteFrameName(olm->getIsRunning() ? "GJ_starBtn2_001.png" : "GJ_starBtnMod_001.png");

    auto btn = geode::cocos::CCMenuItemExt::createSpriteExtra(spr, [](auto sender) {
        OneLifePopup::create()->show();
    });

    auto menu = this->getChildByID("right-side-menu");
    if (menu) {
        menu->addChild(btn);
        menu->updateLayout();
    } else {
        geode::log::error("Failed to get \'right side menu\', One Life will not load its button.");
    }
    
    if (olm->getFromStartedRun()) {
        olm->setFromStartedRun(false);

        if (!olm->getIsRunning()) {
            auto popup = OneLifePopup::create();
            setKeyboardEnabled(false);
            popup->m_scene = this;
            popup->m_noElasticity = true;
            popup->show();
        }
    }
    
    return true;
}
};