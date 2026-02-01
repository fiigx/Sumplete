#include <stdio.h>
#include <stdlib.h>

void criaMatriz();

int main(){
    criaMatriz();
    return 0;
}

void criaMatriz(){
    int tamanho, **matriz;
    printf("Digite o tamanho da matriz: ");
    scanf("%d", &tamanho);
    matriz = malloc(tamanho * sizeof(int*));
    for(int i = 0; i < tamanho; i++){
        matriz[i] = malloc(tamanho * sizeof(int));
    }
    for(int i = 0; i < tamanho; i++){
        for(int j = 0; j < tamanho; j++){
            matriz[i][j] = rand() % 10;
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}
