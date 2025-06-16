 #include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "include/adc_utils.h"
#include "include/display.h"
#include "include/gpio.h"

#define I2C_SDA 14
#define I2C_SCL 15
#define BUZZER_PIN 21

#define BUTTON_A 5
#define BUTTON_B 6

#define LED_VERMELHO 13
#define LED_VERDE 11

#define SAMPLE_RATE 3000
#define DURATION_SECONDS 40
#define NUM_SAMPLES (SAMPLE_RATE * DURATION_SECONDS)

uint16_t adc_buffer[NUM_SAMPLES];
volatile int adc_buffer_index = 0;
bool gravado = false;


void init_pwm_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice_num, false);
}

void set_pwm_frequency_duty(uint freq_hz, uint8_t duty_percent) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    const uint32_t clk_freq = 125000000;

    if (freq_hz < 50) freq_hz = 50;
    if (freq_hz > 20000) freq_hz = 20000;

    uint32_t wrap = clk_freq / freq_hz - 1;
    pwm_set_wrap(slice_num, wrap);
    pwm_set_gpio_level(BUZZER_PIN, wrap * duty_percent / 100);
    pwm_set_enabled(slice_num, true);
}

void play_buffer_as_sound(uint16_t *buffer, int length) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint32_t clk_freq = 125000000;
    uint freq_hz = 40000;  // PWM em 40kHz

    uint32_t wrap = clk_freq / freq_hz - 1;
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true);

    for (int i = 0; i < length; i++) {
        uint16_t val = buffer[i];
        uint level = (val * wrap) / 4095;  // duty proporcional ao sinal captado
        pwm_set_gpio_level(BUZZER_PIN, level);
        sleep_us(1000000 / SAMPLE_RATE);
    }

    pwm_set_enabled(slice_num, false);
}

void aguardar_botao(int gpio) {
    while (gpio_get(gpio)) sleep_ms(10);
    while (!gpio_get(gpio)) sleep_ms(10);
}


int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(28);  // ADC2

    init_i2c();
    ssd1306_init();
    init_pwm_buzzer();

    init_gpio();


    struct render_area frame_area = {
        .start_column = 0,
        .end_column = 127,
        .start_page = 0,
        .end_page = 7
    };
    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];

    while (1) {
        memset(ssd, 0, ssd1306_buffer_length);
        ssd1306_draw_string(ssd, 0, 0, "A=Gravar");
        ssd1306_draw_string(ssd, 0, 30,"B=Tocar");
        render_on_display(ssd, &frame_area);

        if (!gpio_get(BUTTON_A)) {
            aguardar_botao(BUTTON_A);
            adc_buffer_index = 0;
            gpio_put(LED_VERMELHO, 1);

            memset(ssd, 0, ssd1306_buffer_length);
            ssd1306_draw_string(ssd, 0, 0, "Gravando...");
            render_on_display(ssd, &frame_area);

            int coluna = 0;
            uint64_t start_time = to_ms_since_boot(get_absolute_time());
            memset(ssd, 0, ssd1306_buffer_length);

            while (adc_buffer_index < NUM_SAMPLES) {
                adc_select_input(2);
                uint16_t adc_raw = adc_read();
                adc_buffer[adc_buffer_index++] = adc_raw;

                int coluna = adc_buffer_index % 800;  // Rola coluna de 0 a 127
                desenhar_onda(ssd, adc_raw, coluna);

                // Atualiza OLED a cada 128 amostras ou no final
                if (coluna == 127 || adc_buffer_index == NUM_SAMPLES) {
                    render_on_display(ssd, &frame_area);
                    memset(ssd, 0, ssd1306_buffer_length);
                }
                sleep_us(1000000 / SAMPLE_RATE);
            }

            gpio_put(LED_VERMELHO, 0);
            uint64_t end_time = to_ms_since_boot(get_absolute_time());
            float elapsed_sec = (end_time - start_time) / 1000.0f;

            memset(ssd, 0, ssd1306_buffer_length);
            ssd1306_draw_string(ssd, 0, 0, "Gravacao completa!");
            char msg[30];
            sprintf(msg, "Duracao: %.2fs", elapsed_sec);
            ssd1306_draw_string(ssd, 0, 16, msg);
            render_on_display(ssd, &frame_area);

            gravado = true;
            sleep_ms(1500);
        }

        if (!gpio_get(BUTTON_B)) {
            aguardar_botao(BUTTON_B);
            if (!gravado) {
                memset(ssd, 0, ssd1306_buffer_length);
                ssd1306_draw_string(ssd, 0, 0, "Grave primeiro!");
                render_on_display(ssd, &frame_area);
                sleep_ms(2000);
                continue;
            }

            gpio_put(LED_VERDE, 1);
            memset(ssd, 0, ssd1306_buffer_length);
            ssd1306_draw_string(ssd, 0, 0, "Reproduzindo...");
            render_on_display(ssd, &frame_area);

            play_buffer_as_sound(adc_buffer, NUM_SAMPLES);

            memset(ssd, 0, ssd1306_buffer_length);
            ssd1306_draw_string(ssd, 0, 0, "Fim da Reproducao.");
            render_on_display(ssd, &frame_area);
            sleep_ms(2000);
        }
        gpio_put(LED_VERDE, 0);
        sleep_ms(50);
    }

    return 0;
}