#pragma once

#include <stdbool.h>

bool is_russian_layout_active(void);
void switch_to_en(void);
void switch_to_ru(void);
void switch_lang(void);
void send_en_symbol(void (*send)(void));
