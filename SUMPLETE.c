//GUILHERME AUGUSTO FIGUEIREDO GONÇALVES - 25.2.4151
#include <stdio.h> // Para funções básicas, como printf e scanf.
#include <stdlib.h> // Para alocar memória dinamicamente.
#include <string.h> // Para manipulação de strings.
#include <time.h> // Para pegar o tempo do pc e usar srand().
#include <locale.h> // Para acentuações brasileiras funcionarem.
#define MAX 100 // Usado para algumas strings.

// Definição de Macros e adição de cores para o jogo:
#define ANSI_BOLD             "\x1b[1m"
#define ANSI_DIM "\x1b[2m"
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_RESET            "\x1b[0m"
#define DIM(string) ANSI_DIM string ANSI_RESET
#define BOLD(string)       ANSI_BOLD             string ANSI_RESET
#define RED(string)        ANSI_COLOR_RED        string ANSI_RESET
#define GREEN(string)      ANSI_COLOR_GREEN      string ANSI_RESET

typedef struct{ // Cria uma estrutura que será usada para cada posição da matriz.
    int valor;
    int estadoP;
    int estadoU;
} Numero;

typedef struct{ // struct principal, mudar depois
    char nome[MAX];
    int tempo;
    int TAM;
    //Numero **matriz;
} JogoSumplete;


void novoJogo(); 
void ajuda(int *ajuda); // Apresenta ao usuário suas opções de comandos.
int criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD); // Função responsável por criar uma matriz e imprimir na tela.
void adicionar(Numero **matriz, int l, int c); // Adiciona um número como verdadeiro, estadoU = 1.
void remover(Numero **matriz, int l, int c); // Adiciona um número como falso, estadoU = -1.
void dica(Numero **matriz, int *TAM); // Remove da matriz um número não pertecente a soma.
void resolver(Numero **matriz, int *TAM); // Resolve o jogo.
int verificaVitoria(int **matriz, int estadoUD, int estadoPD); // Verifica se as condições de vitória estão satisfeitas.
void liberaMatriz(Numero **matriz, int *TAM); // Libera a memória alocada para a matriz.
void limpar_buffer(); // Limpa o buffer.

int main(){ // Main, link menu/funções
    setlocale(LC_ALL, "Portuguese"); // Para acentuações brasileiras funcionarem 

    char opcao[MAX];
    printf("Bem vindo ao Jogo SUMPLETE:\n\nComandos do jogo:\n- (sair)\n- (novo)\n- (ajuda)\nSelecione um dos comandos anteriores escrevendo-o a seguir: ");
    do{
        scanf("%s", opcao);
        limpar_buffer();
        if(strcmp(opcao, "novo") == 0){
            novoJogo();
        }
        else if(strcmp(opcao, "ajuda") == 0){
            int ajudaMenu = 1;
            ajuda(&ajudaMenu);
        }
        else if(strcmp(opcao, "sair") == 0){
            return 0;
        }
        else{
            printf("Você selecionou uma opção inválida, escreva \"ajuda\" para acessar os comandos disponíveis: ");
        }
    } while(strcmp(opcao, "sair") != 0);

    return 0;
}   

