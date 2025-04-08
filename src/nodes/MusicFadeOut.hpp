#pragma once

#include <Geode/Geode.hpp>

class MusicFadeOut : public cocos2d::CCActionInterval {
private:
    float m_startPitch = -1.f;
public:
    static MusicFadeOut* create(float d);

    virtual void update(float dt);
    ~MusicFadeOut();
};