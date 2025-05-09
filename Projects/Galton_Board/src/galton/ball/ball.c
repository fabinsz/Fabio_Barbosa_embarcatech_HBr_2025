
#include "src/galton/ball/config.h"
#include "src/galton/ball/ball.h"


void mover_bola(Bola *bola, int direcao) {
    if (!bola->ativa) return;

    bola->y += bola->velocidade;
    bola->x += direcao;  
}

void ativar_bola_periodicamente(int *contador, Bola bolas[], int *bolas_ativas) {
    if (*contador % 10 == 0 && *bolas_ativas < MAX_BOLAS) {
        // Ativa uma nova bola
        for (int i = 0; i < MAX_BOLAS; i++) {
            if (!bolas[i].ativa) {
                bolas[i].ativa = 1;
                bolas[i].x = 64;  // Posição inicial 
                bolas[i].y = 0;   // Posição inicial
                bolas[i].velocidade = 2; // Exemplo de velocidade
                (*bolas_ativas)++;
                break;
            }
        }
    }
}
