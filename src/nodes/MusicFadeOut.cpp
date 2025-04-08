#include <nodes/MusicFadeOut.hpp>

using namespace geode::prelude;

MusicFadeOut* MusicFadeOut::create(float d) {
    auto ret = new MusicFadeOut();
    if(ret->initWithDuration(d)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void MusicFadeOut::update(float dt) {
    auto target = FMODAudioEngine::get();
    if(!target || target->m_unkBool1a8) {
        target->stopAction(this);
        return;
    }   

    for(auto channelid : FMODAudioEngine::get()->m_musicChannels) {
        auto channel = FMODAudioEngine::get()->getActiveMusicChannel(channelid.first);
        if(!channel) continue;

        if(m_startPitch < 0.f)
            channel->getPitch(&m_startPitch);

        if(dt >= 1.f) {
            channel->setPaused(true);
            channel->setPitch(m_startPitch);
            return;
        }

        // reversed circular ease out
        float ease = 1.f - sqrt(1.f - pow(dt - 1.f, 2.f));
        channel->setPitch(ease * m_startPitch);
    }
}


MusicFadeOut::~MusicFadeOut() {
    auto target = FMODAudioEngine::get();
    if(target && m_startPitch >= 0.f) {
        for(auto channelid : FMODAudioEngine::get()->m_musicChannels) {
            auto channel = FMODAudioEngine::get()->getActiveMusicChannel(channelid.first);
            if(!channel) continue;
            
            channel->setPitch(m_startPitch);
        }
    }
    CCActionInterval::stop();
}