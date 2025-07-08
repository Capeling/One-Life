#include <ui/OneLifeEffectLayer.hpp>

#include <nodes/FadeMusicAction.hpp>
#include <OneLifeManager.hpp>
#include <OneLifeConstants.hpp>
#include <hooks/MenuLayer.hpp>

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
    
    auto director = cocos2d::CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    cocos2d::CCTouchDispatcher::get()->registerForcePrio(this, 2);

    if (!CCLayerColor::initWithColor({ 0, 0, 0, 150 }))
        return false;
    

    this->m_type = type;
    
    auto colorOpacityAction = cocos2d::CCFadeTo::create(0.15f, 150);
    
    this->setOpacity(0);
    this->runAction(colorOpacityAction);
    GameManager::get()->getActionManager()->addAction(FadeMusicAction::create(OneLifeConstants::COUNTDOWN_START, FadeMusicDirection::FadeOut), FMODAudioEngine::get(), false);
    // cocos2d::CCScene::get()->runAction(FadeMusicAction::create(6.f, FadeMusicDirection::FadeOut));
    
    auto endCallAction = cocos2d::CCCallFunc::create(this, callfunc_selector(OneLifeEffectLayer::endEffect));

    auto endAction = cocos2d::CCSequence::create(
        endCallAction,
        0
    );
    
    if (type == OneLifeEffectType::Start) {
        m_counterStage = OneLifeConstants::COUNTDOWN_START;

        m_counterLabel = cocos2d::CCLabelBMFont::create(fmt::format("{}", m_counterStage).c_str(), "bigFont.fnt");
        m_counterLabel->setScale(0.f);
        addChildAtPosition(m_counterLabel, geode::Anchor::Center, ccp(0, 0), false);
        
        auto scaleAction = cocos2d::CCScaleTo::create(1.f, 2);
        auto scaleEase = cocos2d::CCEaseSineOut::create(scaleAction);
        
        auto opacityAction = cocos2d::CCFadeOut::create(1.f);
        
        auto tintAction = cocos2d::CCTintTo::create(1.f, 255, 0, 0);
        
        auto combinedAction = cocos2d::CCSpawn::create(
            scaleEase,
            opacityAction,
            tintAction,
            0
        );
        
        auto decreaseAction = cocos2d::CCCallFunc::create(this, callfunc_selector(OneLifeEffectLayer::countDownDecrease));

        auto countdownSequenceAction = cocos2d::CCSequence::create(
            combinedAction,
            decreaseAction,
            0
        );
        auto repeatAction = cocos2d::CCRepeat::create(countdownSequenceAction, OneLifeConstants::COUNTDOWN_START - 1);
        
        auto sequenceAction = cocos2d::CCSequence::create(
            repeatAction,
            combinedAction,
            endAction,
            0
        );
        
        auto path = MusicDownloadManager::sharedState()->pathForSFX(4292);
        FMODAudioEngine::sharedEngine()->playEffect(path);
        
        m_counterLabel->runAction(sequenceAction);
    } else if (type == OneLifeEffectType::End) {
        m_endSprite = cocos2d::CCSprite::createWithSpriteFrameName(OneLifeConstants::SPRITE_HEART);
        m_endSprite->setColor(OneLifeConstants::COLOR_HEART_ACTIVE);
        m_endSprite->setScale(0.f);
        m_endSprite->setOpacity(0);
        addChildAtPosition(m_endSprite, geode::Anchor::Center, ccp(0, 0), false);
        
        auto endOverlay = cocos2d::CCSprite::createWithSpriteFrameName(OneLifeConstants::SPRITE_HEART_OVERLAY);
        endOverlay->setOpacity(0);
        m_endSprite->addChildAtPosition(endOverlay, geode::Anchor::Center);
        
        cocos2d::CCScaleTo* scaleAction = cocos2d::CCScaleTo::create(OneLifeConstants::MAX_END_TIME, 2.f);
        cocos2d::CCEaseOut* scaleEase = cocos2d::CCEaseOut::create(scaleAction, 2.f);
        
        cocos2d::CCFadeIn* fadeAction = cocos2d::CCFadeIn::create(1.f);

        cocos2d::CCFadeIn* overlayFadeAction = cocos2d::CCFadeIn::create(1.f);
        endOverlay->runAction(overlayFadeAction);
        
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
            OneLifeConstants::MAX_END_TIME * 2.5
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
        
        cocos2d::CCTintTo* endTintAction = cocos2d::CCTintTo::create(
            0.4f,
            OneLifeConstants::COLOR_HEART_INACTIVE.r,
            OneLifeConstants::COLOR_HEART_INACTIVE.g,
            OneLifeConstants::COLOR_HEART_INACTIVE.b
        );
        
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
    
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);
    
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

    auto path = MusicDownloadManager::sharedState()->pathForSFX(OneLifeConstants::SFX_COUNTDOWN_POP);
    FMODAudioEngine::sharedEngine()->playEffect(path);
}

void OneLifeEffectLayer::playEndSFX() {
    auto path = MusicDownloadManager::sharedState()->pathForSFX(OneLifeConstants::SFX_END);
    FMODAudioEngine::sharedEngine()->playEffect(path);
}

void OneLifeEffectLayer::endEffect() {
    toggleRunState();
    
    if (m_type == OneLifeEffectType::Start) {
        auto path = MusicDownloadManager::sharedState()->pathForSFX(OneLifeConstants::SFX_START);
        FMODAudioEngine::sharedEngine()->playEffect(path);
    }
    
    FMODAudioEngine::get()->stopAllMusic(true);
    FMODAudioEngine::get()->stopAllActions();
    
    GameManager::get()->getActionManager()->addAction(cocos2d::CCSequence::create(
        cocos2d::CCDelayTime::create(0.5f),
        cocos2d::CCCallFunc::create(GameManager::get(), callfunc_selector(GameManager::fadeInMenuMusic)),
        0
    ), GameManager::get(), false);

    cocos2d::CCFadeTo* fadeOutBG = cocos2d::CCFadeTo::create(.5f, 0.f);
    cocos2d::CCCallFunc* removeMeFunc = cocos2d::CCCallFunc::create(this, callfunc_selector(OneLifeEffectLayer::removeMe));

    this->runAction(cocos2d::CCSequence::create(
        fadeOutBG,
        removeMeFunc,
        0
    ));
}

void OneLifeEffectLayer::removeMe() {
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void OneLifeEffectLayer::toggleRunState() {
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::get();
    
    olm->setFromStartedRun(true);
    olm->toggleRun(RunType::Mixed);

    if (auto playLayer = PlayLayer::get()) {
        playLayer->resetAudio();
        FMODAudioEngine::get()->unloadAllEffects();
        director->replaceScene(cocos2d::CCTransitionFade::create(0.5f, MenuLayer::scene(false)));
    }

    if (auto menuLayer = MenuLayer::get()) {
        static_cast<HookMenuLayer*>(menuLayer)->updateOneLifeBtn();
    }

}
