#include <ui/OneLifePopup.hpp>
#include <ui/OneLifeEffectLayer.hpp>
#include <OneLifeManager.hpp>
#include <OneLifeConstants.hpp>

OneLifePopup* OneLifePopup::create() {
    auto ret = new OneLifePopup();
    if (ret->initAnchored(320, 265, "geode.loader/GE_square03.png")) {
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

    bool running = olm->getIsRunning();

    if (!running) {
        
        m_statLabel = cocos2d::CCLabelBMFont::create(olm->getBestStatString().c_str(), "bigFont.fnt");
        
        m_runStateLabel = cocos2d::CCLabelBMFont::create(OneLifeConstants::POPUP_NOT_IN_RUN, "goldFont.fnt");
    } else {
        m_runStateLabel = cocos2d::CCLabelBMFont::create(OneLifeConstants::POPUP_IN_RUN, "goldFont.fnt");
        
        m_statLabel = cocos2d::CCLabelBMFont::create(olm->getCurrentStatString().c_str(), "bigFont.fnt"); 
    }
    
    m_statLabel->setAlignment(cocos2d::kCCTextAlignmentCenter);
    m_statLabel->setScale(0.6f);
    m_mainLayer->addChildAtPosition(m_statLabel, geode::Anchor::Center);

    m_startBtn = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName(
        running ? OneLifeConstants::SPRITE_END : OneLifeConstants::SPRITE_START,
        1.f, 
        [this, running](auto sender){
            toggleRunBtn(!running);
        }
    );
    
    m_buttonMenu->addChildAtPosition(m_startBtn, geode::Anchor::Bottom, ccp(0, 40));
    m_runStateLabel->setScale(0.6f);
    m_mainLayer->addChildAtPosition(m_runStateLabel, geode::Anchor::Top, ccp(0, -50));
    
    return true;
}

void OneLifePopup::toggleRunBtn(bool type) {
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::get();

    auto running = olm->getIsRunning();

    geode::createQuickPopup(
        running ? OneLifeConstants::POPUP_END_RUN : OneLifeConstants::POPUP_START_RUN,
        running ? OneLifeConstants::POPUP_END_RUN_DESC : OneLifeConstants::POPUP_START_RUN_DESC,
        OneLifeConstants::GENERIC_CANCEL, OneLifeConstants::GENERIC_CONFIRM,
        [this, olm, director, type](auto, bool btn2) {
            if (btn2) {
                auto effectType = OneLifeEffectType::Start;
                if (!type)
                    effectType = OneLifeEffectType::End;

                auto olel = OneLifeEffectLayer::create(effectType);
                olel->addToScene();

            }
        }
    );
}

