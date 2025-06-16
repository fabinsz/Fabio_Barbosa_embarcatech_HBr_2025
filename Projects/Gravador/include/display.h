#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void init_i2c();
void desenhar_onda(uint8_t *buffer_display, uint16_t adc_val, int coluna);

#endif
