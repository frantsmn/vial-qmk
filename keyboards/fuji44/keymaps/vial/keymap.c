#include QMK_KEYBOARD_H
#include "auto_swap_key.h"
#include "custom_symbols.h"
#include "keycodes.h"
#include "language_state.h"
#include "temporary_layer.h"
#include "windows_alt_tab.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {};

#define WINDOWS_ALT_TAB_LAYER 6
#define WINDOWS_ALT_TAB_TIMEOUT_MS 3000
#define WINDOWS_ALT_TAB_INITIAL_STEP_DELAY_MS 40

// Слой с цифрами (#5)
#define TEMPORARY_LAYER_5 5
// Таймаут для циферного слоя (#5)
#define TEMPORARY_LAYER_5_TIMEOUT_MS 2000

// QMK callback. Нужен для быстрого срабатывания правого Shift.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SFT_T(KC_QUOTE):
        case RSFT_T(KC_QUOTE):
            return true;
        default:
            return false;
    }
}

static auto_swap_key_t auto_swap_keys[] = {
    AUTO_SWAP_KEY_WITH_TIMEOUT(AUTO_SWAP_M_RUEN, KC_M, KC_RBRC, 200),
     // мб делать свап без кастомных клавиш на базе обычных (?)
     // (риск без прошивки клавиатуры не вернуть оригинальное поведение клавиши ч-з vial)
};
#define AUTO_SWAP_KEYS_COUNT ARRAY_SIZE(auto_swap_keys)

void keyboard_post_init_user(void) {
    windows_alt_tab_init(
        ALT_TAB_MODE,
        WINDOWS_ALT_TAB_LAYER,
        WINDOWS_ALT_TAB_TIMEOUT_MS,
        WINDOWS_ALT_TAB_INITIAL_STEP_DELAY_MS
    );
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!windows_alt_tab_process_record(keycode, record)) {
        return false;
    }

    if (!process_auto_swap_keys(keycode, record, is_russian_layout_active(), auto_swap_keys, AUTO_SWAP_KEYS_COUNT)) {
        return false;
    }

    if (!record->event.pressed) {
        return true;
    }

    temporary_layer_refresh();

    switch (keycode) {
        case SWITCH_LANG:
            switch_lang();
            return false;
        case LANG_EN:
            switch_to_en();
            return false;
        case LANG_RU:
            switch_to_ru();
            return false;
        case LAYER_5_TIMEOUT:
            temporary_layer_activate(TEMPORARY_LAYER_5, TEMPORARY_LAYER_5_TIMEOUT_MS);
            return false;
    }

    return !process_custom_symbols(keycode);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return windows_alt_tab_layer_state_set(state);
}

void housekeeping_task_user(void) {
    // Housekeeping выполняется после обработки событий текущего цикла матрицы.
    windows_alt_tab_task();
}

void matrix_scan_user(void) {
    auto_swap_key_task(auto_swap_keys, AUTO_SWAP_KEYS_COUNT);
    temporary_layer_task();
}
