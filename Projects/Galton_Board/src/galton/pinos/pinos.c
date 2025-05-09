
#include <stdint.h>
#include "src/Oled/Oled.h"
#include "inc/ssd1306.h"
#include "src/galton/pinos/pinos.h"
#include "src/galton/ball/ball.h"

#define ESPACAMENTO_X 8
#define ESPACAMENTO_Y 6
#define LINHAS_PINOS 6

void desenhar_pinos(uint8_t *buffer) {
    int espacamento_x = 8;
    int espacamento_y = 6;
    int linhas_pinos = 6;

    for (int linha = 0; linha < linhas_pinos; linha++) {
        int num_pinos = linha + 1;
        int y = 10 + linha * espacamento_y;

        // Calcula ponto inicial para centralizar os pinos horizontalmente
        int largura_total = (num_pinos - 1) * espacamento_x;
        int x_inicio = 64 - largura_total / 2;

        for (int i = 0; i < num_pinos; i++) {
            int x = x_inicio + i * espacamento_x;
            if (x >= 0 && x < 128 && y < 64) {
                ssd1306_draw_pixel(buffer, x, y);
            }
        }
    }
}





