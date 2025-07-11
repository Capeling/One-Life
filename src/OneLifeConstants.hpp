#pragma once

#include <Geode/DefaultInclude.hpp>

struct OneLifeConstants {
    static constexpr const char* POPUP_TITLE = "One Life";
    static constexpr const char* POPUP_IN_RUN = "in a run";
    static constexpr const char* POPUP_NOT_IN_RUN = "not in a run";
    static constexpr const char* POPUP_SPRITE_BACKGROUND = "geode.loader/GE_square03.png";
    
    static constexpr const char* POPUP_START_RUN = "Start Run";
    static constexpr const char* POPUP_END_RUN = "End Run";
    static constexpr const char* POPUP_START_RUN_DESC = "Are you sure you want to <cg>start a run?</c>";
    static constexpr const char* POPUP_END_RUN_DESC = "Are you sure you want to <cr>end the run?</c>";
    
    static constexpr const char* INFO_TITLE = "Info";
    static constexpr const char* INFO_DESC = "<cl>One Life</c> is like a roguelike for Geometry Dash, when you start a run all stats you gain <cl>from levels only</c> are counted towards your run stats.\n\nYour run stats are calculated into an <cg>exp</c> value <cl>which is the main way you should base your run level on</c>.\n\nDepending on the difficulty of a demon level you will gain more demon points, e.g. <cl>an extreme demon will give you 5 demon points while an easy demon will only give you 1</c>.";
    
    static constexpr const char* GENERIC_ERROR = "ERROR!";
    static constexpr const char* GENERIC_CANCEL = "Cancel";
    static constexpr const char* GENERIC_CONFIRM = "Yes";
    static constexpr const char* GENERIC_OK = "OK";
    
    static constexpr const char* SPRITE_HEART = "d_heart01_001.png";
    static constexpr const char* SPRITE_HEART_OVERLAY = "d_heart01_color_001.png";
    
    static constexpr const char* SPRITE_DEBUG = "GJ_editBtn_001.png";

    static constexpr const char* SPRITE_START = "GJ_starBtnMod_001.png";
    static constexpr const char* SPRITE_INFO = "GJ_infoIcon_001.png";
    static constexpr const char* SPRITE_END = "GJ_starBtn2_001.png";
    
    static constexpr const char* ERROR_INVALID_EFFECT = "Invalid effect, something has gone horribly wrong!\nContact @capeling on discord for help.";
    static constexpr const char* ERROR_UNKNOWN_NODE = "Failed to get \'{}\'.";
    
    static constexpr int SFX_START = 566;
    static constexpr int SFX_END = 4290;
    static constexpr int SFX_COUNTDOWN_POP = 4292;
    
    static constexpr cocos2d::ccColor3B COLOR_HEART_ACTIVE = {255, 0 ,0};
    static constexpr cocos2d::ccColor3B COLOR_HEART_INACTIVE = {95, 85 ,85};
    
    static constexpr int COUNTDOWN_START = 3;
    static constexpr int MAX_END_TIME = 3;
};