#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>

#include "OneLifeManager.hpp"

struct HookGameStatsManager : geode::Modify<HookGameStatsManager, GameStatsManager> {
    void incrementStat(char const* p0, int p1) {
        auto olm = OneLifeManager::get();
        std::string type = p0;
        
        if (type == "13")
            olm->incrementDiamonds(p1);

        if (type == "14")
            olm->incrementOrbs(p1);

        GameStatsManager::incrementStat(p0, p1);
    }
};