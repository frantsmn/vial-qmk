#pragma once

#include QMK_KEYBOARD_H

enum custom_keycodes {

// --- Functions

    SWITCH_LANG = QK_KB_0,  // Switch languages (LCTL+1 | LCTL+2)
    LANG_EN,                // Switch to EN (LCTL+1)
    LANG_RU,                // Switch to RU (LCTL+2)
    ALT_TAB_MODE,           // Windows Alt-Tab mode
    NUM_LAYER_TIMEOUT,      // Switch to NUM layer (2 sec timeout)

    USER_FN_5,              // Reserved
    USER_FN_4,              // Reserved
    USER_FN_3,              // Reserved
    USER_FN_2,              // Reserved
    USER_FN_1,              // Reserved

// --- Symbols

    GRAVE_EN,               // `
    TILDE_EN,               // ~
    AT_EN,                  // @
    HASH_EN,                // #
    DOLLAR_EN,              // $
    CARET_EN,               // ^
    AMP_EN,                 // &
    VERBAR_EN,              // |
    DOT_COMMA_EN,           // . ,
    COMMA_EN,               // ,
    QUOTES_EN,              // " '
    SINGLE_QUOTE_EN,        // '
    COLONS_EN,              // : ;
    SEMICOLON_EN,           // ;
    QUEST_EN,               // ? !
    SLASH_BSLS_EN,          // / and backslash

    USER_SYM_5,             // Reserved
    USER_SYM_4,             // Reserved
    USER_SYM_3,             // Reserved
    USER_SYM_2,             // Reserved
    USER_SYM_1,             // Reserved

// --- Braces

    LANGLE_EN,              // <
    RANGLE_EN,              // >
    LBRC_EN,                // ( [
    RBRC_EN,                // ) ]
    LSBRC_EN,               // [
    RSBRC_EN,               // ]
    LCBRC_EN,               // {
    RCBRC_EN,               // }

// --- Custom

    USER_CUS_1,             // Reserved
    USER_CUS_2,             // Reserved
    USER_CUS_3,             // Reserved

    YO_RU,                  // ё
    HARD_SIGN_RU,           // ъ
    ARROW_FN,               // =>
    MINUS_PLUS,             // - +

    USER_CUS_8,             // Reserved
    USER_CUS_9,             // Reserved
    USER_CUS_10,            // Reserved

// --- Code

    CODE_COML_OR_TRNS,
    CODE_COMB_OR_TRNS,
    CODE_COPYP_OR_TRNS,
};
