#ifndef BALL_H
#define BALL_H

typedef struct {
    int x;
    int y;
    int velocidade;
    int ativa; // 1 se ainda está em movimento, 0 se parou
} Bola;

void ativar_bola_periodicamente(int *contador, Bola bolas[], int *bolas_ativas); 

#endif
