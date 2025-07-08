#ifndef __MENU_LAYER_HPP__
#define __MENU_LAYER_HPP__

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

struct HookMenuLayer : geode::Modify<HookMenuLayer, MenuLayer> {
    struct Fields {
        cocos2d::CCSprite *m_heart = nullptr;
    };
    
    bool init();
    void updateOneLifeBtn();
};

#endif /* __MENU_LAYER_HPP__ */