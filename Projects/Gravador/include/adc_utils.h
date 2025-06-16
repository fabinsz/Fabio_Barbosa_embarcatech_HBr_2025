#ifndef ADC_UTILS_H
#define ADC_UTILS_H

#include <stdint.h>

bool store_adc_value(float value, uint8_t *ssd);
bool adc_buffer_cheio(void);

#endif
