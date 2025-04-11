#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include <ui/OneLifePopup.hpp>
#include <OneLifeManager.hpp>
#include <OneLifeConstants.hpp>

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
    
    // auto spr = cocos2d::CCSprite::createWithSpriteFrameName(
    //     olm->getIsRunning() ? OneLifeConstants::SPRITE_END : OneLifeConstants::SPRITE_START
    // );

    auto heart = cocos2d::CCSprite::createWithSpriteFrameName(OneLifeConstants::SPRITE_HEART);
    auto overlay = cocos2d::CCSprite::createWithSpriteFrameName(OneLifeConstants::SPRITE_HEART_OVERLAY);
    heart->addChildAtPosition(overlay, geode::Anchor::Center);

    heart->setColor({ 255, 0, 0 });

    auto spr = geode::CircleButtonSprite::create(
        heart,
        geode::CircleBaseColor::Blue,
        geode::CircleBaseSize::Small
    );
    spr->setTopRelativeScale(0.7);

    auto btn = geode::cocos::CCMenuItemExt::createSpriteExtra(spr, [](auto sender) {
        OneLifePopup::create()->show();
    });
    
    auto menu = this->getChildByID("profile-menu");
    if (menu) {
        menu->addChild(btn);
        menu->updateLayout();
    } else {
        geode::log::error(OneLifeConstants::ERROR_UNKNOWN_NODE, "profile-menu");
    }

    if (olm->getFromStartedRun()) {
        olm->setFromStartedRun(false);
        setKeyboardEnabled(true);

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