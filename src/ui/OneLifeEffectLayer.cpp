#include <ui/OneLifeEffectLayer.hpp>
#include <nodes/MusicFadeOut.hpp>
#include <OneLifeManager.hpp>
#include <OneLifeConstants.hpp>

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
        m_endSprite = cocos2d::CCSprite::createWithSpriteFrameName("d_heart01_001.png");
        m_endSprite->setColor({ 255, 0, 0 });
        m_endSprite->setScale(0.f);
        m_endSprite->setOpacity(0);
        addChildAtPosition(m_endSprite, geode::Anchor::Center, ccp(0, 0), false);

        cocos2d::CCScaleTo* scaleAction = cocos2d::CCScaleTo::create(3, 2.f);
        cocos2d::CCEaseOut* scaleEase = cocos2d::CCEaseOut::create(scaleAction, 2.f);

        cocos2d::CCFadeIn* fadeAction = cocos2d::CCFadeIn::create(1.f);

        cocos2d::CCDelayTime* delayShake = cocos2d::CCDelayTime::create(0.5f);

        cocos2d::CCMoveBy* shakeAction1 = cocos2d::CCMoveBy::create(0.1f, { 1, 2 });
        cocos2d::CCMoveBy* shakeAction2 = cocos2d::CCMoveBy::create(0.1f, { -1, -2 });
        cocos2d::CCMoveBy* shakeAction3 = cocos2d::CCMoveBy::create(0.1f, { 2, 0.5 });
        cocos2d::CCMoveBy* shakeAction4 = cocos2d::CCMoveBy::create(0.1f, { -2, -0.5 });
        
        cocos2d::CCSequence* shakeSequence = cocos2d::CCSequence::create(
            shakeAction1,
            shakeAction2,
            shakeAction3,
            shakeAction4,
            0
        );
        
        cocos2d::CCRepeat* repeatShake = cocos2d::CCRepeat::create(
            shakeSequence,
            6
        );
        
        cocos2d::CCSequence* shakeSequenceDelay = cocos2d::CCSequence::create(
            delayShake,
            repeatShake,
            0
        );

        cocos2d::CCSpawn* combinedAction = cocos2d::CCSpawn::create(
            scaleEase,
            fadeAction,
            shakeSequenceDelay,
            0
        );

        cocos2d::CCScaleBy* endScaleAction = cocos2d::CCScaleBy::create(0.5f, 1.7f);
        cocos2d::CCEaseInOut* endScaleEase = cocos2d::CCEaseInOut::create(endScaleAction, 2.f);
        
        cocos2d::CCScaleTo* endScaleAction2 = cocos2d::CCScaleTo::create(0.75f, 0.f);
        cocos2d::CCEaseInOut* endScaleEase2 = cocos2d::CCEaseInOut::create(endScaleAction2, 2.f);

        cocos2d::CCFadeOut* endFadeAction = cocos2d::CCFadeOut::create(1.f);
        cocos2d::CCEaseInOut* endFadeEase = cocos2d::CCEaseInOut::create(endFadeAction, 2.f);

        cocos2d::CCSpawn* endActionCombined = cocos2d::CCSpawn::create(
            endScaleEase2,
            endFadeEase,
            0
        );

        cocos2d::CCDelayTime* endDelay = cocos2d::CCDelayTime::create(0.1f);
        cocos2d::CCCallFunc* playEndSFX = cocos2d::CCCallFunc::create(this, callfunc_selector(OneLifeEffectLayer::playEndSFX));

        cocos2d::CCSequence* endScaleSequence = cocos2d::CCSequence::create(
            endScaleEase,
            endActionCombined,
            endDelay,
            0
        );

        cocos2d::CCTintTo* endTintAction = cocos2d::CCTintTo::create(0.4f, 55, 55, 55);

        cocos2d::CCSpawn* endCombinedAction = cocos2d::CCSpawn::create(
            endScaleSequence,
            endTintAction,
            playEndSFX,
            0
        );

        cocos2d::CCSequence* fullSequence = cocos2d::CCSequence::create(
            combinedAction,
            endCombinedAction,
            endAction,
            0
        );

        m_endSprite->runAction(fullSequence);

    } else {
        FLAlertLayer::create(
            OneLifeConstants::GENERIC_ERROR,
            OneLifeConstants::ERROR_INVALID_EFFECT,
            OneLifeConstants::GENERIC_OK
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

void OneLifeEffectLayer::playEndSFX() {
    auto path = MusicDownloadManager::sharedState()->pathForSFX(4290);
    FMODAudioEngine::sharedEngine()->playEffect(path);
}

void OneLifeEffectLayer::endEffect() {
    toggleRunState();

    int sfx = 566;

    if (m_type != OneLifeEffectType::End) {
        auto path = MusicDownloadManager::sharedState()->pathForSFX(sfx);
        FMODAudioEngine::sharedEngine()->playEffect(path);
    }

    FMODAudioEngine::get()->stopAllMusic(true);
    FMODAudioEngine::get()->stopAllActions();
    
    GameManager::get()->getActionManager()->addAction(cocos2d::CCSequence::create(
        cocos2d::CCDelayTime::create(0.5f),
        cocos2d::CCCallFunc::create(GameManager::get(), callfunc_selector(GameManager::fadeInMenuMusic)),
        0
    ), GameManager::get(), false);

    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
    cocos2d::CCTouchDispatcher::get()->unregisterForcePrio(this);
}

void OneLifeEffectLayer::toggleRunState() {
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::get();
    
    olm->setFromStartedRun(true);
    olm->toggleRun(RunType::Mixed);
    if (auto playLayer = PlayLayer::get())
        playLayer->onExit();

    director->replaceScene(cocos2d::CCTransitionFade::create(0.5f, MenuLayer::scene(false)));

}