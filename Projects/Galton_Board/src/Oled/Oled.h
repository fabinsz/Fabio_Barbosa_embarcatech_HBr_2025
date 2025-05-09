#ifndef OLED_H
#define OLED_H


void I2C();
void draw_int_on_oled(uint8_t *ssd, int x, int y, int value);
void ssd1306_draw_pixel(uint8_t *buffer, int x, int y);

#endif
