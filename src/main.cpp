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
    
    auto text = cocos2d::CCLabelBMFont::create(fmt::format("One Life: {}", olm->getIsRunning()).c_str(), "bigFont.fnt");
    auto menu = cocos2d::CCMenu::create();
    
    text->setScale(0.6f);
    
    auto textBtn = geode::cocos::CCMenuItemExt::createSpriteExtra(text, [](auto sender) {
        OneLifePopup::create()->show();
    });
    
    textBtn->setPosition(ccp( 0, 148 ));
    menu->setZOrder(5);
    menu->addChild(textBtn);
    this->addChild(menu);
    
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