#include <Geode/modify/PlayLayer.hpp>

#include "OneLifeManager.hpp"
#include "OneLifeEffectLayer.hpp"

struct HookPlayLayer : geode::Modify<HookPlayLayer, PlayLayer> {
    virtual void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        PlayLayer::destroyPlayer(p0, p1);

        auto olm = OneLifeManager::get();

        if (!olm->getIsRunning()) return;

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
};