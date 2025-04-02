#include "OneLifePopup.hpp"
#include "OneLifeManager.hpp"
#include "MusicFadeOut.hpp"

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

    if (m_isCounting) return;

    std::string title = "Start Run";
    std::string desc = "Are you sure you want to <cl>start a run?</c>";

    if (!type) {
        title = "End Run";
        desc = "Are you sure you want to <cr>end the run?</c>";
    }

    geode::createQuickPopup(title.c_str(), desc, "Cancel", "Yes", [this, olm, director, type](auto, bool btn2) {
        if (btn2) {
            if (type) {
                countDown();
            } else {
                toggleRunState();

                auto path = MusicDownloadManager::sharedState()->pathForSFX(4290);
                FMODAudioEngine::sharedEngine()->playEffect(path);
            }
            
        }
    });
}

void OneLifePopup::countDown() {
    m_colorLayer = cocos2d::CCLayerColor::create();
    m_colorLayer->initWithColor({ 0, 0, 0, 145 });
    m_colorLayer->setOpacity(0);
    
    addChildAtPosition(m_colorLayer, geode::Anchor::BottomLeft, ccp(0, 0), false);
    
    m_counterLabel = cocos2d::CCLabelBMFont::create(fmt::format("{}", m_counterStage).c_str(), "bigFont.fnt");
    m_counterLabel->setScale(0.f);
    addChildAtPosition(m_counterLabel, geode::Anchor::Center, ccp(0, 0), false);
    
    m_startBtn->setEnabled(false);
    m_closeBtn->setEnabled(false);

    m_isCounting = true;
    
    auto scaleAction = cocos2d::CCScaleTo::create(1.f, 2);
    auto scaleEase = cocos2d::CCEaseInOut::create(scaleAction, 2.f);
    
    auto opacityAction = cocos2d::CCFadeTo::create(1.f, 0);
    auto opacityEase = cocos2d::CCEaseInOut::create(opacityAction, 2.f);
    
    auto tintAction = cocos2d::CCTintTo::create(1.f, 255, 0, 0);
    auto tintEase = cocos2d::CCEaseInOut::create(tintAction, 2.f);
    
    auto colorOpacityAction = cocos2d::CCFadeTo::create(0.15f, 125);

    m_colorLayer->runAction(colorOpacityAction);
    GameManager::get()->getActionManager()->addAction(MusicFadeOut::create(8.f), FMODAudioEngine::get(), false);

    auto combinedAction = cocos2d::CCSpawn::create(
        scaleEase,
        opacityEase,
        tintEase,
        0
    );

    auto decreaseAction = cocos2d::CCCallFunc::create(this, callfunc_selector(OneLifePopup::countDownDecrease));
    auto endAction = cocos2d::CCCallFunc::create(this, callfunc_selector(OneLifePopup::endCountDown));

    auto sequenceAction = cocos2d::CCSequence::create(
        combinedAction,
        decreaseAction,

        combinedAction,
        decreaseAction,

        combinedAction,

        endAction,
        0
    );

    auto path = MusicDownloadManager::sharedState()->pathForSFX(4292);
    FMODAudioEngine::sharedEngine()->playEffect(path);

    m_counterLabel->runAction(sequenceAction);
}

void OneLifePopup::onClose(cocos2d::CCObject* sender) {
    if (!m_isCounting) {
        if (MenuLayer::get())
            MenuLayer::get()->setKeyboardEnabled(true);
        Popup::onClose(sender);
    }
}

void OneLifePopup::countDownDecrease() {
    m_counterStage--;
    m_counterLabel->setString(fmt::format("{}", m_counterStage).c_str());

    m_counterLabel->setScale(0.f);
    m_counterLabel->setOpacity(255);
    m_counterLabel->setColor({ 255, 255, 255 });

    auto path = MusicDownloadManager::sharedState()->pathForSFX(4292);
    FMODAudioEngine::sharedEngine()->playEffect(path);
}

void OneLifePopup::endCountDown() {
    toggleRunState();

    auto path = MusicDownloadManager::sharedState()->pathForSFX(566);
    FMODAudioEngine::sharedEngine()->playEffect(path);

    FMODAudioEngine::get()->stopAllMusic(true);
    FMODAudioEngine::get()->stopAllActions();

    GameManager::get()->getActionManager()->addAction(cocos2d::CCSequence::create(
        cocos2d::CCDelayTime::create(0.5f),
        cocos2d::CCCallFunc::create(GameManager::get(), callfunc_selector(GameManager::fadeInMenuMusic)),
        0
    ), GameManager::get(), false);
}

void OneLifePopup::toggleRunState() {
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::get();

    olm->setFromStartedRun(true);
    olm->toggleRun(RunType::Mixed);
    director->pushScene(cocos2d::CCTransitionFade::create(0.5f, MenuLayer::scene(false)));
}