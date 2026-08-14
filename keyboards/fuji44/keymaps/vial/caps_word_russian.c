#include QMK_KEYBOARD_H
#include "language_state.h"

#ifdef CAPS_WORD_ENABLE
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Standard Latin letters and minus continue Caps Word with Shift.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;

        // Russian-only letter positions outside KC_A ... KC_Z.
        case KC_GRV:  // Ё
        case KC_LBRC: // Х
        case KC_RBRC: // Ъ
        case KC_SCLN: // Ж
        case KC_QUOT: // Э
        case KC_COMM: // Б
        case KC_DOT:  // Ю
            if (is_russian_layout_active()) {
                add_weak_mods(MOD_BIT(KC_LSFT));
                return true;
            }
            return false;

        // Keys that continue Caps Word without Shift.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false;
    }
}
#endif
