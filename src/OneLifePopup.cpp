#include "OneLifePopup.hpp"
#include "OneLifeManager.hpp"
#include "OneLifeEffectLayer.hpp"

OneLifePopup* OneLifePopup::create() {
    auto ret = new OneLifePopup();
    if (ret->initAnchored(315, 250, "geode.loader/GE_square03.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OneLifePopup::setup() {
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::get();
    
    setTitle("One Life", "bigFont.fnt");
    addCorners(Corner::Blue, Corner::Blue);

    if (!olm->getIsRunning()) {
        m_startBtn = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName("GJ_starBtnMod_001.png", 1.f, [this](auto sender){
            toggleRunBtn(true);
        });

        m_runStateLabel = cocos2d::CCLabelBMFont::create("not in a run", "goldFont.fnt");
    } else {
        m_runStateLabel = cocos2d::CCLabelBMFont::create("in a run", "goldFont.fnt");
        
        m_statLabel = cocos2d::CCLabelBMFont::create(olm->getStatsString().c_str(), "bigFont.fnt");
        m_statLabel->setAlignment(cocos2d::kCCTextAlignmentCenter);
        m_statLabel->setScale(0.6f);
        m_mainLayer->addChildAtPosition(m_statLabel, geode::Anchor::Center);
        
        m_startBtn = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName("GJ_starBtn2_001.png", 1.f, [this](auto sender){
            toggleRunBtn(false);
        });
        
    }
    
    m_buttonMenu->addChildAtPosition(m_startBtn, geode::Anchor::Bottom, ccp(0, 40));
    m_runStateLabel->setScale(0.6f);
    m_mainLayer->addChildAtPosition(m_runStateLabel, geode::Anchor::Top, ccp(0, -50));

    return true;
}

void OneLifePopup::toggleRunBtn(bool type) {
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::get();

    std::string title = "Start Run";
    std::string desc = "Are you sure you want to <cl>start a run?</c>";

    if (!type) {
        title = "End Run";
        desc = "Are you sure you want to <cr>end the run?</c>";
    }

    geode::createQuickPopup(title.c_str(), desc, "Cancel", "Yes", [this, olm, director, type](auto, bool btn2) {
        if (btn2) {
            auto effectType = OneLifeEffectType::Start;
            if (!type)
                effectType = OneLifeEffectType::End;

            auto olel = OneLifeEffectLayer::create(effectType);
            olel->addToScene();

        }
    });
}

