#pragma once

#include QMK_KEYBOARD_H

enum custom_keycodes {
    SWITCH_LANG = QK_KB_0,
    LANG_EN,
    LANG_RU,

    AT_EN,
    CARET_EN,
    VERBAR_EN,
    DOT_EN,
    COMMA_EN,
    ANGLE_LT_EN,
    ANGLE_GT_EN,

    HASH_EN,
    DOLLAR_EN,
    AMP_EN,
    QUEST_RUEN,
    USER_QUOTES,
    SINGLE_QUOTE_EN,
    USER_COLON,
    SEMICOLON_EN,

    LSBRC_EN,
    RSBRC_EN,
    LCBRC_EN,
    RCBRC_EN,

    MINUS_EQUAL,
    ARROW_FN,

    DOT_COMMA_RUEN,
    SLASH_BSLS_RUEN,

    LBRC_RUEN,
    RBRC_RUEN,
    NUM_LAYER_5_TIMEOUT,
    GRAVE_RUEN,

    AUTO_SWAP_M_RUEN,
    ALT_TAB_MODE,
    AUTO_SWAP_T_RUEN,
};
