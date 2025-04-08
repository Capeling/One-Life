#pragma once
#include <Geode/Geode.hpp>

enum class OneLifeEffectType {
    None = 0,
    Start = 1,
    End = 2
};

class OneLifeEffectLayer : public cocos2d::CCLayerColor {
public:
    static OneLifeEffectLayer* create(OneLifeEffectType type);
    
    void addToScene();
protected:
    OneLifeEffectType m_type = OneLifeEffectType::None;
    int m_counterStage = 3;
    
    cocos2d::CCLabelBMFont* m_counterLabel = nullptr;
    cocos2d::CCSprite* m_endSprite = nullptr;

    bool init(OneLifeEffectType type);
    
    void registerWithTouchDispatcher() override {
        cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
    }
    
    ~OneLifeEffectLayer() override {
        cocos2d::CCTouchDispatcher::get()->unregisterForcePrio(this);
    }
    
    void endEffect();
    void playEndSFX();
    void toggleRunState();
    void countDownDecrease();
};