#include QMK_KEYBOARD_H
#include "auto_swap_key.h"
#include "symbols.h"
#include "windows_alt_tab.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {};

#define WINDOWS_ALT_TAB_LAYER 6
#define WINDOWS_ALT_TAB_TIMEOUT_MS 3000
#define WINDOWS_ALT_TAB_INITIAL_STEP_DELAY_MS 40

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SFT_T(KC_QUOTE):
            return true;
        default:
            return false;
    }
}

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
    QUEST_RUEN,             // ?
    USER_QUOTES,            // '"
    SINGLE_QUOTE_EN,        // '
    USER_COLON,             // :;
    SEMICOLON_EN,           // ;

    LSBRC_EN,               // [
    RSBRC_EN,               // ]
    LCBRC_EN,               // {
    RCBRC_EN,               // }

    MINUS_EQUAL,            // -=
    ARROW_FN,               // =>

    DOT_COMMA_RUEN,         // .,
    SLASH_BSLS_RUEN,        // /\\ on EN; ., on RU

    LBRC_RUEN,              // ([
    RBRC_RUEN,              // )]
    LAYER_5_3S,             // Layer 5 for 3 seconds
    GRAVE_RUEN,             // `

    AUTO_SWAP_M_RUEN,       // m/ь; hold on RU => ъ
    ALT_TAB_MODE,           // Windows Alt+Tab mode
};

#define TEMP_LAYER_5 5
#define TEMP_LAYER_5_TIMEOUT 2000

typedef enum {
    LANG_STATE_EN = 0,
    LANG_STATE_RU = 1,
} lang_state_t;

lang_state_t current_lang = LANG_STATE_EN;

static auto_swap_key_t auto_swap_keys[] = {
    AUTO_SWAP_KEY_WITH_TIMEOUT(AUTO_SWAP_M_RUEN, KC_M, KC_RBRC, 200),
};
#define AUTO_SWAP_KEYS_COUNT ARRAY_SIZE(auto_swap_keys)

static uint16_t temp_layer_5_timer = 0;
static bool temp_layer_5_active = false;

void keyboard_post_init_user(void) {
    windows_alt_tab_init(
        ALT_TAB_MODE,
        WINDOWS_ALT_TAB_LAYER,
        WINDOWS_ALT_TAB_TIMEOUT_MS,
        WINDOWS_ALT_TAB_INITIAL_STEP_DELAY_MS
    );
}

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
    if (current_lang != LANG_STATE_EN) {
        switch_to_en();
        wait_ms(10);
        send();
        wait_ms(1);
        switch_to_ru();
        return;
    }

    send();
    return;
}

// void send_ru_symbol(void (*send)(void)) {
//     lang_state_t prev = current_lang;
//
//     if (current_lang != LANG_STATE_RU) {
//         switch_to_ru();
//         wait_ms(10);
//     }
//
//     send();
//
//     if (prev != LANG_STATE_RU) {
//         wait_ms(10);
//         switch_to_en();
//     }
//     return;
// }

static void activate_temp_layer_5(void) {
    layer_clear();
    layer_on(TEMP_LAYER_5);
    temp_layer_5_timer = timer_read();
    temp_layer_5_active = true;
}

