#include <OneLifeManager.hpp>
#include <OneLifeConstants.hpp>

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
    m_collectedStars = 0;
    m_collectedMoons = 0;
    m_collectedOrbs = 0;
    m_collectedDiamonds = 0;
    
    m_completedDemons = 0;

    m_completedIDS.clear();

    m_hasDied = false;
    m_runType = RunType::None;

    m_currentExp = 0.f;
}

void OneLifeManager::resetSave() {
    auto mod = geode::Mod::get();

    mod->setSavedValue<bool>("has_died", false);
    mod->setSavedValue<bool>("is_running", false);
    
    mod->setSavedValue<int>("collected_stars", 0);
    mod->setSavedValue<int>("collected_moons", 0);
    mod->setSavedValue<int>("collected_orbs", 0);
    mod->setSavedValue<int>("collected_diamonds", 0);
    mod->setSavedValue<int>("completed_demons", 0);
    mod->setSavedValue<std::vector<int>>("completed_ids", {});

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
            incrementDemons(level->m_demon.value());
    }

    m_currentExp = calculateExp(m_collectedStars, m_collectedMoons, m_collectedOrbs, m_collectedDiamonds, m_completedDemons);
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
        m_collectedStars = mod->getSavedValue<int>("collected_stars");
        m_collectedMoons = mod->getSavedValue<int>("collected_moons");
        m_collectedOrbs = mod->getSavedValue<int>("collected_orbs");
        m_collectedDiamonds = mod->getSavedValue<int>("collected_diamonds");
        m_completedDemons = mod->getSavedValue<int>("completed_demons");
        m_completedIDS = mod->getSavedValue<std::vector<int>>("completed_ids");
        
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

    exp += stars * 0.5f;
    exp += moons * 0.5f;
    exp += diamonds * 0.05f;
    exp += orbs * 0.001f;
    exp += demons;

    return exp;
}

void OneLifeManager::saveBestRun() {
    if (m_bestRun.size() == 0) {
        m_bestRun.push_back(m_collectedStars);
        m_bestRun.push_back(m_collectedMoons);
        m_bestRun.push_back(m_collectedOrbs);
        m_bestRun.push_back(m_collectedDiamonds);
        m_bestRun.push_back(m_completedDemons);
    } else {
        if (m_collectedStars > m_bestRun[0])
            m_bestRun[0] = m_collectedStars;
        if (m_collectedMoons > m_bestRun[1])
            m_bestRun[1] = m_collectedMoons;
        if (m_collectedOrbs > m_bestRun[2])
            m_bestRun[2] = m_collectedOrbs;
        if (m_collectedDiamonds > m_bestRun[3])
            m_bestRun[3] = m_collectedDiamonds;
        if (m_completedDemons > m_bestRun[4])
            m_bestRun[4] = m_completedDemons;
    }
}

void OneLifeManager::writeSaveData() {
    auto mod = geode::Mod::get();

    geode::log::info("Writing save data");
    
    mod->setSavedValue<bool>("is_running", m_isRunning);
    
    if (m_isRunning) {
        mod->setSavedValue<int>("collected_stars", m_collectedStars);
        mod->setSavedValue<int>("collected_moons", m_collectedMoons);
        mod->setSavedValue<int>("collected_orbs", m_collectedOrbs);
        mod->setSavedValue<int>("collected_diamonds", m_collectedDiamonds);
        mod->setSavedValue<int>("completed_demons", m_completedDemons);

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
        calculateExp(m_collectedStars, m_collectedMoons, m_collectedOrbs, m_collectedDiamonds, m_completedDemons),
        m_collectedStars, m_collectedMoons, m_collectedOrbs, m_collectedDiamonds, m_completedDemons
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