#pragma once

#include <stdint.h>

void temporary_layer_activate(uint8_t layer, uint16_t timeout_ms);
void temporary_layer_refresh(void);
void temporary_layer_task(void);
