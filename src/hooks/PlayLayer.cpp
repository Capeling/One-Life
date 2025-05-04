#include <Geode/modify/PlayLayer.hpp>

#include <OneLifeManager.hpp>
#include <ui/OneLifeEffectLayer.hpp>

struct HookPlayLayer : geode::Modify<HookPlayLayer, PlayLayer> {
    
    void pauseGame(bool p0) {
        auto olm = OneLifeManager::get();

        if (olm->getIsRunning() && olm->getHasDied()) {
            return;
        }

        PlayLayer::pauseGame(p0);
    }

    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);

        auto olm = OneLifeManager::get();

        if (!olm->getIsRunning() || m_level->m_levelType == GJLevelType::Editor) return;

        if (p1 != this->m_anticheatSpike && !olm->getHasDied()) {
            olm->playerDied();
            
            OneLifeEffectLayer::create(OneLifeEffectType::End)->addToScene();
        }
    }

    void resetLevel() {
        auto olm = OneLifeManager::get();

        if (olm->getHasDied()) {
            return;
        }

        PlayLayer::resetLevel();
    }

    void levelComplete() {
        auto olm = OneLifeManager::get();
        auto director = cocos2d::CCDirector::sharedDirector();

        if (olm->getIsRunning()) {
            olm->giveStatsFromLevel(this->m_level);
        }

        PlayLayer::levelComplete();
    }
};