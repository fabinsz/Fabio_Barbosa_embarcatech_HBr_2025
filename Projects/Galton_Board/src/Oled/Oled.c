#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "src/Oled/Oled.h"

#define I2C_SDA 14
#define I2C_SCL 15

void draw_int_on_oled(uint8_t *ssd, int x, int y, int value) {
    char buffer[20];  
    sprintf(buffer, "%d", value);
    ssd1306_draw_string(ssd, x, y, buffer);
}

void I2C() {
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void ssd1306_draw_pixel(uint8_t *buffer, int x, int y) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;

    int page = y / 8;
    int bit = y % 8;
    int index = page * 128 + x;

    buffer[index] |= (1 << bit);
}