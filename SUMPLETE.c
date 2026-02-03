#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

void limpar_buffer(); // Limpa o caralho do buffer
void criaMatriz(); // Função responsável por criar uma matriz e imprimir na tela

int main(){ // Main, link menu/funções
    char option[MAX];
    printf("Bem vindo ao Jogo SUMPLETE:\n\nComandos do jogo:\n(sair)\n(novo)\n");
    do{
        scanf("%s", option);
        limpar_buffer();
        if(strcmp(option, "novo") == 0){
            criaMatriz();
        }
        else{
            printf("Selecione uma opção válida: ");
        }
    } while(strcmp(option, "sair") != 0);

    return 0;
}

void criaMatriz(){
    srand(1022);
    char dificuldade, nome[MAX]; int **matriz, TAM;
    printf("Você iniciu um novo jogo, digite seu nome para continuar: ");
    fgets(nome, MAX, stdin);
    printf("Selecione a dificuldade do jogo:\nf: nível fácil, tamanho 3x3.\nm: nível médio, tamanho 5x5\nd: nível difícil, tamanho 7x7.\n");
    scanf("%c", &dificuldade);
    if(dificuldade == 'f'){
        TAM = 3;
    }
    else if(dificuldade == 'm'){
        TAM = 5;
    }
    else if(dificuldade == 'd'){
        TAM = 7;
    }
    else{
        printf("DIGITE UMA DIFICULDADE VÁLIDA");
    }

    matriz = malloc((TAM+1) * sizeof(int*));
    for(int i = 0; i < TAM+1; i++){ 
        matriz[i] = malloc((TAM+1) * sizeof(int));
    }
    
    for(int i = 0; i < TAM+1; i++){ //Zera a última linha da matriz (soma).
        matriz[TAM][i] = 0;
    }
    for(int i = 0; i < TAM+1; i++){ //Zera a última coluna da matriz (soma).
        matriz[i][TAM] = 0;
    }
    
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            matriz[i][j] = rand() % 10;
            if((rand() % 2) == 0){ 
                matriz[i][TAM] = matriz[i][TAM] + matriz[i][j];
                matriz[TAM][j] = matriz[TAM][j] + matriz[i][j];
            }
        }
    }


    for(int i = 0; i < TAM+1; i++){ // aqui apenas imprime a matriz teste
        for(int j = 0; j < TAM+1; j++){ 
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
        
    return 0;
}

void limpar_buffer(){
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}