#include <ui/OneLifePopup.hpp>
#include <ui/OneLifeEffectLayer.hpp>
#include <OneLifeManager.hpp>
#include <OneLifeConstants.hpp>

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
    
    // m_statArea = TextArea::create(running ? olm->getCurrentStatString() : olm->getBestStatString(),
    //     "bigFont.fnt",
    //     0.6f,
    //     250,
    //     { 0.5f, 0.5f },
    //     25,
    //     false
    // );
    // m_statArea->setScale(0.6f);
    // m_mainLayer->addChildAtPosition(m_statArea, geode::Anchor::Center, ccp(0, 17));

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
            FLAlertLayer::create(OneLifeConstants::INFO_TITLE, OneLifeConstants::INFO_DESC, OneLifeConstants::GENERIC_OK)->show();
        }
    );
    m_buttonMenu->addChildAtPosition(m_infoBtn, geode::Anchor::TopRight, ccp(-20, -20));

    std::string versionString = fmt::format("Mod Version: {}\nMod Commit: {}\nMod Build Type: {}",
        geode::Mod::get()->getVersion().toVString().c_str(),
        OneLifeConstants::MOD_COMMIT,
        OneLifeConstants::MOD_BUILD_TYPE
    );

    m_versionLabel = cocos2d::CCLabelBMFont::create(versionString.c_str(), "chatFont.fnt");
    m_versionLabel->setScale(0.5f);
    m_versionLabel->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentCenter);
    m_versionLabel->setOpacity(255 / 2);
    m_mainLayer->addChildAtPosition(m_versionLabel, geode::Anchor::Bottom, ccp(0, 23));

    // auto size = cocos2d::CCSize{300.f, 60.f};
    // auto bg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
    // bg->setColor({0,0,0});
    // bg->setOpacity(100);
    // bg->setContentSize(size);
    
    // auto msg = CCTextInputNode::create(size.width, size.height - 10.f, "Message (optional)", "Thonburi", 24, 0);
    // msg->setMaxLabelLength(1000);
    // msg->addTextArea(TextArea::create("", "chatFont.fnt", 1.f, size.width - 40.0f, {0.5, 0.5}, 25.0f, true));
    // msg->m_isChatFont = true;
    // msg->setUserObject("fix-text-input", cocos2d::CCBool::create(true));
    
    // m_mainLayer->addChildAtPosition(bg, geode::Anchor::Center);
    // m_mainLayer->addChildAtPosition(msg, geode::Anchor::Center);

    // bool isLevelInfoActive = false;

    // LevelInfoLayer* levelInfoLayer = nullptr;

    // auto scene = cocos2d::CCScene::get();
    // for (int i = 0; i < scene->getChildrenCount(); i++) {
    //     geode::log::debug("aaaa");
    //     auto node = scene->getChildren()->objectAtIndex(i);

    //     if (auto levelInfoNode = geode::cast::typeinfo_cast<LevelInfoLayer*>(node)) {
    //         isLevelInfoActive = true;
    //         levelInfoLayer = levelInfoNode;
    //         break;
    //     }
    // }
    
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

