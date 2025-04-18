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

#ifdef DEBUG_ENABLED

#include <Geode/modify/LevelInfoLayer.hpp>

struct HookLevelInfoLayer : geode::Modify<HookLevelInfoLayer, LevelInfoLayer> {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge))
            return false;

        if (OneLifeManager::get()->getIsRunning()) {
            auto btn = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName(
                OneLifeConstants::SPRITE_DEBUG, .60f, [this](auto sender) {
                    OneLifeManager::get()->giveStatsFromLevel(this->m_level);
                }
            );
            
            auto menu = this->getChildByID("left-side-menu");
            if (menu) {
                menu->addChild(btn);
                menu->updateLayout();
            } else {
                geode::log::error(OneLifeConstants::ERROR_UNKNOWN_NODE, "left-side-menu");
            }
        }

        return true;
    }
};
#endif

struct HookMenuLayer : geode::Modify<HookMenuLayer, MenuLayer> {
    bool init() {
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
        
        return true;
    }
};