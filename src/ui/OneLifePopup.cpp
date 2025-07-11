#include <ui/OneLifePopup.hpp>
#include <ui/OneLifeEffectLayer.hpp>
#include <OneLifeManager.hpp>
#include <OneLifeConstants.hpp>
#include <BuildConstants.hpp>

OneLifePopup* OneLifePopup::create() {
    auto ret = new OneLifePopup();
    if (ret->initAnchored(330, 285, OneLifeConstants::POPUP_SPRITE_BACKGROUND)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OneLifePopup::setup() {
    auto olm = OneLifeManager::get();
    auto director = cocos2d::CCDirector::get();
    
    setTitle(OneLifeConstants::POPUP_TITLE, "bigFont.fnt");
    addCorners(Corner::Blue, Corner::Blue);

    bool running = olm->getIsRunning();

    m_statLabel = cocos2d::CCLabelBMFont::create(
        running ? olm->getCurrentStatString().c_str() : olm->getBestStatString().c_str(),
        "bigFont.fnt"
    );
    m_statLabel->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentCenter);
    m_statLabel->setScale(0.6f);
    m_mainLayer->addChildAtPosition(m_statLabel, geode::Anchor::Center, ccp(0, 17));

    m_runStateLabel = cocos2d::CCLabelBMFont::create(
        running ? OneLifeConstants::POPUP_IN_RUN : OneLifeConstants::POPUP_NOT_IN_RUN,
        "goldFont.fnt"
    );
    m_runStateLabel->setScale(0.6f);
    m_mainLayer->addChildAtPosition(m_runStateLabel, geode::Anchor::Top, ccp(0, -47));
    
    m_startBtn = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName(
        running ? OneLifeConstants::SPRITE_END : OneLifeConstants::SPRITE_START,
        1.f, 
        [this, running](auto sender){
            toggleRunBtn(!running);
        }
    );
    m_buttonMenu->addChildAtPosition(m_startBtn, geode::Anchor::Bottom, ccp(0, 67));

    m_infoBtn = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName(
        OneLifeConstants::SPRITE_INFO,
        0.8f,
        [this](auto sender) {
            FLAlertLayer::create(nullptr, OneLifeConstants::INFO_TITLE, OneLifeConstants::INFO_DESC, OneLifeConstants::GENERIC_OK, nullptr, 410.f, false, 350.f, .9f)->show();
        }
    );
    m_buttonMenu->addChildAtPosition(m_infoBtn, geode::Anchor::TopRight, ccp(-20, -20));

    std::string versionString = fmt::format("{}\n{}\n{}",
        geode::Mod::get()->getVersion().toVString().c_str(),
        BuildConstants::MOD_COMMIT,
        BuildConstants::MOD_BUILD_TYPE
    );

    m_versionLabel = cocos2d::CCLabelBMFont::create(versionString.c_str(), "chatFont.fnt");
    m_versionLabel->setScale(0.5f);
    m_versionLabel->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentCenter);
    m_versionLabel->setOpacity(255 / 2);
    m_mainLayer->addChildAtPosition(m_versionLabel, geode::Anchor::Bottom, ccp(0, 23));


#ifdef DEBUG_ENABLED
    auto instantSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
    instantSpr->setColor(OneLifeConstants::COLOR_HEART_INACTIVE);
    instantSpr->setOpacity(150);
    instantSpr->setScale(.45f);

    auto instantBtn = geode::cocos::CCMenuItemExt::createSpriteExtra(
        instantSpr,
        [this, running](auto sender){
            auto olm = OneLifeManager::get();
            auto director = cocos2d::CCDirector::get();
            
            if (!olm->getIsRunning()) {
                auto path = MusicDownloadManager::sharedState()->pathForSFX(OneLifeConstants::SFX_START);
                FMODAudioEngine::sharedEngine()->playEffect(path);
            } else {
                auto path = MusicDownloadManager::sharedState()->pathForSFX(OneLifeConstants::SFX_END);
                FMODAudioEngine::sharedEngine()->playEffect(path);
            }

            olm->setFromStartedRun(true);
            olm->toggleRun(RunType::Mixed);
            if (auto playLayer = PlayLayer::get())
                playLayer->onExit();
        
            director->replaceScene(cocos2d::CCTransitionFade::create(0.5f, MenuLayer::scene(false)));
        }
    );
    m_buttonMenu->addChildAtPosition(instantBtn, geode::Anchor::Bottom, ccp(45, 67));
#endif
    
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
                onClose(nullptr);

            }
        }
    );
}

