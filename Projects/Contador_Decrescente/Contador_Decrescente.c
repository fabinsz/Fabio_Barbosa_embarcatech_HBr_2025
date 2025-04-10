#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

#define BUTTON_A 5
#define BUTTON_B 6

#define I2C_SDA 14
#define I2C_SCL 15

void botoes();
void I2C();
void draw_int_on_oled(uint8_t *ssd, int x, int y, int value);

void draw_int_on_oled(uint8_t *ssd, int x, int y, int value) {
    char buffer[20];  
    sprintf(buffer, "%d", value);
    ssd1306_draw_string(ssd, x, y, buffer);
}

void botoes() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

void I2C() {
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

int main() {
    stdio_init_all();
    botoes();
    I2C();

    ssd1306_init();  // Inicializa display
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = 127,
        .start_page = 0,
        .end_page = 7
    };
    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];

    int valor = 9;
    int b = 0;
    bool contando = true;

    while (true) {
        // Verifica se botão A foi pressionado para resetar contagem
        if (gpio_get(BUTTON_A) == 0) {
            sleep_ms(50);  // debounce
            if (gpio_get(BUTTON_A) == 0) {
                valor = 9;
                b = 0;
                contando = true;
                while (gpio_get(BUTTON_A) == 0);  // espera soltar
            }
        }

        // Durante a contagem regressiva
        if (contando && valor >= 0) {
            memset(ssd, 0, sizeof(ssd));
            ssd1306_draw_string(ssd, 0, 0, "Contador");
            draw_int_on_oled(ssd, 80, 0, valor);
            ssd1306_draw_string(ssd, 0, 40, "Cliques");
            draw_int_on_oled(ssd, 80, 40, b);
            render_on_display(ssd, &frame_area);

            int tempo_ms = 1000;
            int check_interval = 50;
            int checks = tempo_ms / check_interval;

            for (int i = 0; i < checks; i++) {
                if (gpio_get(BUTTON_B) == 0) {
                    sleep_ms(50);  // debounce
                    if (gpio_get(BUTTON_B) == 0) {
                        b++;
                        while (gpio_get(BUTTON_B) == 0);  // espera soltar
                    }
                }
                sleep_ms(check_interval);
            }

            valor--;

            if (valor < 0) {
                contando = false;  // chegou ao fim
            }

        } else {
            // Mostra tela travada em 0 com total de cliques B
            memset(ssd, 0, sizeof(ssd));
            ssd1306_draw_string(ssd, 0, 0, "Pausado");
            draw_int_on_oled(ssd, 80, 0, 0);
            ssd1306_draw_string(ssd, 0, 40, "Cliques");
            draw_int_on_oled(ssd, 80, 40, b);
            render_on_display(ssd, &frame_area);
            sleep_ms(1000);
        }
    }

    return 0;
}
