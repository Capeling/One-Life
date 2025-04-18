#pragma once
#include <Geode/Geode.hpp>

enum class RunType {
    None = -1,
    Mixed = 0,
    Classic = 1,
    Platformer = 2
};

class OneLifeManager {
protected:
    bool m_isRunning;
    bool m_firstBootup;
    bool m_hasDied;

    // int m_collectedStars;
    // int m_collectedMoons;
    // int m_collectedOrbs;
    // int m_collectedDiamonds;
    // int m_completedDemons;

    std::vector<int> m_completedIDS = {};

    std::vector<int> m_currentRun = {};
    std::vector<int> m_bestRun = {};

    float m_currentExp;
    

    RunType m_runType;

    bool m_fromStartedRun;

public:
    bool init();
    void initiateSaveData();
    void loadSaveData();
    void writeSaveData();
    
    void startRun(RunType type);
    void endRun();

    void playerDied();

    void toggleRun(RunType type);

    void resetStats();
    void resetSave();

    void saveBestRun();

    bool getIsRunning() { return m_isRunning; }
    bool getHasDied() { return m_hasDied; }
    RunType getRunType() { return m_runType; }
    
    int getCollectedStars() { return m_currentRun[0]; }
    int getCollectedMoons() { return m_currentRun[1]; }
    int getCollectedOrbs() { return m_currentRun[2]; }
    int getCollectedDiamonds() { return m_currentRun[3]; }
    int getCompletedDemons() { return m_currentRun[4]; }

    void incrementStars(int by) { m_currentRun[0] += by; };
    void incrementMoons(int by) { m_currentRun[1] += by; };
    void incrementOrbs(int by) { m_currentRun[2] += by; };
    void incrementDiamonds(int by) { m_currentRun[3] += by; };
    void incrementDemons(int by) { m_currentRun[4] += by; };

    void giveStatsFromLevel(GJGameLevel* level);

    float calculateExp(int stars, int moons, int orbs, int diamonds, int demons);

    bool getFromStartedRun() { return m_fromStartedRun; }
    void setFromStartedRun(bool V) { m_fromStartedRun = V; }

    std::string getStatString(float exp, int stars, int moons, int orbs, int diamonds, int demons);

    std::string getCurrentStatString();
    std::string getBestStatString();

    static OneLifeManager* get();

};