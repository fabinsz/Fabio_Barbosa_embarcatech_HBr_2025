#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h" // biblioteca do display SSD1306

#define I2C_SDA 14
#define I2C_SCL 15

// Inicializa a interface I2C nos pinos definidos
void init_i2c() {
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void draw_int_on_oled(uint8_t *ssd, int x, int y, int value) {
    char buffer[20];  
    sprintf(buffer, "%d", value);
    ssd1306_draw_string(ssd, x, y, buffer);
}

int main() {
    stdio_init_all();
    
    // Inicializa ADC e pinos
    adc_init();
    adc_gpio_init(26);  // Y = ADC0
    adc_gpio_init(27);  // X = ADC1

    // Inicializa I2C e Display
    init_i2c();
    ssd1306_init();  // Inicializa display
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = 127,
        .start_page = 0,
        .end_page = 7
    };
    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];;

    const uint bar_width = 40;
    const uint adc_max = (1 << 12) - 1;

    while (1) {
        // Lê eixo X (ADC1 = GPIO 27)
        adc_select_input(1);
        uint adc_x_raw = adc_read();

        // Lê eixo Y (ADC0 = GPIO 26)
        adc_select_input(0);
        uint adc_y_raw = adc_read();

        // Limpa o buffer do display
        memset(ssd, 0, sizeof(ssd));

        // Desenha a barra X
        ssd1306_draw_string(ssd, 0, 0, "X:");

        draw_int_on_oled(ssd, 80, 0, adc_x_raw);

        // Desenha a barra Y
        ssd1306_draw_string(ssd, 0, 16, "Y");

        draw_int_on_oled(ssd, 80, 16, adc_y_raw);


        // Mostra no display
        render_on_display(ssd, &frame_area);

        sleep_ms(50);
    }

    return 0;
}
