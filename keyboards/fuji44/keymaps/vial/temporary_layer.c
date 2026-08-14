#include QMK_KEYBOARD_H
#include "temporary_layer.h"

static uint8_t temporary_layer = 0;
static uint16_t temporary_layer_timer = 0;
static uint16_t temporary_layer_timeout_ms = 0;
static bool temporary_layer_active = false;

void temporary_layer_activate(uint8_t layer, uint16_t timeout_ms) {
    temporary_layer = layer;
    temporary_layer_timeout_ms = timeout_ms;
    temporary_layer_timer = timer_read();
    temporary_layer_active = true;
    layer_on(temporary_layer);
}

void temporary_layer_refresh(void) {
    if (temporary_layer_active && layer_state_is(temporary_layer)) {
        temporary_layer_timer = timer_read();
    }
}

void temporary_layer_task(void) {
    if (!temporary_layer_active) {
        return;
    }

    if (!layer_state_is(temporary_layer)) {
        temporary_layer_active = false;
        return;
    }

    if (timer_elapsed(temporary_layer_timer) >= temporary_layer_timeout_ms) {
        // Отключаем только временный слой, не затрагивая остальные активные слои.
        layer_off(temporary_layer);
        temporary_layer_active = false;
    }
}
