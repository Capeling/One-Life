#include "MenuLayer.hpp"

#include "ui/OneLifePopup.hpp"
#include "OneLifeConstants.hpp"
#include "OneLifeManager.hpp"

bool HookMenuLayer::init() {
    if (!MenuLayer::init())
        return false;

    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto heart = cocos2d::CCSprite::createWithSpriteFrameName(OneLifeConstants::SPRITE_HEART);
    auto overlay = cocos2d::CCSprite::createWithSpriteFrameName(OneLifeConstants::SPRITE_HEART_OVERLAY);
    heart->addChildAtPosition(overlay, geode::Anchor::Center);

    if (olm->getIsRunning())
        heart->setColor(OneLifeConstants::COLOR_HEART_ACTIVE);
    else
        heart->setColor(OneLifeConstants::COLOR_HEART_INACTIVE);

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
    }

    m_fields->m_heart = heart;
    
    return true;
}

void HookMenuLayer::updateOneLifeBtn() {
    if (OneLifeManager::get()->getIsRunning())
        m_fields->m_heart->setColor(OneLifeConstants::COLOR_HEART_ACTIVE);
    else
        m_fields->m_heart->setColor(OneLifeConstants::COLOR_HEART_INACTIVE);
}