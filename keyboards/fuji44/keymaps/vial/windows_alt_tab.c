#include "windows_alt_tab.h"

typedef struct {
    uint16_t activation_keycode;
    uint8_t target_layer;
    uint32_t timeout_ms;
    uint16_t initial_step_delay_ms;
    uint32_t activity_timer;
    uint16_t consumed_cancel_keycode;
    bool configured;
    bool active;
    bool cancel_key_consumed;
} windows_alt_tab_state_t;

static windows_alt_tab_state_t switcher;

static bool is_target_layer_selected(layer_state_t state) {
    // The activation key must work only on the currently selected layer,
    // not merely while the target layer exists below another active layer.
    return get_highest_layer(state | default_layer_state) == switcher.target_layer;
}

static void activate(void) {
    if (switcher.active) {
        return;
    }

    switcher.active = true;
    add_weak_mods(MOD_BIT_LALT);
    send_keyboard_report();

    // Opening Alt+Tab selects the next window. Shift+Tab returns selection to
    // the currently focused window while leaving the switcher open.
    tap_code(KC_TAB);
    wait_ms(switcher.initial_step_delay_ms);
    tap_code16(LSFT(KC_TAB));

    switcher.activity_timer = timer_read32();
}

static void deactivate(void) {
    if (!switcher.active) {
        return;
    }

    switcher.active = false;
    del_weak_mods(MOD_BIT_LALT);
    send_keyboard_report();
}

static void cancel(void) {
    // Escape must be reported while Alt is still held. This cancels the
    // Windows switcher instead of accepting the last highlighted window.
    add_weak_mods(MOD_BIT_LALT);
    tap_code(KC_ESCAPE);
    deactivate();
}

static uint16_t tap_keycode(uint16_t keycode) {
    if (IS_QK_MODS(keycode)) {
        return QK_MODS_GET_BASIC_KEYCODE(keycode);
    }

    if (IS_QK_MOD_TAP(keycode)) {
        return QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
    }

    if (IS_QK_LAYER_TAP(keycode)) {
        return QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
    }

    return keycode;
}

static bool is_navigation_key(uint16_t keycode) {
    switch (keycode) {
        case KC_UP:
        case KC_DOWN:
        case KC_LEFT:
        case KC_RIGHT:
        case KC_HOME:
        case KC_END:
        case KC_TAB:
        case KC_DEL:
            return true;
    }

    if (IS_QK_MODS(keycode)) {
        const uint8_t mods = QK_MODS_GET_MODS(keycode);

        return (
        (QK_MODS_GET_BASIC_KEYCODE(keycode) == KC_TAB && (mods == MOD_LSFT || mods == MOD_RSFT))
        || (QK_MODS_GET_BASIC_KEYCODE(keycode) == KC_LEFT && (mods == MOD_LCTL || mods == MOD_RCTL))
        || (QK_MODS_GET_BASIC_KEYCODE(keycode) == KC_RIGHT && (mods == MOD_LCTL || mods == MOD_RCTL))
        );
    }

    if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
        switch (tap_keycode(keycode)) {
            case KC_UP:
            case KC_DOWN:
            case KC_LEFT:
            case KC_RIGHT:
            case KC_HOME:
            case KC_END:
            case KC_TAB:
                return true;
        }
    }

    return false;
}

static bool is_cancel_key(uint16_t keycode) {
    return tap_keycode(keycode) == KC_ESCAPE;
}

static bool is_confirm_key(uint16_t keycode) {
    switch (tap_keycode(keycode)) {
        case KC_ENTER:
        case KC_KP_ENTER:
        case KC_SPACE:
            return true;
        default:
            return false;
    }
}

void windows_alt_tab_init(
    uint16_t activation_keycode,
    uint8_t target_layer,
    uint32_t timeout_ms,
    uint16_t initial_step_delay_ms) {
    deactivate();

    switcher.activation_keycode = activation_keycode;
    switcher.target_layer = target_layer;
    switcher.timeout_ms = timeout_ms;
    switcher.initial_step_delay_ms = initial_step_delay_ms;
    switcher.activity_timer = 0;
    switcher.consumed_cancel_keycode = KC_NO;
    switcher.cancel_key_consumed = false;
    switcher.configured = true;
}

bool windows_alt_tab_process_record(uint16_t keycode, keyrecord_t *record) {
    if (!switcher.configured) {
        return true;
    }

    // Consume the physical Escape release as well. The module already sent a
    // complete synthetic Escape tap while Alt was held during cancellation.
    if (switcher.cancel_key_consumed &&
        keycode == switcher.consumed_cancel_keycode) {
        if (!record->event.pressed) {
            switcher.cancel_key_consumed = false;
            switcher.consumed_cancel_keycode = KC_NO;
        }

        return false;
    }

    if (keycode == switcher.activation_keycode) {
        if (record->event.pressed && is_target_layer_selected(layer_state)) {
            // ALT_TAB_MODE behaves as a toggle. A second press cancels the
            // switcher with Escape instead of accepting the selected window.
            if (switcher.active) {
                cancel();
            } else {
                activate();
            }
        }

        return false;
    }

    if (!switcher.active || !record->event.pressed) {
        return true;
    }

    if (is_cancel_key(keycode)) {
        // Replace the physical Escape with a controlled Alt+Escape tap so
        // Windows cancels the switcher before virtual Alt is released.
        switcher.cancel_key_consumed = true;
        switcher.consumed_cancel_keycode = keycode;
        cancel();
        return false;
    }

    if (is_confirm_key(keycode)) {
        // Confirmation keys release Alt before their normal key action.
        deactivate();
    } else {
        // action_exec() clears all weak modifiers before every physical key
        // press. Restore Alt before QMK sends this key's USB report.
        add_weak_mods(MOD_BIT_LALT);

        if (is_navigation_key(keycode)) {
            switcher.activity_timer = timer_read32();
        }
    }

    return true;
}

layer_state_t windows_alt_tab_layer_state_set(layer_state_t state) {
    if (switcher.configured && switcher.active &&
        !is_target_layer_selected(state)) {
        deactivate();
    }

    return state;
}

void windows_alt_tab_task(void) {
    if (switcher.configured && switcher.active &&
        timer_elapsed32(switcher.activity_timer) >= switcher.timeout_ms) {
        cancel();
    }
}
