#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

void init_i2c();
void draw_float_on_oled(uint8_t *ssd, int x, int y, float value, int precision);
float adc_to_celsius(uint16_t adc_val);
float read_onboard_temperature();

void draw_float_on_oled(uint8_t *ssd, int x, int y, float value, int precision) {
    char buffer[20];
    sprintf(buffer, "%.*f", precision, value);
    ssd1306_draw_string(ssd, x, y, buffer);
}

float adc_to_celsius(uint16_t adc_val) {
    float voltage = adc_val * 3.3f / 4095.0f; // Conversão para tensão (12 bits, 3.3V ref)
    return 27.0f - (voltage - 0.706f) / 0.001721f; // Fórmula do datasheet
}

float read_onboard_temperature() {
    const int num_samples = 100;
    uint32_t raw = 0;

    for (int i = 0; i < num_samples; i++) {
        raw += adc_read();
        sleep_ms(10);
    }
    raw /= num_samples;

    return adc_to_celsius((uint16_t)raw);
}

void init_i2c() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

int main() {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); // Canal do sensor interno

    init_i2c();
    ssd1306_init();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = 127,
        .start_page = 0,
        .end_page = 7
    };
    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];

    while (1) {
        float temp_c = read_onboard_temperature();

        memset(ssd, 0, sizeof(ssd));
        ssd1306_draw_string(ssd, 0, 0, "Temp");
        draw_float_on_oled(ssd, 80, 0, temp_c, 2);
        render_on_display(ssd, &frame_area);

        printf("Temperatura interna: %.2f °C\n", temp_c);
        sleep_ms(1000);
    }

    return 0;
}