void novoJogo(){
    JogoSumplete jogo;

    Numero **matriz; int estadoPL = 0, estadoPD = 0; char opcao[MAX]; // Declaração de variáveis que vão ser usadas.

    matriz = criaMatriz(matriz, &jogo.TAM, &estadoPL, &estadoPD); // Cria uma matriz e ela é adicionada a variável "matriz".

    time_t inicioJogo, fimJogo;
    inicioJogo = time(NULL); // talvez eu devesse mudar isso de lugar depois


    do{
                
        int estadoUL = 0, estadoUD = 0; // Zera o contador do estado usuário ligado e do estado usuário desligado toda vez.
        for(int i = 0; i < jogo.TAM+1; i++){ // Impressão da matriz na tela.
            for(int j = 0; j < jogo.TAM+1; j++){ 
                if(matriz[i][j].estadoU == 1){
                    printf(BOLD(GREEN("%d ")), matriz[i][j].valor); // Impressão do valor em cor verde (estado usuário ligado).
                    if(matriz[i][j].estadoU == matriz[i][j].estadoP){ 
                        estadoUL++; // Acrescenta mais um no contador do estado usário ligado.
                    }
                }
                else if(matriz[i][j].estadoU == -1){
                    printf(BOLD(RED("%d ")), matriz[i][j].valor); // Impressão do valor em cor vermelha (estado usuário desligado).
                    if(matriz[i][j].estadoU == matriz[i][j].estadoP){
                        estadoUD++; // Acrescenta mais um no contador do estado usuário desligado.
                    }
                }
                else if(j == jogo.TAM && i != jogo.TAM){ // Impressão das dicas da direita.
                    int somaL = 0;
                    for(int k = 0; k < jogo.TAM; k++){ // Somatório dos números que não estão apagados.
                        if(matriz[i][k].estadoU != -1){
                            somaL = somaL + matriz[i][k].valor;
                        }
                    }
                    if(somaL == matriz[i][j].valor){ // Validação da dica e impressão dela em negrito.
                        printf(BOLD("%d "), matriz[i][j].valor);
                    }
                    else{
                        printf(DIM("%d "), matriz[i][j].valor); // Impressão das dicas com baixa opacidade.
                    }
                }
                else if(i == jogo.TAM && j != jogo.TAM){ // Impressão das dicas de baixo.
                    int somaC = 0;
                    for(int k = 0; k < jogo.TAM; k++){ // Somatório dos números que não estão apagados.
                        if(matriz[k][j].estadoU != -1){
                            somaC = somaC + matriz[k][j].valor;
                        }
                    }
                    if(somaC == matriz[i][j].valor){ // Validação da dica e impressão dela em negrito.
                        printf(BOLD("%d "), matriz[i][j].valor);
                    }
                    else{
                        printf(DIM("%d "), matriz[i][j].valor); // Impressão das dicas com baixa opacidade.
                    }   
                }
                else if((i == jogo.TAM) && (j == jogo.TAM)){ // Apenas para não printar a posição TAM.TAM.
                } 
                else
                    printf(BOLD("%d "), matriz[i][j].valor); // Impressão em negrito (estado usuário nulo).
            } 
            printf("\n");
        }

        if(verificaVitoria(matriz, estadoUD, estadoPD) == 1){

            

            double tempoGasto;
            printf("Voce venceu!\n");
            fimJogo = time(NULL);

            tempoGasto = difftime(fimJogo, inicioJogo);

            printf("Tempo total: %.0f segundos\n", tempoGasto);
            return 0; // Programa sai, mas talvez eu queira mudar depois
        }
        

        // Printa o estadoP de toma a matriz, apenas para verificação.
        /*printf("\n"); //Tirar depois
        for(int i = 0; i < TAM+1; i++){ //estadosP do programa, tirar depois
            for(int j = 0; j < TAM+1; j++){
                printf("%d ", matriz[i][j].estadoP);
            }  
            printf("\n");
        }*/

        printf("O que você quer fazer: "); // Seleção de comando.
        do{
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
                dica(matriz, &jogo.TAM);
            }
            else if(strcmp(opcao, "resolver") == 0){
                resolver(matriz, &jogo.TAM);
            }
            else if(strcmp(opcao, "salvar") == 0){ // Salva o jogo e recolhe o nome do arquivo de save.
                char arqNome[MAX];
                scanf("%s", arqNome);
                //salvarJogo(); 
            }
            else if(strcmp(opcao, "ajuda") == 0){
                int ajudaJogo = 2;
                ajuda(&ajudaJogo);
            }
            else{
                printf("Você selecionou uma opção inválida, escreva \"ajuda\" para acessar os comandos disponíveis: "); // Implementar ajuda aqui depois
            }
        } while((strcmp(opcao, "adicionar") != 0) && (strcmp(opcao, "remover") != 0) && (strcmp(opcao, "dica") != 0) && (strcmp(opcao, "resolver") != 0) && (strcmp(opcao, "salvar") != 0) && (strcmp(opcao, "ajuda") != 0)); // Verificar se todas as condições estão aqui antes de entregar, depois

    } while(strcmp(opcao, "sair") != 0);
}

int criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD){
    srand(time(NULL));
    char dificuldade, nome[MAX];

    printf("Você iniciu um novo jogo, digite seu nome para continuar: ");
    fgets(nome, MAX, stdin);

    printf("Selecione a dificuldade do jogo:\n- (f) nível fácil, tamanho 3x3.\n- (m) nével médio, tamanho 5x5.\n- (d) nível difícil, tamanho 7x7.\n");
    do{ // Seleção de dificuldade e mudança de tamanho da matriz.
        scanf(" %c", &dificuldade);
        limpar_buffer();
        if(dificuldade == 'f') 
            *TAM = 3;
        else if(dificuldade == 'm')
            *TAM = 5;
        else if(dificuldade == 'd')
            *TAM = 7;
        else
            printf("DIGITE UMA DIFICULDADE VÁLIDA: ");
    }
    while((dificuldade != 'f') && (dificuldade != 'm') && (dificuldade != 'd')); // Loop até digitar uma dificuldade válida.

    matriz = malloc((*TAM+1) * sizeof(Numero*)); // Aloca um espaço de memória para a matriz        
    for(int i = 0; i < *TAM+1; i++){ 
        matriz[i] = malloc((*TAM+1) * sizeof(Numero));
    } 
    
    for(int i = 0; i < *TAM+1; i++){ // Zera e atribui o estadoP/estadoU nulo (0) para a última linha da matriz (soma).
        matriz[*TAM][i].valor = 0; matriz[*TAM][i].estadoP = 0; matriz[*TAM][i].estadoU = 0;
    }
    for(int i = 0; i < *TAM+1; i++){ // Zera e atribui o estadoP/estadoU nulo (0) para a última linha da matriz (soma).
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
    return matriz;
}

void adicionar(Numero **matriz, int l, int c){ // Adiciona um número como verdadeiro, estadoU = 1.          
    matriz[l-1][c-1].estadoU = 1;
}

void remover(Numero **matriz, int l, int c){ // Adiciona um número como falso, estadoU = -1.
    matriz[l-1][c-1].estadoU = -1;
}

void dica(Numero **matriz, int *TAM){
    int validador = 0;
    for(int i = 0; i < *TAM && validador == 0; i++){ // A dica está seguindo a órdem da matriz, seria bom colocar uma órdem aleátoria depois
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

void liberaMatriz(Numero **matriz, int *TAM){ // Preciso verificar se está certo depois
    for(int i = 0; i < (*TAM)+1; i++){
        free(matriz[i]);
    }
    free(matriz);
}

void ajuda(int *ajuda){ // Printa os comandos disponíveis para o usuário.
    if(*ajuda == 1){
        printf("\nComandos disponíveis: \nComo jogar:\nObjetivo:\nEm cada linha e coluna, os números que ficarem no tabuleiro devem somar exatamente o valor-dica mostrado ao lado (linhas) e acima (colunas).\n\nComo jogar:\n-Cada célula pode: manter ou remover o n�mero.\n-Números removidos não contam na soma.\n-Você decide quais números apagar até todas as somas baterem.\n\nVitória:\nO puzzle termina quando todas as linhas e todas as colunas atingem suas somas ao mesmo tempo.");
        *ajuda == 0;
    }
    else if(*ajuda == 2){
        printf("Comandos disponíveis:\n - (adicionar \"x\" \"y\"): seleciona um número para adicionar\n - (remover \"x\" \"y\"): seleciona um número para remover\n - (dica): retira um número falso\n - (resolver): resolve o puzzle\n"); // Por todos os comandos aqui depois
        *ajuda = 0;
    }
}

int verificaVitoria(int **matriz, int estadoUD, int estadoPD){
    if(estadoUD == estadoPD){ // Verifica a condição de vitória e printa uma mensagem de vitória.
        printf("Parabéns seu fudido, você ganhou!\n");
        //liberaMatriz(matriz, TAM); // Preciso ver se a função de fato está liberando a matriz de maneira correta depois 
        return 1;
    }
    else{
        return 0;
    }
}

void limpar_buffer(){ // Para limpar o buffer.
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}