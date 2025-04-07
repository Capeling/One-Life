#include "OneLifeEffectLayer.hpp"
#include "MusicFadeOut.hpp"
#include "OneLifeManager.hpp"

OneLifeEffectLayer* OneLifeEffectLayer::create(OneLifeEffectType type) {
    auto ret = new OneLifeEffectLayer();
    if (ret->init(type)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OneLifeEffectLayer::init(OneLifeEffectType type) {
    if (!CCLayerColor::initWithColor({ 0, 0, 0, 150 }))
        return false;
    
    auto director = cocos2d::CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    cocos2d::CCTouchDispatcher::get()->registerForcePrio(this, 2);
    
    this->m_type = type;
    this->setTouchEnabled(true);
    this->setKeyboardEnabled(true);

    auto colorOpacityAction = cocos2d::CCFadeTo::create(0.15f, 150);
    
    this->setOpacity(0);
    this->runAction(colorOpacityAction);
    GameManager::get()->getActionManager()->addAction(MusicFadeOut::create(3.f), FMODAudioEngine::get(), false);
    
    auto endAction = cocos2d::CCCallFunc::create(this, callfunc_selector(OneLifeEffectLayer::endEffect));

    if (type == OneLifeEffectType::Start) {
        m_counterLabel = cocos2d::CCLabelBMFont::create(fmt::format("{}", m_counterStage).c_str(), "bigFont.fnt");
        m_counterLabel->setScale(0.f);
        addChildAtPosition(m_counterLabel, geode::Anchor::Center, ccp(0, 0), false);
        
        auto scaleAction = cocos2d::CCScaleTo::create(1.f, 2);
        auto scaleEase = cocos2d::CCEaseInOut::create(scaleAction, 2.f);
        
        auto opacityAction = cocos2d::CCFadeTo::create(1.f, 0);
        auto opacityEase = cocos2d::CCEaseInOut::create(opacityAction, 2.f);
        
        auto tintAction = cocos2d::CCTintTo::create(1.f, 255, 0, 0);
        auto tintEase = cocos2d::CCEaseInOut::create(tintAction, 2.f);
    
        auto combinedAction = cocos2d::CCSpawn::create(
            scaleEase,
            opacityEase,
            tintEase,
            0
        );
    
        auto decreaseAction = cocos2d::CCCallFunc::create(this, callfunc_selector(OneLifeEffectLayer::countDownDecrease));
    
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
    } else if (type == OneLifeEffectType::End) {
        endEffect();
    } else {
        FLAlertLayer::create(
            "ERROR!",
            "Invalid effect, something has gone horribly wrong!\nContact Capeling on discord for help.",
            "OK"
        )->show();
    }
    
    return true;
}

void OneLifeEffectLayer::addToScene() {
    cocos2d::CCScene::get()->addChild(this, 1000);
}

void OneLifeEffectLayer::countDownDecrease() {
    m_counterStage--;
    m_counterLabel->setString(fmt::format("{}", m_counterStage).c_str());

    m_counterLabel->setScale(0.f);
    m_counterLabel->setOpacity(255);
    m_counterLabel->setColor({ 255, 255, 255 });

    auto path = MusicDownloadManager::sharedState()->pathForSFX(4292);
    FMODAudioEngine::sharedEngine()->playEffect(path);
}

void OneLifeEffectLayer::endEffect() {
    toggleRunState();

    int sfx = 566;

    if (m_type == OneLifeEffectType::End)
        sfx = 4290;

    auto path = MusicDownloadManager::sharedState()->pathForSFX(sfx);
    FMODAudioEngine::sharedEngine()->playEffect(path);

    FMODAudioEngine::get()->stopAllMusic(true);
    FMODAudioEngine::get()->stopAllActions();

    GameManager::get()->getActionManager()->addAction(cocos2d::CCSequence::create(
        cocos2d::CCDelayTime::create(0.5f),
        cocos2d::CCCallFunc::create(GameManager::get(), callfunc_selector(GameManager::fadeInMenuMusic)),
        0
    ), GameManager::get(), false);
}

void OneLifeEffectLayer::toggleRunState() {
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::get();
    
    olm->setFromStartedRun(true);
    olm->toggleRun(RunType::Mixed);
    director->replaceScene(cocos2d::CCTransitionFade::create(0.5f, MenuLayer::scene(false)));

    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);

    cocos2d::CCTouchDispatcher::get()->unregisterForcePrio(this);
}