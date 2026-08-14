#include QMK_KEYBOARD_H
#include "language_state.h"

#define LANGUAGE_SWITCH_DELAY_MS 10
#define LANGUAGE_RESTORE_DELAY_MS 1

typedef enum {
    LANG_STATE_EN = 0,
    LANG_STATE_RU = 1,
} lang_state_t;

typedef struct {
    uint8_t mods;
    uint8_t weak_mods;
#ifndef NO_ACTION_ONESHOT
    uint8_t oneshot_mods;
#endif
} mod_state_t;

static lang_state_t current_lang = LANG_STATE_EN;

bool is_russian_layout_active(void) {
    return current_lang == LANG_STATE_RU;
}

static mod_state_t save_mod_state(void) {
    mod_state_t state = {
        .mods = get_mods(),
        .weak_mods = get_weak_mods(),
#ifndef NO_ACTION_ONESHOT
        .oneshot_mods = get_oneshot_mods(),
#endif
    };

    return state;
}

static void clear_active_mods(void) {
    clear_mods();
    clear_weak_mods();
#ifndef NO_ACTION_ONESHOT
    clear_oneshot_mods();
#endif
}

static void restore_mod_state(mod_state_t state) {
    set_mods(state.mods);
    set_weak_mods(state.weak_mods);
#ifndef NO_ACTION_ONESHOT
    set_oneshot_mods(state.oneshot_mods);
#endif
    send_keyboard_report();
}

static void restore_held_mods(mod_state_t state) {
    set_mods(state.mods);
    set_weak_mods(state.weak_mods);
    send_keyboard_report();
}

static void send_language_shortcut(uint16_t keycode) {
    /*
     * Раскладка ОС намеренно переключается именно Ctrl+1 / Ctrl+2.
     * Перед отправкой все модификаторы сняты: удерживаемый Shift иначе
     * превратил бы это сочетание в Ctrl+Shift+1 или Ctrl+Shift+2.
     */
    register_code(KC_LCTL);
    tap_code16(keycode);
    unregister_code(KC_LCTL);
}

static void switch_to_en_unmodified(void) {
    send_language_shortcut(KC_1);
    current_lang = LANG_STATE_EN;
}

static void switch_to_ru_unmodified(void) {
    send_language_shortcut(KC_2);
    current_lang = LANG_STATE_RU;
}

void switch_to_en(void) {
    const mod_state_t mod_state = save_mod_state();

    clear_active_mods();
    switch_to_en_unmodified();
    restore_mod_state(mod_state);
}

void switch_to_ru(void) {
    const mod_state_t mod_state = save_mod_state();

    clear_active_mods();
    switch_to_ru_unmodified();
    restore_mod_state(mod_state);
}

void switch_lang(void) {
    if (current_lang == LANG_STATE_EN) {
        switch_to_ru();
    } else {
        switch_to_en();
    }
}

void send_en_symbol(void (*send)(void)) {
    const mod_state_t mod_state = save_mod_state();
    const bool restore_russian = current_lang == LANG_STATE_RU;

    clear_active_mods();

    if (restore_russian) {
        switch_to_en_unmodified();
        wait_ms(LANGUAGE_SWITCH_DELAY_MS);
    }

    send();

    if (restore_russian) {
        wait_ms(LANGUAGE_RESTORE_DELAY_MS);
        switch_to_ru_unmodified();
    }

    /* One-shot модификаторы считаются использованными введённым символом. */
    restore_held_mods(mod_state);
}
