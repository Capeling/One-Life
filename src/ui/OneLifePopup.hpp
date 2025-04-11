#pragma once
#include <Geode/Geode.hpp>

#include <ui/PopupWithCorners.hpp>

class OneLifePopup : public PopupWithCorners<> {
public:
    static OneLifePopup* create();

protected:
    CCMenuItemSpriteExtra* m_startBtn = nullptr;
    CCMenuItemSpriteExtra* m_infoBtn = nullptr;
    
    cocos2d::CCLabelBMFont* m_runStateLabel = nullptr;
    cocos2d::CCLabelBMFont* m_statLabel = nullptr;
    // TextArea* m_statArea = nullptr;
    cocos2d::CCLabelBMFont* m_versionLabel = nullptr;

    bool setup();
    void toggleRunBtn(bool type);

};