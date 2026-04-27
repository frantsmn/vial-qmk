#include QMK_KEYBOARD_H
#include "symbols.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {};

enum my_keycodes {
    SWITCH_LANG = QK_KB_0,
    LANG_EN,
    LANG_RU,

    AT_EN,                  // @
    CARET_EN,               // ^
    VERBAR_EN,              // |
    DOT_EN,                 // .
    COMMA_EN,               // ,
    ANGLE_LT_EN,            // <
    ANGLE_GT_EN,            // >

    HASH_EN,                // #
    DOLLAR_EN,              // $
    AMP_EN,                 // &
    QUEST_EN,               // ?!
    QUOTES_EN,              // '"
    COLON_SEMICOLON_EN,     // :;
    SEMICOLON_EN,           // ;

    MINUS_EQUAL,            // -=
    L_CBRC_EN,              // ({
    R_CBRC_EN,              // )}

    LBRC_EN,                // [{
    RBRC_EN,                // ]}
    LSFT_LBRC_EN,           // {
    LSFT_RBRC_EN,           // }

    ROUND_BRCS,             // ()
    ARROW_FN,               // =>
    SINGLE_QUOTE_EN,        // '

    // SQUARE_BRCS_EN,
    // CURLY_BRCS_EN,
};

typedef enum {
    LANG_STATE_EN = 0,
    LANG_STATE_RU = 1,
} lang_state_t;

lang_state_t current_lang = LANG_STATE_EN;

typedef struct {
    uint8_t mods;
#ifndef NO_ACTION_ONESHOT
    uint8_t oneshot_mods;
#endif
} mod_state_t;

static mod_state_t save_mod_state(void) {
    mod_state_t state = {
        .mods = get_mods(),
#ifndef NO_ACTION_ONESHOT
        .oneshot_mods = get_oneshot_mods(),
#endif
    };

    return state;
}

static uint8_t get_effective_mods(mod_state_t state) {
#ifndef NO_ACTION_ONESHOT
    return state.mods | state.oneshot_mods;
#else
    return state.mods;
#endif
}

static void clear_active_mods(void) {
    clear_mods();
#ifndef NO_ACTION_ONESHOT
    clear_oneshot_mods();
#endif
}

static void restore_held_mods(mod_state_t state) {
    set_mods(state.mods);
}

void switch_to_en(void) {
    register_code(KC_LCTL);
    tap_code(KC_1);
    unregister_code(KC_LCTL);
    current_lang = LANG_STATE_EN;
    return;
}

void switch_to_ru(void) {
    register_code(KC_LCTL);
    tap_code(KC_2);
    unregister_code(KC_LCTL);
    current_lang = LANG_STATE_RU;
    return;
}

void switch_lang(void) {
    switch (current_lang) {
        case LANG_STATE_EN: {
            switch_to_ru();
            return;
        }
        case LANG_STATE_RU: {
            switch_to_en();
            return;
        }
    }
    return;
}

void send_en_symbol(void (*send)(void)) {
    lang_state_t prev = current_lang;

    if (current_lang != LANG_STATE_EN) {
        switch_to_en();
        wait_ms(10);
    }

    send();

    if (prev != LANG_STATE_EN) {
        wait_ms(10);
        switch_to_ru();
    }
    return;
}

void send_ru_symbol(void (*send)(void)) {
    lang_state_t prev = current_lang;

    if (current_lang != LANG_STATE_RU) {
        switch_to_ru();
        wait_ms(10);
    }

    send();

    if (prev != LANG_STATE_RU) {
        wait_ms(10);
        switch_to_en();
    }
    return;
}

