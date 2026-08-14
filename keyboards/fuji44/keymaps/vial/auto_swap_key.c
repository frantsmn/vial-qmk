#include "auto_swap_key.h"

#ifdef CAPS_WORD_ENABLE
#    include "caps_word.h"
#endif

static uint8_t get_effective_mods(void) {
    uint8_t mods = get_mods();

#ifndef NO_ACTION_ONESHOT
    mods |= get_oneshot_mods();
#endif

    return mods;
}

static void tap_code16_with_mods(uint16_t keycode, uint8_t mods) {
    const uint8_t added_mods = mods & ~get_mods();

    register_mods(added_mods);
    tap_code16(keycode);
    unregister_mods(added_mods);
}

static void send_hold(auto_swap_key_t *key) {
    tap_code16_with_mods(key->hold, key->mods);
    key->hold_sent = true;
}

static auto_swap_key_t *find_key(uint16_t keycode, auto_swap_key_t *keys, uint8_t key_count) {
    for (uint8_t i = 0; i < key_count; ++i) {
        if (keys[i].trigger == keycode) {
            return &keys[i];
        }
    }

    return NULL;
}

bool process_auto_swap_keys(uint16_t keycode, keyrecord_t *record, bool delay_hold,
                            auto_swap_key_t *keys, uint8_t key_count) {
    auto_swap_key_t *key = find_key(keycode, keys, key_count);

    if (key == NULL) {
        return true;
    }

    if (record->event.pressed) {
        key->active = true;
        key->delayed = delay_hold;
        key->hold_sent = false;
        key->timer = timer_read();
        key->mods = get_effective_mods();
        key->caps_word_mods = 0;

#ifdef CAPS_WORD_ENABLE
        if (is_caps_word_on()) {
            key->mods |= MOD_BIT(KC_LSFT);
            key->caps_word_mods = MOD_BIT(KC_LSFT) & ~get_mods();
#    if CAPS_WORD_IDLE_TIMEOUT > 0
            caps_word_reset_idle_timer();
#    endif
        }
#endif

        if (!key->delayed) {
            register_mods(key->caps_word_mods);
            register_code16(key->tap);
        }
    } else if (key->active) {
        if (!key->delayed) {
            unregister_code16(key->tap);
            unregister_mods(key->caps_word_mods);
        } else if (!key->hold_sent) {
            if (timer_elapsed(key->timer) >= key->timeout) {
                send_hold(key);
            } else {
                tap_code16_with_mods(key->tap, key->mods);
            }
        }

        key->active = false;
    }

    return false;
}

void auto_swap_key_task(auto_swap_key_t *keys, uint8_t key_count) {
    for (uint8_t i = 0; i < key_count; ++i) {
        auto_swap_key_t *key = &keys[i];

        if (key->active && key->delayed && !key->hold_sent &&
            timer_elapsed(key->timer) >= key->timeout) {
            send_hold(key);
        }
    }
}
