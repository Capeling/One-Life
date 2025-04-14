#pragma once
#include <Geode/cocos/actions/CCActionInterval.h>

/*
Taken from https://github.com/undefined06855/Rewind with permission from undefined
Thank you goat
*/

enum class FadeMusicDirection {
    FadeIn, FadeOut
};

class FadeMusicAction : public cocos2d::CCActionInterval {
    FadeMusicAction();
public:
    static FadeMusicAction* create(float d, FadeMusicDirection dir);
    static FadeMusicAction* create(float d, FadeMusicDirection dir, const std::unordered_map<int, float>& targetPitches);
    bool init(float d, FadeMusicDirection dir, const std::unordered_map<int, float>& targetPitches);

    FadeMusicDirection m_dir;
    std::unordered_map<int, float> m_initialOrTargetPitches;

    void update(float time);
};
