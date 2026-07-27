#pragma once

#include QMK_KEYBOARD_H

/**
 * Configures the Windows Alt+Tab switcher.
 *
 * The activation keycode is effective only while target_layer is active.
 * timeout_ms is the maximum idle time after activation or the last navigation
 * key press. initial_step_delay_ms is the pause between the initial Tab and
 * Shift+Tab that returns selection to the currently focused window.
 */
void windows_alt_tab_init(
    uint16_t activation_keycode,
    uint8_t target_layer,
    uint32_t timeout_ms,
    uint16_t initial_step_delay_ms
);

/**
 * Handles the activation key, navigation activity, and keys that close the
 * switcher. Returns false when the activation keycode or a handled Escape
 * event has been consumed.
 */
bool windows_alt_tab_process_record(uint16_t keycode, keyrecord_t *record);

/**
 * Releases virtual Alt when target_layer is no longer active.
 */
layer_state_t windows_alt_tab_layer_state_set(layer_state_t state);

/**
 * Cancels the Windows switcher with Escape and releases virtual Alt when the
 * configured idle timeout expires. Call this from housekeeping_task_user() so
 * key events from the current matrix scan are processed before the timeout.
 */
void windows_alt_tab_task(void);
