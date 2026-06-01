#include "symbols.h"

void send_exclamation(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_1);
    unregister_mods(MOD_LSFT);
}

void send_at(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_2);
    unregister_mods(MOD_LSFT);
}

void send_hash(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_3);
    unregister_mods(MOD_LSFT);
}

void send_dollar(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_4);
    unregister_mods(MOD_LSFT);
}

void send_caret(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_6);
    unregister_mods(MOD_LSFT);
}

void send_amp(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_7);
    unregister_mods(MOD_LSFT);
}

void send_verbar(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_NUBS);
    unregister_mods(MOD_LSFT);
}

void send_quest(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_SLSH);
    unregister_mods(MOD_LSFT);
}

void send_ru_quest(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_7);
    unregister_mods(MOD_LSFT);
}

void send_semicolon(void) {
    tap_code(KC_SCLN);
}

void send_ru_semicolon(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_4);
    unregister_mods(MOD_LSFT);
}

void send_colon(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_SCLN);
    unregister_mods(MOD_LSFT);
}
void send_ru_colon(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_6);
    unregister_mods(MOD_LSFT);
}

void send_single_quote(void) {
    tap_code(KC_QUOT);
}

void send_double_quote(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_QUOT);
    unregister_mods(MOD_LSFT);
}

void send_ru_double_quote(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_2);
    unregister_mods(MOD_LSFT);
}

void send_dot(void) {
    tap_code(KC_DOT);
}

void send_comma(void) {
    tap_code(KC_COMMA);
}

void send_less(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_COMMA);
    unregister_mods(MOD_LSFT);
}

void send_greater(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_DOT);
    unregister_mods(MOD_LSFT);
}

void send_left_square_bracket(void) {
    tap_code(KC_LBRC);
}

void send_right_square_bracket(void) {
    tap_code(KC_RBRC);
}

void send_left_curly_bracket(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_LBRC);
    unregister_mods(MOD_LSFT);
}

void send_right_curly_bracket(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_RBRC);
    unregister_mods(MOD_LSFT);
}

// Одинаково на RU и EN
void send_asterisk(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_8);
    unregister_mods(MOD_LSFT);
}

// Одинаково на RU и EN
void send_percent(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_5);
    unregister_mods(MOD_LSFT);
}

void send_fat_arrow(void) {
    tap_code(KC_EQL);
    send_greater();
}