void send_fat_arrow(void) {
    tap_code(KC_EQL);
    send_greater();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    const mod_state_t mod_state = save_mod_state();
    const uint8_t effective_mods = get_effective_mods(mod_state);
    const bool shifted = (effective_mods & MOD_MASK_SHIFT) != 0;

    switch (keycode) {
        // -----------
        // LANG SWITCH
        // -----------

        case SWITCH_LANG:                               // EN <-> RU
            switch_lang();
            return false;
        case LANG_EN:                                   // EN
            switch_to_en();
            return false;
        case LANG_RU:                                   // RU
            switch_to_ru();
            return false;


        // ------------
        // SINGLE CHARS
        // ------------

        case AT_EN:                                     // @
            send_en_symbol(send_at);
            return false;
        case HASH_EN: {                                 // #
            send_en_symbol(send_hash);
            return false;
        }
        case DOLLAR_EN: {                               // $
            send_en_symbol(send_dollar);
            return false;
        }
        case AMP_EN: {                                  // &
            send_en_symbol(send_amp);
            return false;
        }
        case CARET_EN:                                  // ^
            send_en_symbol(send_caret);
            return false;
        case VERBAR_EN:                                 // |
            send_en_symbol(send_verbar);
            return false;
        case DOT_EN:                                    // .
            send_en_symbol(send_dot);
            return false;
        case COMMA_EN:                                  // ,
            send_en_symbol(send_comma);
            return false;
        case ANGLE_LT_EN:                               // <
            send_en_symbol(send_less);
            return false;
        case ANGLE_GT_EN:                               // >
            send_en_symbol(send_greater);
            return false;


        // -------------
        // SHIFTED CHARS
        // -------------
        case QUEST_EN: {                                // ?!
            if (shifted) {
                tap_code16(KC_1);                       // !
            } else {
                clear_active_mods();
                send_en_symbol(send_quest);       // ?
                restore_held_mods(mod_state);
            }
            return false;
        }
        case QUOTES_EN: {                               // " '
            clear_active_mods();

            if (shifted) {
                send_en_symbol(send_single_quote); // '
            } else {
                send_en_symbol(send_double_quote); // "
            }

            restore_held_mods(mod_state);
            return false;
        }
        case SINGLE_QUOTE_EN: {                    // '
            clear_active_mods();
            send_en_symbol(send_single_quote);
            restore_held_mods(mod_state);
            return false;
        }
        case COLON_SEMICOLON_EN: {               // :;
            clear_active_mods();

            if (shifted) {
              send_en_symbol(send_semicolon);   // ;
            } else {
              send_en_symbol(send_colon);       // :
            }

            restore_held_mods(mod_state);
            return false;
        }
        case SEMICOLON_EN:                      // ;
            clear_active_mods();
            send_en_symbol(send_semicolon);
            restore_held_mods(mod_state);
            return false;


        // ------
        // BRACES
        // ------
        case MINUS_EQUAL: // - =
            if (shifted) {
                clear_active_mods();
                tap_code(KC_EQUAL);
                restore_held_mods(mod_state);
            } else {
                tap_code16(KC_MINUS);
            }
            return false;
        case L_CBRC_EN: // ({
            if (shifted) {
                clear_active_mods();
                send_en_symbol(send_curly_left_bracket);
                restore_held_mods(mod_state);
            } else {
                register_mods(MOD_LSFT);
                tap_code16(KC_9);
                unregister_mods(MOD_LSFT);
            }
            return false;
        case R_CBRC_EN: // )}
            if (shifted) {
                clear_active_mods();
                send_en_symbol(send_curly_right_bracket);
                restore_held_mods(mod_state);
            } else {
                register_mods(MOD_LSFT);
                tap_code16(KC_0);
                unregister_mods(MOD_LSFT);
            }
            return false;
        case LBRC_EN: // [
            clear_active_mods();
            send_en_symbol(send_square_left_bracket);
            restore_held_mods(mod_state);
            return false;
        case RBRC_EN: // ]
            clear_active_mods();
            send_en_symbol(send_square_right_bracket);
            restore_held_mods(mod_state);
            return false;
        case LSFT_LBRC_EN: // {
            clear_active_mods();
            send_en_symbol(send_curly_left_bracket);
            restore_held_mods(mod_state);
            return false;
        case LSFT_RBRC_EN: // }
            clear_active_mods();
            send_en_symbol(send_curly_right_bracket);
            restore_held_mods(mod_state);
            return false;

        // (|)
        case ROUND_BRCS: {
            clear_active_mods();
            register_mods(MOD_LSFT);
            tap_code16(KC_9);
            tap_code16(KC_0);
            unregister_mods(MOD_LSFT);
            wait_ms(20);

            tap_code16(KC_LEFT);

            restore_held_mods(mod_state);
            return false;
        }

        // =>
        case ARROW_FN: {
            clear_active_mods();
            send_en_symbol(send_fat_arrow);
            restore_held_mods(mod_state);
            return false;
        }

        // // [|]
        // case SQUARE_BRCS_EN: {
        //     uint8_t mods = get_mods();
        //     clear_mods();

        //     tap_code16(KC_LBRC);
        //     tap_code16(KC_RBRC);
        //     wait_ms(20);

        //     tap_code16(KC_LEFT);

        //     set_mods(mods);
        //     return false;
        // }

        // // {|}
        // case CURLY_BRCS_EN: {
        //     uint8_t mods = get_mods();
        //     clear_mods();

        //     register_mods(MOD_LSFT);
        //     tap_code16(KC_LBRC);
        //     tap_code16(KC_RBRC);
        //     unregister_mods(MOD_LSFT);
        //     wait_ms(20);

        //     tap_code16(KC_LEFT);

        //     set_mods(mods);
        //     return false;
        // }


        // -------
        // RUSSIAN
        // -------

        // Shift + "ь" => "ъ" только для RU
        case KC_M: {
            if (current_lang == LANG_STATE_EN) {
                return true; // en игнорируем
            }

            if (!shifted) {
                return true; // без shit будет напечатана "ь"
            }

            clear_active_mods();

            send_ru_symbol(send_square_right_bracket);

            restore_held_mods(mod_state);
            return false;
        }
    }

    return true;
}

// TAPPING_TERM для LSFT_T(KC_A) и RSFT_T(KC_SCLN) = 140 ms
uint16_t get_tapping_term_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_A):
//         case RSFT_T(KC_SCLN):
            return 140;
        default:
            return TAPPING_TERM;
    }
}
