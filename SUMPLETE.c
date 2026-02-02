#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

void limpar_buffer(); //Limpa o caralho do buffer
void criaMatriz(); //Função responsável por criar uma matriz e imprimir na tela

int main(){
    char option[MAX];
    printf("Selecione a opção de jogo:\nnovo. ");
    scanf("%s", option);
    limpar_buffer();
    if(strcmp(option, "novo") == 0){
        criaMatriz();
    }
    else{
        printf("Selecione uma opção válida. ");
    }
    return 0;
}

void criaMatriz(){
    char dificuldade, nome[MAX]; int **matriz;
    printf("Você iniciu um novo jogo, digite seu nome para continuar: ");
    fgets(nome, MAX, stdin);
    printf("Selecione a dificuldade do jogo:\nf: nível fácil, tamanho 3x3.\nm: nível médio, tamanho 5x5\nd: nível difícil, tamanho 7x7.\n");
    scanf("%c", &dificuldade);
    if(dificuldade == 'f'){
        matriz = malloc(3 * sizeof(int*));
        for(int i = 0; i < 3; i++){
            matriz[i] = malloc(3 * sizeof(int));
        }
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                matriz[i][j] = rand() % 10;
                printf("%d ", matriz[i][j]);
            }
            printf("\n");
        }
    }
    else{
        printf("Digite uma dificuldade válida.");
    }
    return 0;
}

void limpar_buffer(){
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}