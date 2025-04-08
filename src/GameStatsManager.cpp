#include <Geode/Geode.hpp>
#include <Geode/modify/GameStatsManager.hpp>

#include "OneLifeManager.hpp"

struct HookGameStatsManager : geode::Modify<HookGameStatsManager, GameStatsManager> {
};