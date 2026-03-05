#include "symbols.h"

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

void send_semicolon(void) {
    tap_code(KC_SCLN);
}

void send_colon(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_SCLN);
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

void send_dot(void) {
    tap_code(KC_DOT);
}

void send_comma(void) {
    tap_code(KC_COMMA);
}

void send_square_left_bracket(void) {
    tap_code(KC_LBRC);
}

void send_square_right_bracket(void) {
    tap_code(KC_RBRC);
}

void send_curly_left_bracket(void) {
    register_mods(MOD_LSFT);
    tap_code(KC_LBRC);
    unregister_mods(MOD_LSFT);
}

void send_curly_right_bracket(void) {
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
