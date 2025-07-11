#pragma once
#include <Geode/cocos/actions/CCActionInterval.h>

enum class FadeMusicDirection {
    None, FadeIn, FadeOut
};

class FadeMusicAction : public cocos2d::CCActionInterval {
    FadeMusicAction();
public:
    static FadeMusicAction* create(float d, FadeMusicDirection dir, bool sfx = true);
    bool init(float d, FadeMusicDirection dir, bool sfx);

    FadeMusicDirection m_dir = FadeMusicDirection::None;
    bool m_sfx = false;

    void update(float time);
};
