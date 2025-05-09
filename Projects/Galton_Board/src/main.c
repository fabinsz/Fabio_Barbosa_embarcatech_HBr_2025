#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "src/galton/random/random_utils.h"
#include "src/Oled/Oled.h"
#include "inc/ssd1306.h"
#include "src/galton/ball/ball.h"
#include "src/galton/ball/config.h"
#include "src/galton/pinos/pinos.h"

#define ESPACAMENTO_Y 6
#define LINHAS_PINOS 6
#define NUM_CANALETAS 7
#define LARGURA_CANALETA (64 / NUM_CANALETAS)

int main() {
    stdio_init_all();
    init_random();
    I2C();
    ssd1306_init();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = 127,
        .start_page = 0,
        .end_page = 7
    };

    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];
    
    Bola bolas[MAX_BOLAS] = {0};
    int bolas_ativas = 0;
    int contador = 0;
    int contagem_espacos[NUM_CANALETAS] = {0};
    int preenchimento[NUM_CANALETAS] = {0};
    int total_bolas_soltadas = 0;  // << contador de bolas soltas

    while (true) {
        memset(ssd, 0, sizeof(ssd));

        desenhar_pinos(ssd);

        for (int i = 1; i < NUM_CANALETAS; i++) {
            int x = 32 + i * LARGURA_CANALETA;
            for (int y = 56; y < 64; y++) {
                ssd1306_draw_pixel(ssd, x, y);
            }
        }

        // Verifica se uma nova bola foi ativada
        int bolas_antes = bolas_ativas;

        ativar_bola_periodicamente(&contador, bolas, &bolas_ativas);
        if (bolas_ativas > bolas_antes) {
            total_bolas_soltadas++;
        }

        // Mostra total de bolas soltas (à direita)
        draw_int_on_oled(ssd, 0, 0, total_bolas_soltadas);

        for (int i = 0; i < bolas_ativas; i++) {
            if (!bolas[i].ativa) continue;

            if ((bolas[i].y - 10) % ESPACAMENTO_Y == 0 && bolas[i].y >= 10 && bolas[i].y <= 10 + (LINHAS_PINOS - 1) * ESPACAMENTO_Y) {
                int direcao = random_direction();
                bolas[i].x += (direcao == 0) ? -5 : 5;
            }

            bolas[i].y += bolas[i].velocidade;

            if (bolas[i].x < 0) bolas[i].x = 0;
            if (bolas[i].x > 127) bolas[i].x = 127;
            if (bolas[i].y > 63) bolas[i].ativa = 0;

            if (bolas[i].y >= 63) {
                bolas[i].ativa = 0;

                for (int espaco = 0; espaco < NUM_CANALETAS; espaco++) {
                    int espaco_inicio = 32 + espaco * LARGURA_CANALETA;
                    int espaco_fim = espaco_inicio + LARGURA_CANALETA;

                    if (bolas[i].x >= espaco_inicio && bolas[i].x < espaco_fim) {
                        contagem_espacos[espaco]++;
                        preenchimento[espaco] = contagem_espacos[espaco];
                        draw_int_on_oled(ssd, 0, 0, espaco);
                        printf("Bola caiu no espaço entre as canaletas %d\n", espaco);
                    }
                }
            }

            if (bolas[i].ativa)
                ssd1306_draw_pixel(ssd, bolas[i].x, bolas[i].y);
        }

        for (int espaco = 0; espaco < NUM_CANALETAS; espaco++) {
            for (int y = 63; y > 63 - preenchimento[espaco]; y--) {
                int espaco_inicio = 32 + espaco * LARGURA_CANALETA;
                for (int x = espaco_inicio; x < espaco_inicio + LARGURA_CANALETA; x++) {
                    ssd1306_draw_pixel(ssd, x, y);
                }
            }
        }

        render_on_display(ssd, &frame_area);
        sleep_ms(100);
        contador++;
    }

    return 0;
}
