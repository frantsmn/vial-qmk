#include QMK_KEYBOARD_H
// #include "eeconfig.h"

// typedef enum {
//     OS_WINDOWS = 0,
//     OS_LINUX   = 1,
// } os_mode_t;

// typedef union {
//     uint32_t raw;
//     struct {
//         uint8_t os_mode;
//     };
// } user_config_t;

// static user_config_t user_config;

// void keyboard_post_init_user(void) {
//     user_config.raw = eeconfig_read_user();

//     if (user_config.os_mode > OS_LINUX) {
//         user_config.os_mode = OS_WINDOWS;
//     }
// }

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
    ROUND_BR_MOD,
    SQUARE_BR_MOD,
    CURLY_BR_MOD,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {

            case ROUND_BR_MOD: {
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

            case SQUARE_BR_MOD: {
                uint8_t mods = get_mods();
                clear_mods();

                tap_code16(KC_LBRC);
                tap_code16(KC_RBRC);
                wait_ms(20);

                tap_code16(KC_LEFT);

                set_mods(mods);
                return false;
            }

            case CURLY_BR_MOD: {
                uint8_t mods = get_mods();
                clear_mods();

                register_mods(MOD_LSFT);
                tap_code16(KC_LBRC);
                tap_code16(KC_RBRC);
                unregister_mods(MOD_LSFT);
                wait_ms(20);

                tap_code16(KC_LEFT);

                set_mods(mods);
                return false;
            }

            case SWITCH_LANG: {
                register_code(KC_LGUI);
                tap_code(KC_SPC);
                unregister_code(KC_LGUI);
                return false;
            }

            case LANG_EN: {
                register_code(KC_LCTL);
                tap_code(KC_1);
                unregister_code(KC_LCTL);
                // tap_code(KC_F21);
                return false;
            }

            case LANG_RU: {
                register_code(KC_LCTL);
                tap_code(KC_2);
                unregister_code(KC_LCTL);
                // tap_code(KC_F22);
                return false;
            }
        }
    }

    return true;
}
