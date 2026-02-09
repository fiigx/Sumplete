#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h> // Para acentuaÁıes brasileiras funcionarem 
#define MAX 100
#define ANSI_BOLD             "\x1b[1m"
#define ANSI_DIM "\x1b[2m"
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_RESET            "\x1b[0m"
#define DIM(string) ANSI_DIM string ANSI_RESET
#define BOLD(string)       ANSI_BOLD             string ANSI_RESET
#define RED(string)        ANSI_COLOR_RED        string ANSI_RESET
#define GREEN(string)      ANSI_COLOR_GREEN      string ANSI_RESET
typedef struct{
    int valor;
    int estadoP;
    int estadoU;
} Numero;

void limpar_buffer(); // Limpa o caralho do buffer
void novoJogo(); 
void ajuda();
void criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD); // Fun√ß√£o respons√°vel por criar uma matriz e imprimir na tela
void adicionar(Numero **matriz, int l, int c);
void remover(Numero **matriz, int l, int c);
void dica(Numero **matriz, int *TAM); // Remove da matriz um n√∫mero n√£o pertecente a soma.
void resolver(Numero **matriz, int *TAM); // Resolve o jogo

int main(){ // Main, link menu/fun√ß√µes
    setlocale(LC_ALL, "Portuguese"); // Para acentuaÁıes brasileiras funcionarem 

    char opcao[MAX];
    printf("Bem vindo ao Jogo SUMPLETE:\n\nComandos do jogo:\n- (sair)\n- (novo)\n- (ajuda)\nSelecione um dos comandos anteriores escrevendo-o a seguir: ");
    do{
        scanf("%s", opcao);
        limpar_buffer();
        if(strcmp(opcao, "novo") == 0){
            novoJogo();
        }
        else if(strcmp(opcao, "ajuda") == 0){
            ajuda();
        }
        else{
            printf("Selecione uma opÁ„o v·lida: ");
        }
    } while(strcmp(opcao, "sair") != 0);

    return 0;
}   

void novoJogo(){
    Numero **matriz; int TAM, estadoPL = 0, estadoPD = 0; char opcao[MAX];

    matriz = malloc((3+1) * sizeof(Numero*)); // Aloca um espa√ßo de mem√≥ria para a matriz        
    for(int i = 0; i < 3+1; i++){ 
        matriz[i] = malloc((3+1) * sizeof(Numero));
    }

    criaMatriz(matriz, &TAM, &estadoPL, &estadoPD);
    do{
        int estadoUL = 0, estadoUD = 0; // Zera o contador do estado usu√°rio ligado e do estado usu√°rio desligado toda vez.
        for(int i = 0; i < TAM+1; i++){ // Impress√£o da matriz na tela.
            for(int j = 0; j < TAM+1; j++){ 
                if(matriz[i][j].estadoU == 1){
                    printf(BOLD(GREEN("%d ")), matriz[i][j].valor); // Impress√£o do valor em cor verde (estado usu√°rio ligado).
                    if(matriz[i][j].estadoU == matriz[i][j].estadoP){ 
                        estadoUL++; // Acrescenta mais um no contador do estado us√°rio ligado.
                    }
                }
                else if(matriz[i][j].estadoU == -1){
                    printf(BOLD(RED("%d ")), matriz[i][j].valor); // Impress√£o do valor em cor vermelha (estado usu√°rio desligado).
                    if(matriz[i][j].estadoU == matriz[i][j].estadoP){
                        estadoUD++; // Acrescenta mais um no contador do estado us√°rio desligado.
                    }
                }
                else if(j == TAM || i == TAM){
                    printf(DIM("%d "), matriz[i][j].valor); // Impress√£o do valor em cor normal.
                    //printf(BOLD("%d "), matriz[i][j].valor);
                }
                else
                    //printf("%d ", matriz[i][j].valor); // Impress√£o do valor em cor normal.
                    printf(BOLD("%d "), matriz[i][j].valor);
            } 
            printf("\n");
        }

        if(estadoUD == estadoPD){
            printf("ParabÈns seu fudido, vocÍ ganhou!\n");
            return 0;
        }
        
        //printf("Somat√≥rio do estadoPL:%d Somat√≥rio do estado PD:%d\nSomat√≥rio do estadoUL:%d Somat√≥rio do estadoUD:%d\n\n", estadoPL, estadoPD, estadoUL, estadoUD); //apenas para saber quantos n√∫meros est√£o ligados e quantos est√£o desligados. tirar depois

        printf("\n\n");
        for(int i = 0; i < TAM+1; i++){ //estadosP do programa, tirar depois
            for(int j = 0; j < TAM+1; j++){
                printf("%d ", matriz[i][j].estadoP);
            }  
            printf("\n");
        }

        printf("O que vocÍ quer fazer: ");
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
            printf("Selecione uma opÁ„o v·lida");
        }

    } while(strcmp(opcao, "sair") != 0);
}

void criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD){
    srand(time(NULL));
    char dificuldade, nome[MAX];

    printf("VocÍ iniciu um novo jogo, digite seu nome para continuar: ");
    fgets(nome, MAX, stdin);

    printf("Selecione a dificuldade do jogo:\nf: nÌvel f·cil, tamanho 3x3.\nm: nÌvel mÈdio, tamanho 5x5\nd: nÌvel difÌcil, tamanho 7x7.\n");
    scanf("%c", &dificuldade);

    if(dificuldade == 'f'){ // Sele√ß√£o de dificuldade e mudan√ßa de tamanho da matriz
        *TAM = 3;
    }
    else if(dificuldade == 'm'){
        *TAM = 5;
    }
    else if(dificuldade == 'd'){
        *TAM = 7;
    }
    else{
        printf("DIGITE UMA DIFICULDADE V¡LIDA");
    }

    /*matriz = malloc((*TAM+1) * sizeof(Numero*)); // Aloca um espa√ßo de mem√≥ria para a matriz        
    for(int i = 0; i < *TAM+1; i++){ 
        matriz[i] = malloc((*TAM+1) * sizeof(Numero));
    }*/ //Isso aqui deveria estar nessa fun√ß√£o, por√©m est√° na fun√ß√£o na fun√ß√£o "novoJogo", preciso tirar de l√° para passar pra ca! 
    
    for(int i = 0; i < *TAM+1; i++){ // Zera e atribui o estadoP/estadoU nulo (0) para a √∫ltima linha da matriz (soma).
        matriz[*TAM][i].valor = 0; matriz[*TAM][i].estadoP = 0; matriz[*TAM][i].estadoU = 0;
    }
    for(int i = 0; i < *TAM+1; i++){ // Zera e atribui o estadoP/estadoU nulo (0) para a √∫ltima linha da matriz (soma)..
        matriz[i][*TAM].valor = 0; matriz[i][*TAM].estadoP = 0; matriz[*TAM][i].estadoU = 0;
    }
    
    for(int i = 0; i < *TAM; i++){ // Estrutura de repeti√ß√£o para preencher a matriz e as dicas. comentar melhor depois
        for(int j = 0; j < *TAM; j++){
            matriz[i][j].valor = (rand() % 9) + 1;
            matriz[i][j].estadoP = -1;
            if((rand() % 2) == 0){ // Valida√ß√£o do n√∫mero e somat√≥rio das dicas. comentar melhor depois
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
    for(int i = 0; i < *TAM && validador == 0; i++){ // A dica est√° seguindo a √≥rdem da matriz, seria bom colocar uma √≥rdem ale√°toria depois
        for(int j = 0; j < *TAM; j++){
            if(matriz[i][j].estadoU != -1 && matriz[i][j].estadoP == -1){
                matriz[i][j].estadoU = -1;
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

void ajuda(){
    printf("\nComandos: \nObjetivo:\nEm cada linha e coluna, os n˙meros que ficarem no tabuleiro devem somar exatamente o valor-dica mostrado ao lado (linhas) e acima (colunas).\n\nComo jogar:\n-Cada cÈlula pode: manter ou remover o n˙mero.\n-N˙meros removidos n„o contam na soma.\n-VocÍ decide quais n˙meros apagar atÈ todas as somas baterem.\n\nVitÛria:\nO puzzle termina quando todas as linhas e todas as colunas atingem suas somas ao mesmo tempo.");
}

void limpar_buffer(){
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}