#include QMK_KEYBOARD_H
#include "symbols.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_ESC,     KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_TAB,     KC_A,   KC_S,   KC_D,   KC_F,   KC_G,          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT,    KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,          KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                            KC_LGUI,KC_ESC, KC_LCTL, KC_SPC,       KC_BSPC,  KC_DEL,  KC_ENT, KC_RALT
    )
};

enum my_keycodes {
    SWITCH_LANG = QK_KB_0,
    LANG_EN,
    LANG_RU,

    AT_EN,                  // @
    CARET_EN,               // ^
    VERBAR_EN,              // |
    DOT_EN,                 // .
    COMMA_EN,               // ,

    HASH_EN,                // #@
    DOLLAR_EN,              // $%
    AMP_EN,                 // &|
    QUEST_EN,               // ?!
    QUOTES_EN,              // '"
    SEMICOLON_EN,           // ;:

    L_BRCS_EN,              // ([
    R_BRCS_EN,              // )]
    LBRC_EN,                // [
    RBRC_EN,                // ]
    LSFT_LBRC_EN,           // {
    LSFT_RBRC_EN,           // }

    ROUND_BRCS,             // ()
    // SQUARE_BRCS_EN,
    // CURLY_BRCS_EN,
};

typedef enum {
    LANG_STATE_EN = 0,
    LANG_STATE_RU = 1,
} lang_state_t;

lang_state_t current_lang = LANG_STATE_EN;

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    const uint8_t mods = get_mods();
    const bool shifted = (mods & MOD_MASK_SHIFT) != 0;

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


        // -------------
        // SHIFTED CHARS
        // -------------

        case HASH_EN: {                                 // # @
            del_mods(MOD_MASK_SHIFT);

            if (shifted) {
                send_en_symbol(send_at);          // @
            } else {
                send_en_symbol(send_hash);        // #
            }

            set_mods(mods);
            return false;
        }
        case DOLLAR_EN: {                               // $ %
            del_mods(MOD_MASK_SHIFT);

            if (shifted) {
                send_percent();                         // %
            } else {
                send_en_symbol(send_dollar);      // $
            }

            set_mods(mods);
            return false;
        }
        case AMP_EN: {                                  // & |
            del_mods(MOD_MASK_SHIFT);

            if (shifted) {
                send_en_symbol(send_verbar);      // |
            } else {
                send_en_symbol(send_amp);         // &
            }

            set_mods(mods);
            return false;
        }
        case QUEST_EN: {                                // ?!
            if (shifted) {
                tap_code16(KC_1);                       // !
            } else {
                del_mods(MOD_MASK_SHIFT);
                send_en_symbol(send_quest);       // ?
                set_mods(mods);
            }
            return false;
        }
        case QUOTES_EN: {                               // ' "
            del_mods(MOD_MASK_SHIFT);

            if (shifted) {
                send_en_symbol(send_double_quote); // "
            } else {
                send_en_symbol(send_single_quote); // '
            }

            set_mods(mods);
            return false;
        }
        case SEMICOLON_EN: {                             // ;:
            send_en_symbol(send_colon);
            return false;
        }


        // ------
        // BRACES
        // ------

        case L_BRCS_EN: // ([
            if (shifted) {
                clear_mods();
                send_en_symbol(send_square_left_bracket);
                set_mods(mods);
            } else {
                register_mods(MOD_LSFT);
                tap_code16(KC_9);
                unregister_mods(MOD_LSFT);
            }
            return false;
            case R_BRCS_EN: // )]
            if (shifted) {
                clear_mods();
                send_en_symbol(send_square_right_bracket);
                set_mods(mods);
            } else {
                register_mods(MOD_LSFT);
                tap_code16(KC_0);
                unregister_mods(MOD_LSFT);
            }
            return false;
        case LBRC_EN: // [
            send_en_symbol(send_square_left_bracket);
            return false;
        case RBRC_EN: // ]
            send_en_symbol(send_square_right_bracket);
            return false;
        // {
        case LSFT_LBRC_EN:
            send_en_symbol(send_curly_left_bracket);
            return false;
        // }
        case LSFT_RBRC_EN:
            send_en_symbol(send_curly_right_bracket);
            return false;

        // (|)
        case ROUND_BRCS: {
            uint8_t mods = get_mods();
            clear_mods();
            register_mods(MOD_LSFT);
            tap_code16(KC_9);
            tap_code16(KC_0);
            unregister_mods(MOD_LSFT);
            wait_ms(20);

            tap_code16(KC_LEFT);

            set_mods(mods);
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

            del_mods(MOD_MASK_SHIFT);

            send_ru_symbol(send_square_right_bracket);

            set_mods(mods);
            return false;
        }
    }

    return true;
}
