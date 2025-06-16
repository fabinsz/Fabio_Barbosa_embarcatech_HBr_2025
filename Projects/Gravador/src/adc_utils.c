#include <stdio.h>
#include <string.h>
#include <stdbool.h>  // para bool
#include "include/adc_utils.h"
#include "inc/ssd1306.h"  // para desenhar no OLED

#define BUFFER_SIZE 100

static float adc_buffer[BUFFER_SIZE];
static int buffer_index = 0;
static bool buffer_cheio_flag = false;

bool store_adc_value(float value, uint8_t *ssd) {
    if (buffer_cheio_flag) {
        // buffer cheio, não armazena mais valores
        return true;
    }

    adc_buffer[buffer_index++] = value;

    if (buffer_index >= BUFFER_SIZE) {
        // limpar tela
        memset(ssd, 0, ssd1306_buffer_length);

        ssd1306_draw_string(ssd, 0, 0, "Carregando...");

        buffer_cheio_flag = true;  // sinaliza buffer cheio
        buffer_index = 0;          // reseta índice se quiser (ou pode deixar como está para congelar)
    }
    return buffer_cheio_flag;
}

bool adc_buffer_cheio(void) {
    return buffer_cheio_flag;
}
