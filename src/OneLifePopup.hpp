#include <Geode/Geode.hpp>

#include "PopupWithCorners.hpp"

class OneLifePopup : public PopupWithCorners<> {
public:
    static OneLifePopup* create();

protected:
    CCMenuItemSpriteExtra* m_startBtn = nullptr;
    
    cocos2d::CCLabelBMFont* m_runStateLabel = nullptr;
    cocos2d::CCLabelBMFont* m_statLabel = nullptr;
    
    bool setup();
    void toggleRunBtn(bool type);

};