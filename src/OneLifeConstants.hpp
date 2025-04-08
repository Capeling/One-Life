#pragma once

struct OneLifeConstants {
    static constexpr const char* POPUP_TITLE = "One Life";
    static constexpr const char* POPUP_IN_RUN = "In a run";
    static constexpr const char* POPUP_NOT_IN_RUN = "Not in a run";
    static constexpr const char* POPUP_SPRITE_BACKGROUND = "geode.loader/GE_square03.png";

    static constexpr const char* POPUP_START_RUN = "Start Run";
    static constexpr const char* POPUP_END_RUN = "End Run";
    static constexpr const char* POPUP_START_RUN_DESC = "Are you sure you want to <cl>start a run?</c>";
    static constexpr const char* POPUP_END_RUN_DESC = "Are you sure you want to <cr>end the run?</c>";
    
    static constexpr const char* GENERIC_ERROR = "ERROR!";
    static constexpr const char* GENERIC_CANCEL = "CANCEL";
    static constexpr const char* GENERIC_CONFIRM = "YES";
    static constexpr const char* GENERIC_OK = "OK";

    static constexpr const char* SPRITE_HEART = "d_heart01_001.png";
    static constexpr const char* SPRITE_START = "GJ_starBtnMod_001.png";
    static constexpr const char* SPRITE_END = "GJ_starBtn2_001.png";

    static constexpr const char* ERROR_INVALID_EFFECT = "Invalid effect, something has gone horribly wrong!\nContact Capeling on discord for help.";
    static constexpr const char* ERROR_UNKNOWN_NODE = "Failed to get \'{}\'.";

    static constexpr int SFX_START = 566;
    static constexpr int SFX_END = 4290;
    static constexpr int SFX_COUNTDOWN_POP = 4292;
};