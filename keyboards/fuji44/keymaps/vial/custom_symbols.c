#include QMK_KEYBOARD_H
#include "custom_symbols.h"
#include "keycodes.h"
#include "language_state.h"
#include "symbols.h"

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
    /*
     * Восстанавливаем только физически удерживаемые модификаторы.
     * One-shot модификаторы намеренно не возвращаются: они считаются
     * использованными при отправке пользовательского символа. Их возврат
     * применил бы one-shot к следующему нажатию и изменил бы ожидаемое
     * поведение. При изменении этой семантики потребуется отдельная проверка
     * всех символов с OSM-модификаторами.
     */
    set_mods(state.mods);
}

bool process_custom_symbols(uint16_t keycode) {
    const mod_state_t mod_state = save_mod_state();
    const uint8_t effective_mods = get_effective_mods(mod_state);
    const bool shifted = (effective_mods & MOD_MASK_SHIFT) != 0;

    switch (keycode) {
        case AT_EN:
            send_en_symbol(send_at);
            return true;
        case HASH_EN:
            send_en_symbol(send_hash);
            return true;
        case DOLLAR_EN:
            send_en_symbol(send_dollar);
            return true;
        case AMP_EN:
            send_en_symbol(send_amp);
            return true;
        case CARET_EN:
            send_en_symbol(send_caret);
            return true;
        case VERBAR_EN:
            send_en_symbol(send_verbar);
            return true;
        case DOT_EN:
            send_en_symbol(send_dot);
            return true;
        case COMMA_EN:
            send_en_symbol(send_comma);
            return true;
        case ANGLE_LT_EN:
            send_en_symbol(send_less);
            return true;
        case ANGLE_GT_EN:
            send_en_symbol(send_greater);
            return true;

        case GRAVE_RUEN:
            clear_active_mods();
            if (shifted) {
                send_en_symbol(send_tilde);
            } else {
                send_en_symbol(send_grave);
            }
            restore_held_mods(mod_state);
            return true;

        case QUEST_RUEN:
            clear_active_mods();
            if (is_russian_layout_active()) {
                send_ru_quest();
            } else {
                send_quest();
            }
            restore_held_mods(mod_state);
            return true;

        case USER_QUOTES:
            clear_active_mods();
            if (shifted) {
                send_en_symbol(send_single_quote);
            } else if (is_russian_layout_active()) {
                send_ru_double_quote();
            } else {
                send_double_quote();
            }
            restore_held_mods(mod_state);
            return true;

        case SINGLE_QUOTE_EN:
            clear_active_mods();
            send_en_symbol(send_single_quote);
            restore_held_mods(mod_state);
            return true;

        case USER_COLON:
            clear_active_mods();
            if (shifted) {
                if (is_russian_layout_active()) {
                    send_ru_semicolon();
                } else {
                    send_semicolon();
                }
            } else if (is_russian_layout_active()) {
                send_ru_colon();
            } else {
                send_colon();
            }
            restore_held_mods(mod_state);
            return true;

        case DOT_COMMA_RUEN:
            clear_active_mods();
            if (shifted) {
                if (is_russian_layout_active()) {
                    send_ru_comma();
                } else {
                    send_comma();
                }
            } else if (is_russian_layout_active()) {
                send_ru_dot();
            } else {
                send_dot();
            }
            restore_held_mods(mod_state);
            return true;

        case SEMICOLON_EN:
            clear_active_mods();
            send_en_symbol(send_semicolon);
            restore_held_mods(mod_state);
            return true;

        case SLASH_BSLS_RUEN: {
            const bool shift_only = shifted &&
                                    (effective_mods & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI)) == 0;

            if (!is_russian_layout_active() && shift_only) {
                clear_active_mods();
                tap_code(KC_BSLS);
                restore_held_mods(mod_state);
            } else {
                tap_code(KC_SLSH);
            }
            return true;
        }

        case LSBRC_EN:
            clear_active_mods();
            send_en_symbol(send_left_square_bracket);
            restore_held_mods(mod_state);
            return true;
        case RSBRC_EN:
            clear_active_mods();
            send_en_symbol(send_right_square_bracket);
            restore_held_mods(mod_state);
            return true;
        case LCBRC_EN:
            clear_active_mods();
            send_en_symbol(send_left_curly_bracket);
            restore_held_mods(mod_state);
            return true;
        case RCBRC_EN:
            clear_active_mods();
            send_en_symbol(send_right_curly_bracket);
            restore_held_mods(mod_state);
            return true;

        case LBRC_RUEN:
            clear_active_mods();
            if (shifted) {
                send_en_symbol(send_left_square_bracket);
            } else {
                register_mods(MOD_LSFT);
                tap_code(KC_9);
                unregister_mods(MOD_LSFT);
            }
            restore_held_mods(mod_state);
            return true;

        case RBRC_RUEN:
            clear_active_mods();
            if (shifted) {
                send_en_symbol(send_right_square_bracket);
            } else {
                register_mods(MOD_LSFT);
                tap_code(KC_0);
                unregister_mods(MOD_LSFT);
            }
            restore_held_mods(mod_state);
            return true;

        case MINUS_EQUAL:
            if (shifted) {
                clear_active_mods();
                tap_code(KC_EQUAL);
                restore_held_mods(mod_state);
            } else {
                tap_code16(KC_MINUS);
            }
            return true;

        case ARROW_FN:
            clear_active_mods();
            send_en_symbol(send_fat_arrow);
            restore_held_mods(mod_state);
            return true;

        default:
            return false;
    }
}
