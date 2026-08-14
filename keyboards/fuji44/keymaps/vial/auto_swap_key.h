#pragma once

#include QMK_KEYBOARD_H

#ifndef AUTO_SWAP_KEY_TIMEOUT
#    define AUTO_SWAP_KEY_TIMEOUT 300
#endif

typedef struct {
    uint16_t trigger;
    uint16_t tap;
    uint16_t hold;
    uint16_t timeout;

    uint16_t timer;
    uint8_t mods;
    uint8_t caps_word_mods;
    bool active;
    bool delayed;
    bool hold_sent;
} auto_swap_key_t;

#define AUTO_SWAP_KEY_WITH_TIMEOUT(_trigger, _tap, _hold, _timeout) \
    { .trigger = (_trigger), .tap = (_tap), .hold = (_hold), .timeout = (_timeout) }

#define AUTO_SWAP_KEY(_trigger, _tap, _hold) \
    AUTO_SWAP_KEY_WITH_TIMEOUT(_trigger, _tap, _hold, AUTO_SWAP_KEY_TIMEOUT)

bool process_auto_swap_keys(uint16_t keycode, keyrecord_t *record, bool delay_hold,
                            auto_swap_key_t *keys, uint8_t key_count);
void auto_swap_key_task(auto_swap_key_t *keys, uint8_t key_count);
