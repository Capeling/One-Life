#include <Geode/Geode.hpp>

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