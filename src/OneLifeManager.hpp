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

    int m_collectedStars;
    int m_collectedMoons;
    int m_collectedOrbs;
    int m_collectedDiamonds;
    
    int m_completedDemons;

    RunType m_runType;

    bool m_fromStartedRun;

public:
    bool init();
    void initiateSaveData();
    void loadSaveData();
    void writeSaveData();
    
    void startRun(RunType type);
    void endRun();

    void toggleRun(RunType type);

    void resetStats();
    void resetSave();

    bool getIsRunning() { return m_isRunning; }
    RunType getRunType() { return m_runType; }
    
    int getCollectedStars() { return m_collectedStars; }
    int getCollectedMoons() { return m_collectedMoons; }
    int getCollectedOrbs() { return m_collectedOrbs; }
    int getCollectedDiamonds() { return m_collectedDiamonds; }
    int getCompletedDemons() { return m_completedDemons; }

    bool getFromStartedRun() { return m_fromStartedRun; }
    void setFromStartedRun(bool V) { m_fromStartedRun = V; }

    std::string getStatsString();

    static OneLifeManager* get();

};