static void deactivate_temp_layer_5(void) {
    layer_clear();
    temp_layer_5_active = false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!windows_alt_tab_process_record(keycode, record)) {
        return false;
    }

    if (!process_auto_swap_keys(keycode, record, current_lang == LANG_STATE_RU, auto_swap_keys, AUTO_SWAP_KEYS_COUNT)) {
        return false;
    }

    if (!record->event.pressed) {
        return true;
    }

    if (temp_layer_5_active && layer_state_is(TEMP_LAYER_5)) {
        temp_layer_5_timer = timer_read();
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
        case LAYER_5_3S:                                // Layer 5 for 3 seconds
            activate_temp_layer_5();
            return false;


        // ------------
        // SINGLE CHARS
        // ------------
        case AT_EN:                                     // @
            send_en_symbol(send_at);
            return false;
        case HASH_EN:                                   // #
            send_en_symbol(send_hash);
            return false;
        case DOLLAR_EN:                                 // $
            send_en_symbol(send_dollar);
            return false;
        case AMP_EN:                                    // &
            send_en_symbol(send_amp);
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
        case ANGLE_LT_EN:                               // <
            send_en_symbol(send_less);
            return false;
        case ANGLE_GT_EN:                               // >
            send_en_symbol(send_greater);
            return false;
        case GRAVE_RUEN: {                                // `
            clear_active_mods();
            send_en_symbol(send_grave);
            restore_held_mods(mod_state);
            return false;
        }


        // -------------
        // SHIFTED CHARS
        // -------------
        case QUEST_RUEN: {                                  // ? Учитывает язык раскладки
            clear_active_mods();

            if (current_lang == LANG_STATE_EN)
                send_quest();                           // ?
            else
                send_ru_quest();                        // ?

            restore_held_mods(mod_state);
            return false;
        }
        case USER_QUOTES: {                                 // "' Учитывает язык раскладки (только для ")
            clear_active_mods();

            if (shifted) {
                send_en_symbol(send_single_quote);          // ' На EN раскладке
            } else {
                if (current_lang == LANG_STATE_EN)
                    send_double_quote();                    // "
                else
                    send_ru_double_quote();                 // "
            }

            restore_held_mods(mod_state);
            return false;
        }
        case SINGLE_QUOTE_EN: {                             // ' На EN раскладке
            clear_active_mods();
            send_en_symbol(send_single_quote);
            restore_held_mods(mod_state);
            return false;
        }
        case USER_COLON: {                                  // :; Учитывает язык раскладки
            clear_active_mods();

            if (shifted) {
                if (current_lang == LANG_STATE_EN)
                    send_semicolon();                       // ;
                else
                    send_ru_semicolon();                    // ;
            } else {
                if (current_lang == LANG_STATE_EN)
                    send_colon();                           // :
                else
                    send_ru_colon();                        // :
            }

            restore_held_mods(mod_state);
            return false;
        }
        case DOT_COMMA_RUEN: {                                  // ., Учитывает язык раскладки
            clear_active_mods();

            if (shifted) {
                if (current_lang == LANG_STATE_EN)
                    send_comma();                           // ,
                else
                    send_ru_comma();                        // ,
            } else {
                if (current_lang == LANG_STATE_EN)
                    send_dot();                             // .
                else
                    send_ru_dot();                          // .
            }

            restore_held_mods(mod_state);
            return false;
        }
        case SEMICOLON_EN:                                  // ; На EN раскладке
            clear_active_mods();
            send_en_symbol(send_semicolon);
            restore_held_mods(mod_state);
            return false;

        case SLASH_BSLS_RUEN: {                            // /\ on EN ., on RU
            const bool shift_only =
                shifted && (effective_mods & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI)) == 0;

            if (current_lang == LANG_STATE_EN && shift_only) {
                clear_active_mods();
                tap_code(KC_BSLS);
                restore_held_mods(mod_state);
                return false;
            }

            tap_code(KC_SLSH);
            return false;
        }


        // ------
        // BRACES
        // ------
        case LSBRC_EN:
            clear_active_mods();
            send_en_symbol(send_left_square_bracket);       // [
            restore_held_mods(mod_state);
            return false;
        case RSBRC_EN:
            clear_active_mods();
            send_en_symbol(send_right_square_bracket);      // ]
            restore_held_mods(mod_state);
            return false;
        case LCBRC_EN:
            clear_active_mods();
            send_en_symbol(send_left_curly_bracket);        // {
            restore_held_mods(mod_state);
            return false;
        case RCBRC_EN:
            clear_active_mods();
            send_en_symbol(send_right_curly_bracket);       // }
            restore_held_mods(mod_state);
            return false;

        case LBRC_RUEN: {                                  // ([ Учитывает язык раскладки
            clear_active_mods();

            if (shifted) {
                clear_active_mods();
                send_en_symbol(send_left_square_bracket);
                restore_held_mods(mod_state);
            } else {
                register_mods(MOD_LSFT);
                tap_code(KC_9);
                unregister_mods(MOD_LSFT);
            }

            restore_held_mods(mod_state);
            return false;
        }

        case RBRC_RUEN: {                                  // )] Учитывает язык раскладки
            clear_active_mods();

            if (shifted) {
                send_en_symbol(send_right_square_bracket);
            } else {
                register_mods(MOD_LSFT);
                tap_code(KC_0);
                unregister_mods(MOD_LSFT);
            }

            restore_held_mods(mod_state);
            return false;
        }


        // ------
        // MISC
        // ------
        case MINUS_EQUAL: // -=
            if (shifted) {
                clear_active_mods();
                tap_code(KC_EQUAL);
                restore_held_mods(mod_state);
            } else {
                tap_code16(KC_MINUS);
            }
            return false;
        case ARROW_FN: // =>
            clear_active_mods();
            send_en_symbol(send_fat_arrow);
            restore_held_mods(mod_state);
            return false;


        // -------
        // RUSSIAN
        // -------
        case KC_M: {                                        // Shift + "ь" => "ъ" только для RU
            if (current_lang == LANG_STATE_EN) {
                return true;                                // en игнорируем
            }

            if (!shifted) {
                return true;                                // без shit будет напечатана "ь"
            }

            clear_active_mods();
            send_right_square_bracket();                    // с shit будет напечатана "ъ"
            restore_held_mods(mod_state);
            return false;
        }
    }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return windows_alt_tab_layer_state_set(state);
}

void housekeeping_task_user(void) {
    // Housekeeping runs after QMK has processed this loop's key events.
    // Navigation can therefore refresh the timer before timeout is checked.
    windows_alt_tab_task();
}

void matrix_scan_user(void) {
    auto_swap_key_task(auto_swap_keys, AUTO_SWAP_KEYS_COUNT);

    if (!temp_layer_5_active) {
        return;
    }

    if (!layer_state_is(TEMP_LAYER_5)) {
        temp_layer_5_active = false;
        return;
    }

    if (timer_elapsed(temp_layer_5_timer) >= TEMP_LAYER_5_TIMEOUT) {
        deactivate_temp_layer_5();
    }
}

// TAPPING_TERM для LSFT_T(KC_A) и RSFT_T(KC_SCLN) = 145 ms
// uint16_t get_tapping_term_user(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         case LSFT_T(KC_A):
//         case RSFT_T(KC_SCLN):
//             return 145;
//         default:
//             return TAPPING_TERM;
//     }
// }
