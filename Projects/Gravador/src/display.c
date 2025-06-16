#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "include/display.h"

#define I2C_SDA 14
#define I2C_SCL 15

static struct render_area frame_area;
static uint8_t ssd_buffer[ssd1306_buffer_length];

void init_i2c() {
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}


void desenhar_onda(uint8_t *buffer_display, uint16_t adc_val, int coluna) {
    int y = (adc_val * 63) / 4095;
    int page = y / 8;
    int bit = y % 8;
    int index = page * 128 + coluna;

    if (index >= 0 && index < 1024) {
        buffer_display[index] |= (1 << bit);
    }
}
