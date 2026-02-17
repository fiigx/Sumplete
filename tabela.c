#include <stdio.h>
#include <time.h>

int main(){

    time_t inicioJogo, fimJogo;
    double tempoGasto;

    printf("Jogo iniciado!\n");
    inicioJogo = time(NULL);

    printf("Voce venceu!\n");
    fimJogo = time(NULL);

    tempoGasto = difftime(fimJogo, inicioJogo);

    printf("Tempo total: %.0f segundos\n", tempoGasto);

    return 0;
}