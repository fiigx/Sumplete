#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 100
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_RESET            "\x1b[0m"
#define RED(string)        ANSI_COLOR_RED        string ANSI_RESET
#define GREEN(string)      ANSI_COLOR_GREEN      string ANSI_RESET
typedef struct{
    int valor;
    int estadoP;
    int estadoU;
} Numero;

void limpar_buffer(); // Limpa o caralho do buffer
void novoJogo(); 
void criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD); // Função responsável por criar uma matriz e imprimir na tela
void adicionar(Numero **matriz, int l, int c);
void remover(Numero **matriz, int l, int c);
void dica(Numero **matriz, int *TAM); // Remove da matriz um número não pertecente a soma.
void resolver(Numero **matriz, int *TAM); // Resolve o jogo

int main(){ // Main, link menu/funções
    char option[MAX];
    printf("Bem vindo ao Jogo SUMPLETE:\n\nComandos do jogo:\n(sair)\n(novo)\n");
    do{
        scanf("%s", option);
        limpar_buffer();
        if(strcmp(option, "novo") == 0){
            novoJogo();
        }
        else{
            printf("Selecione uma opção válida: ");
        }
    } while(strcmp(option, "sair") != 0);

    return 0;
}   

void novoJogo(){
    Numero **matriz; int TAM, estadoPL = 0, estadoPD = 0; char opcao[MAX];

    matriz = malloc((3+1) * sizeof(Numero*)); // Aloca um espaço de memória para a matriz        
    for(int i = 0; i < 3+1; i++){ 
        matriz[i] = malloc((3+1) * sizeof(Numero));
    }

    criaMatriz(matriz, &TAM, &estadoPL, &estadoPD);
    do{
        int estadoUL = 0, estadoUD = 0; // Zera o contador do estado usuário ligado e do estado usuário desligado toda vez.
        for(int i = 0; i < TAM+1; i++){ // Impressão da matriz na tela.
            for(int j = 0; j < TAM+1; j++){ 
                if(matriz[i][j].estadoU == 1){
                    printf(GREEN("%d "), matriz[i][j].valor); // Impressão do valor em cor verde (estado usuário ligado).
                    if(matriz[i][j].estadoU == matriz[i][j].estadoP){ 
                        estadoUL++; // Acrescenta mais um no contador do estado usário ligado.
                    }
                }
                else if(matriz[i][j].estadoU == -1){
                    printf(RED("%d "), matriz[i][j].valor); // Impressão do valor em cor vermelha (estado usuário desligado).
                    if(matriz[i][j].estadoU == matriz[i][j].estadoP){
                        estadoUD++; // Acrescenta mais um no contador do estado usário desligado.
                    }
                }
                else
                    printf("%d ", matriz[i][j].valor); // Impressão do valor em cor normal.
            } 
            printf("\n");
        }

        if(estadoUD == estadoPD){
            printf("Parabéns seu fudido, você ganhou!\n");
            return 0;
        }
        
        //printf("Somatório do estadoPL:%d Somatório do estado PD:%d\nSomatório do estadoUL:%d Somatório do estadoUD:%d\n\n", estadoPL, estadoPD, estadoUL, estadoUD); //apenas para saber quantos números estão ligados e quantos estão desligados. tirar depois

        printf("\n\n");
        for(int i = 0; i < TAM+1; i++){ //estadosP do programa, tirar depois
            for(int j = 0; j < TAM+1; j++){
                printf("%d ", matriz[i][j].estadoP);
            }  
            printf("\n");
        }

        printf("O que você quer fazer: ");
        scanf("%s", opcao);
        
        if(strcmp(opcao, "adicionar") == 0){ // Talvez mudar isso depois.
            int l, c;
            scanf("%d%d", &l, &c);
            adicionar(matriz, l, c);
        }
        else if(strcmp(opcao, "remover") == 0){ // Talvez mudar isso depois.
            int l, c;
            scanf("%d%d", &l, &c);
            remover(matriz, l, c);
        }
        else if(strcmp(opcao, "dica") == 0){
            dica(matriz, &TAM);
        }
        else if(strcmp(opcao, "resolver") == 0){
            resolver(matriz, &TAM);
        }
        else{
            printf("Selecione uma opção válida");
        }

    } while(strcmp(opcao, "sair") != 0);
}

void criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD){
    srand(time(NULL));
    char dificuldade, nome[MAX];

    printf("Você iniciu um novo jogo, digite seu nome para continuar: ");
    fgets(nome, MAX, stdin);

    printf("Selecione a dificuldade do jogo:\nf: nível fácil, tamanho 3x3.\nm: nível médio, tamanho 5x5\nd: nível difícil, tamanho 7x7.\n");
    scanf("%c", &dificuldade);

    if(dificuldade == 'f'){ // Seleção de dificuldade e mudança de tamanho da matriz
        *TAM = 3;
    }
    else if(dificuldade == 'm'){
        *TAM = 5;
    }
    else if(dificuldade == 'd'){
        *TAM = 7;
    }
    else{
        printf("DIGITE UMA DIFICULDADE VÁLIDA");
    }

    /*matriz = malloc((*TAM+1) * sizeof(Numero*)); // Aloca um espaço de memória para a matriz        
    for(int i = 0; i < *TAM+1; i++){ 
        matriz[i] = malloc((*TAM+1) * sizeof(Numero));
    }*/ //Isso aqui deveria estar nessa função, porém está na função na função "novoJogo", preciso tirar de lá para passar pra ca! 
    
    for(int i = 0; i < *TAM+1; i++){ // Zera e atribui o estadoP/estadoU nulo (0) para a última linha da matriz (soma).
        matriz[*TAM][i].valor = 0; matriz[*TAM][i].estadoP = 0; matriz[*TAM][i].estadoU = 0;
    }
    for(int i = 0; i < *TAM+1; i++){ // Zera e atribui o estadoP/estadoU nulo (0) para a última linha da matriz (soma)..
        matriz[i][*TAM].valor = 0; matriz[i][*TAM].estadoP = 0; matriz[*TAM][i].estadoU = 0;
    }
    
    for(int i = 0; i < *TAM; i++){ // Estrutura de repetição para preencher a matriz e as dicas. comentar melhor depois
        for(int j = 0; j < *TAM; j++){
            matriz[i][j].valor = (rand() % 9) + 1;
            matriz[i][j].estadoP = -1;
            if((rand() % 2) == 0){ // Validação do número e somatório das dicas. comentar melhor depois
                matriz[i][*TAM].valor = matriz[i][*TAM].valor + matriz[i][j].valor;
                matriz[*TAM][j].valor = matriz[*TAM][j].valor + matriz[i][j].valor;
                matriz[i][j].estadoP = 1;
                (*estadoPL)++;
            }
            else{
                (*estadoPD)++;
            }
        }
    }
}

void adicionar(Numero **matriz, int l, int c){
    matriz[l-1][c-1].estadoU = 1;
}

void remover(Numero **matriz, int l, int c){
    matriz[l-1][c-1].estadoU = -1;
}

void dica(Numero **matriz, int *TAM){
    int validador = 0;
    for(int i = 0; i < *TAM && validador == 0; i++){ // A dica está seguindo a órdem da matriz, seria bom colocar uma órdem aleátoria depois
        for(int j = 0; j < *TAM; j++){
            if(matriz[i][j].estadoU != -1 && matriz[i][j].estadoP == -1){
                matriz[i][j].estadoU = -1;
                printf("\n\n\nAqui funciona\n\n\n");
                validador = 1;
                break;
            }
        }
    }
}

void resolver(Numero **matriz, int *TAM){ // Resolve o jogo.
    for(int i = 0; i < *TAM; i++){ // Percorre a matriz inteira alterando o estadoU para +1 ou -1.
        for(int j = 0; j < *TAM; j++){
            if(matriz[i][j].estadoP == -1){ 
                matriz[i][j].estadoU = -1;
            }
            else{
                matriz[i][j].estadoU = 1;
            }
        }
    }
}


void limpar_buffer(){
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}