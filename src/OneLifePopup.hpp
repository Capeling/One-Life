#include <Geode/Geode.hpp>
#include "PopupWithCorners.hpp"

class OneLifePopup : public PopupWithCorners<> {
public:
    static OneLifePopup* create();

    bool setup();
    void toggleRunBtn(bool type);

    void countDown();
    void countDownDecrease();
    void endCountDown();

    void toggleRunState();

    void onClose(cocos2d::CCObject* sender);
protected:
    CCMenuItemSpriteExtra* m_startBtn = nullptr;

    cocos2d::CCLabelBMFont* m_runStateLabel = nullptr;
    cocos2d::CCLabelBMFont* m_statLabel = nullptr;
    
    cocos2d::CCLabelBMFont* m_counterLabel = nullptr;
    cocos2d::CCLayerColor* m_colorLayer = nullptr;
    int m_counterStage = 3;
    bool m_isCounting = false;
};