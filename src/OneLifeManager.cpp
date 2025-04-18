#include <OneLifeManager.hpp>

bool OneLifeManager::init() {
    auto mod = geode::Mod::get();
    m_firstBootup = !mod->getSavedValue<bool>("has_booted");
    
    if (m_firstBootup)
        initiateSaveData();
    else
        loadSaveData();

    return true;
}

void OneLifeManager::toggleRun(RunType type) {
    if (m_isRunning)
        endRun();
    else
        startRun(type);
}

void OneLifeManager::startRun(RunType type) {

    int intType = static_cast<int>(type);

    if (intType > 2 || intType < 0) {
        geode::log::error("Invalid RunType: {}", intType);

        return;
    }

    m_isRunning = true;
    
    resetStats();
}

void OneLifeManager::endRun() {
    m_isRunning = false;

    saveBestRun();
    
    resetStats();
}

void OneLifeManager::resetStats() {
    m_currentRun.clear();
    m_currentRun = { 0, 0, 0, 0, 0};
    m_completedIDS.clear();

    m_hasDied = false;
    m_runType = RunType::None;

    m_currentExp = 0.f;
}

void OneLifeManager::resetSave() {
    auto mod = geode::Mod::get();

    mod->setSavedValue<bool>("has_died", false);
    mod->setSavedValue<bool>("is_running", false);

    mod->setSavedValue<std::vector<int>>("current_run", {});
    
    mod->setSavedValue<int>("run_type", -1);
}

void OneLifeManager::initiateSaveData() {
    auto mod = geode::Mod::get();
    
    geode::log::info("First bootup, initializing save data");
    mod->setSavedValue<std::vector<int>>("best_run", {});
    
    resetSave();
    
    loadSaveData();
    
}

void OneLifeManager::giveStatsFromLevel(GJGameLevel* level) {
    auto gsm = GameStatsManager::get();

    if (std::find(m_completedIDS.begin(), m_completedIDS.end(), level->m_levelID.value()) != m_completedIDS.end()) {
        geode::log::debug("Level {} already completed", level->m_levelID.value());
        return;
    }

    m_completedIDS.push_back(level->m_levelID.value());

    if (level->m_stars.value() > 0) {

        if (level->m_levelLength != 5)
            incrementStars(level->m_stars.value());
        else
            incrementMoons(level->m_stars.value());

        incrementDiamonds(gsm->getAwardedDiamondsForLevel(level));
        incrementOrbs(gsm->getBaseCurrencyForLevel(level));

        if (level->m_demon.value() > 0)
            incrementDemons(level->getAverageDifficulty() * level->m_demon.value());
    }

    m_currentExp = calculateExp(m_currentRun[0], m_currentRun[1], m_currentRun[2], m_currentRun[3], m_currentRun[4]);
} 

void OneLifeManager::loadSaveData() {
    auto mod = geode::Mod::get();
    
    #ifdef DEBUG_ENABLED
    geode::log::info("DEBUG One Life build running! This may cause unintended behavior.");
    #else
    geode::log::info("RELEASE One Life build running! Loading save data...");  
    #endif
    
    resetStats();
    
    m_isRunning = mod->getSavedValue<bool>("is_running");
    mod->setSavedValue<bool>("has_booted", true);

    if (m_isRunning) {
        m_currentRun = mod->getSavedValue<std::vector<int>>("current_run");
        
        m_hasDied = mod->getSavedValue<bool>("has_died");
        
        m_runType = static_cast<RunType>(mod->getSavedValue<int>("run_type"));
        
        m_currentExp = 0.f;
    } else {
        resetStats();
    }
    
    m_bestRun = mod->getSavedValue<std::vector<int>>("best_run");

    if (m_hasDied) endRun();
}

void OneLifeManager::playerDied() {
    m_hasDied = true;
}

float OneLifeManager::calculateExp(int stars, int moons, int orbs, int diamonds, int demons) {
    auto mod = geode::Mod::get();
    
    float exp = 0.f;

    exp += stars * 0.1f;
    exp += moons * 0.1f;
    exp += diamonds * 0.05f;
    exp += orbs * 0.001f;
    exp += demons;

    return exp;
}

void OneLifeManager::saveBestRun() {
    if (m_bestRun.size() == 0) {
        m_bestRun = m_currentRun;
    } else {
        int i = 0;
        for (int val : m_bestRun) {
            if (val < m_currentRun[i])
                m_bestRun[i] = m_currentRun[i];
            i++;
        }
    }
}

void OneLifeManager::writeSaveData() {
    auto mod = geode::Mod::get();

    geode::log::info("Writing save data");
    
    mod->setSavedValue<bool>("is_running", m_isRunning);
    
    if (m_isRunning) {
        mod->setSavedValue<std::vector<int>>("current_run", m_currentRun);
        mod->setSavedValue<std::vector<int>>("completed_ids", m_completedIDS);

        mod->setSavedValue<bool>("has_died", m_hasDied);

        mod->setSavedValue<int>("run_type", static_cast<int>(m_runType));
    } else {
        resetSave();
    }

    mod->setSavedValue<std::vector<int>>("best_run", m_bestRun);
}

OneLifeManager* OneLifeManager::get() {
    static OneLifeManager instance;
    return &instance;
}

std::string OneLifeManager::getStatString(float exp, int stars, int moons, int orbs, int diamonds, int demons) {
    return fmt::format("Exp: {}\nStars: {}\nMoons: {}\nOrbs: {}\nDiamonds: {}\nDemons: {}",
        exp,
        stars,
        moons,
        orbs,
        diamonds,
        demons
    );
}

std::string OneLifeManager::getCurrentStatString() {
    return getStatString(
        calculateExp(m_currentRun[0], m_currentRun[1], m_currentRun[2], m_currentRun[3], m_currentRun[4]),
        m_currentRun[0], m_currentRun[1], m_currentRun[2], m_currentRun[3], m_currentRun[4]
            );
}
std::string OneLifeManager::getBestStatString() {
    if (m_bestRun.size() == 0) {
        return "";
    }
    return getStatString(
        calculateExp(m_bestRun[0], m_bestRun[1], m_bestRun[2], m_bestRun[3], m_bestRun[4]),
                m_bestRun[0], m_bestRun[1], m_bestRun[2], m_bestRun[3], m_bestRun[4]
            );
}