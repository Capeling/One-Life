#include "OneLifeManager.hpp"

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
    
    resetStats();
}

void OneLifeManager::resetStats() {
    m_collectedStars = 0;
    m_collectedMoons = 0;
    m_collectedOrbs = 0;
    m_collectedDiamonds = 0;
    
    m_completedDemons = 0;

    m_hasDied = false;
    m_runType = RunType::None;
}

void OneLifeManager::resetSave() {
    auto mod = geode::Mod::get();

    mod->setSavedValue<bool>("has_booted", false);
    mod->setSavedValue<bool>("is_running", false);
    
    mod->setSavedValue<int>("collected_stars", 0);
    mod->setSavedValue<int>("collected_moons", 0);
    mod->setSavedValue<int>("collected_orbs", 0);
    mod->setSavedValue<int>("collected_diamonds", 0);
    mod->setSavedValue<int>("completed_demons", 0);

    mod->setSavedValue<int>("run_type", -1);
}

void OneLifeManager::initiateSaveData() {
    auto mod = geode::Mod::get();

    geode::log::info("First bootup, initializing save data");

    resetSave();
    mod->setSavedValue<bool>("has_booted", true);

    loadSaveData();
    
}

void OneLifeManager::loadSaveData() {
    auto mod = geode::Mod::get();

    geode::log::info("Loading save data");
    resetStats();

    m_isRunning = mod->getSavedValue<bool>("is_running");

    if (m_isRunning) {
        m_collectedStars = mod->getSavedValue<int>("collected_stars");
        m_collectedMoons = mod->getSavedValue<int>("collected_moons");
        m_collectedOrbs = mod->getSavedValue<int>("collected_orbs");
        m_collectedDiamonds = mod->getSavedValue<int>("collected_diamonds");
        m_completedDemons = mod->getSavedValue<int>("completed_demons");

        m_hasDied = mod->getSavedValue<bool>("has_died");

        m_runType = static_cast<RunType>(mod->getSavedValue<int>("run_type"));
    } else {
        resetStats();
    }

    if (m_hasDied) endRun();
}

void OneLifeManager::playerDied() {
    m_hasDied = true;
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

        mod->setSavedValue<bool>("has_died", m_isRunning);

        mod->setSavedValue<int>("run_type", static_cast<int>(m_runType));
    } else {
        resetSave();
    }
}

OneLifeManager* OneLifeManager::get() {
    static OneLifeManager instance;
    return &instance;
}

std::string OneLifeManager::getStatsString() {
    return fmt::format("Stars: {}\nMoons: {}\nOrbs: {}\nDiamonds: {}\nDemons: {}",
        m_collectedStars,
        m_collectedMoons,
        m_collectedOrbs,
        m_collectedDiamonds,
        m_completedDemons
    );
}