//GUILHERME AUGUSTO FIGUEIREDO GONÇALVES - 25.2.4151
// Inclusão de bibliotecas:
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

// Estruturas para gestão do jogo:
typedef struct{ // Cria uma estrutura que será usada para cada posição da matriz.
    int valor; 
    int estadoP;
    int estadoU;
} Numero;

typedef struct{ // Struct principal, carrega informações importantes sobre cada jogo.
    Numero **matriz; // matriz onde cada posição é um tipo "Numero".
    char nome[MAX]; // Nome do jogador.
    int TAM; // Tamanho da matriz: 3x3, 5x5 ou 7x7.
    int estadoPL; // Somatório dos números definidos pelo programa que estão ligados.
    int estadoUL; // Somatório dos números definidos pelo usuário que estão ligados.
    int estadoPD; // Somatório dos números definidos pelo programa que estão desligados.
    int estadoUD; // Somatório dos números definidos pelo usuário que estão desligados.
    int tempo; // Tempo de cada jogo.
} JogoSumplete;

// Protótipos para as funções:
void jogar(); 
void ajuda(int *ajuda); // Apresenta ao usuário suas opções de comandos.
Numero **criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD, char nome[MAX]); // Função responsável por criar uma matriz.
void imprimeMatriz(JogoSumplete *jogo); // Impressão da matriz na tela.
void adicionar(Numero **matriz, int TAM); // Adiciona um número como verdadeiro, estadoU = 1.
void remover(Numero **matriz, int TAM); // Adiciona um número como falso, estadoU = -1.
void dica(Numero **matriz, int *TAM); // Remove da matriz um número não pertecente a soma.
void resolver(Numero **matriz, int *TAM); // Resolve o jogo.
int verificaVitoria(JogoSumplete *jogo, time_t inicioJogo); // Verifica se as condições de vitória estão satisfeitas.
void liberaMatriz(JogoSumplete *jogo); // Libera a memória alocada para a matriz.
void limpar_buffer(); // Limpa o buffer.

int main(){ // Main, link menu/funções
    setlocale(LC_ALL, "Portuguese"); // Para acentuações brasileiras funcionarem 

    char opcao[MAX];
    printf("Bem vindo ao Jogo SUMPLETE:\n\nComandos do jogo:\n-[novo]-\n-[carregar]-\n-[ranking]-\n-[ajuda]-\n-[sair]-\n\nSelecione um dos comandos anteriores escrevendo-o a seguir: ");
    do{
        scanf("%s", opcao);
        limpar_buffer();
        if(strcmp(opcao, "novo") == 0){
            jogar();
        }
        else if(strcmp(opcao, "ajuda") == 0){
            int ajudaMenu = 1;
            ajuda(&ajudaMenu);
        }
        else if(strcmp(opcao, "sair") == 0){
            return 0;
        }
        else if(strcmp(opcao, "carregar") == 0){
            //função para carregar um save, mexer aqui depois
        }
        else if(strcmp(opcao, "ranking") == 0){
            //função para mostrar o ranking, mexer aqui depois
        }
        else{
            printf("Você selecionou uma opção inválida, escreva \"ajuda\" para acessar os comandos disponíveis: ");
        }
    } while(strcmp(opcao, "sair") != 0);

    return 0;
}   

void jogar(){
    JogoSumplete jogo; // Criação de uma variável que vai conter informações importantes para o jogo funcionar.

    jogo.estadoPL = 0; jogo.estadoPD = 0; 
    char opcao[MAX]; // Para selecionar opção.

    jogo.matriz = criaMatriz(jogo.matriz, &jogo.TAM, &jogo.estadoPL, &jogo.estadoPD, jogo.nome); // Cria uma matriz e ela é adicionada a variável "matriz".

    time_t inicioJogo; 
    inicioJogo = time(NULL); // Inicia a contagem do tempo assim que a matriz é montada.

    do{ 
        imprimeMatriz(&jogo); // Printa todo o tabuleiro colorido na tela.

        if(verificaVitoria(&jogo, inicioJogo) == 1){
            return;
        }

        printf("%s, digite um comando: ", jogo.nome); // Seleção de comando.
        do{
            scanf("%s", opcao);
            
            if(strcmp(opcao, "adicionar") == 0){ // Adiciona um número como verdadeiro, estadoU = +1.
                adicionar(jogo.matriz, jogo.TAM);
            }
            else if(strcmp(opcao, "remover") == 0){ // Adiciona um número como falso, estadoU = -1.
                remover(jogo.matriz, jogo.TAM);
            }
            else if(strcmp(opcao, "dica") == 0){ // Revela ao jogador um número falso.
                dica(jogo.matriz, &jogo.TAM);
            }
            else if(strcmp(opcao, "resolver") == 0){ // Resolve todo o puzzle
                resolver(jogo.matriz, &jogo.TAM);
            }
            else if(strcmp(opcao, "salvar") == 0){ // Salva o jogo e recolhe o nome do arquivo de save.
                char arqNome[MAX];
                scanf("%s", arqNome);
                //salvarJogo(); // mexer depois
            }
            else if(strcmp(opcao, "ajuda") == 0){ // Mostra ao jogador os comandos disponíveis na área de jogo.
                int ajudaJogo = 2;
                ajuda(&ajudaJogo);
            }
            else if(strcmp(opcao, "sair") == 0){ // Sai do jogo atual e pergunta o usuário se ele quer salvar, mexer aqui depois
                // Implementar as funções de salvar aqui tb depois
                liberaMatriz(&jogo);
                return;
            }
            else{
                printf("Você selecionou uma opção inválida, escreva \"ajuda\" para acessar os comandos disponíveis: ");
            }
        } while((strcmp(opcao, "adicionar") != 0) && (strcmp(opcao, "remover") != 0) && (strcmp(opcao, "dica") != 0) && (strcmp(opcao, "resolver") != 0) && (strcmp(opcao, "salvar") != 0) && (strcmp(opcao, "ajuda") != 0) && (strcmp(opcao, "sair") != 0)); 

    } while(strcmp(opcao, "sair") != 0);
}

Numero **criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD, char nome[MAX]){
    srand(time(NULL));
    char dificuldade;

    printf("Você iniciu um novo jogo, digite seu nome para continuar: ");
    scanf("%s", nome);

    printf("Selecione a dificuldade do jogo:\n- [f]: nível fácil, tamanho 3x3;\n- [m]: nével médio, tamanho 5x5;\n- [d]: nível difícil, tamanho 7x7.\n");
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
    } while((dificuldade != 'f') && (dificuldade != 'm') && (dificuldade != 'd')); // Loop até digitar uma dificuldade válida.

    matriz = malloc((*TAM+1) * sizeof(Numero*)); // Aloca um espaço de memória para a matriz        
    for(int i = 0; i < *TAM+1; i++){ 
        matriz[i] = malloc((*TAM+1) * sizeof(Numero));
    } 
    
    for(int i = 0; i < *TAM+1; i++){ // Zera e atribui o estadoP/estadoU nulo (0) para a última linha da matriz (soma).
        matriz[*TAM][i].valor = 0; matriz[*TAM][i].estadoP = 0; matriz[*TAM][i].estadoU = 0;
    }
    for(int i = 0; i < *TAM+1; i++){ // Zera e atribui o estadoP/estadoU nulo (0) para a última linha da matriz (soma).
        matriz[i][*TAM].valor = 0; matriz[i][*TAM].estadoP = 0; matriz[i][*TAM].estadoU = 0;
    }
    
    for(int i = 0; i < *TAM; i++){
        for(int j = 0; j < *TAM; j++){
            matriz[i][j].valor = (rand() % 9) + 1;
            matriz[i][j].estadoU = 0; // Atribui o estadoU neutro para todos os números da matriz, evita uso de lixo de memória.
            matriz[i][j].estadoP = -1;
            if((rand() % 2) == 0){ // Validação do número e somatório das dicas. 
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

void imprimeMatriz(JogoSumplete *jogo){ // Toda essa parte serve para imprimir a matriz na tela, validando os estados de ligado ou desligado dos números e realizando a contagem das dicas:
    (*jogo).estadoUL = 0; (*jogo).estadoUD = 0; // Zera o contador do estado usuário ligado e do estado usuário desligado toda vez.
    for(int i = 0; i < (*jogo).TAM+1; i++){ // Impressão da matriz na tela.
        for(int j = 0; j < (*jogo).TAM+1; j++){ 
            if((*jogo).matriz[i][j].estadoU == 1){
                printf(BOLD(GREEN("%d ")), (*jogo).matriz[i][j].valor); // Impressão do valor em cor verde (estado usuário ligado).
                if((*jogo).matriz[i][j].estadoU == (*jogo).matriz[i][j].estadoP){ 
                    (*jogo).estadoUL++; // Acrescenta mais um no contador do estado usário ligado.
                }
            }
            else if((*jogo).matriz[i][j].estadoU == -1){
                printf(BOLD(RED("%d ")), (*jogo).matriz[i][j].valor); // Impressão do valor em cor vermelha (estado usuário desligado).
                if((*jogo).matriz[i][j].estadoU == (*jogo).matriz[i][j].estadoP){
                    (*jogo).estadoUD++; // Acrescenta mais um no contador do estado usuário desligado.
                }
            }
            else if(j == (*jogo).TAM && i != (*jogo).TAM){ // Impressão das dicas da direita.
                int somaL = 0;
                for(int k = 0; k < (*jogo).TAM; k++){ // Somatório dos números que não estão apagados.
                    if((*jogo).matriz[i][k].estadoU != -1){
                        somaL = somaL + (*jogo).matriz[i][k].valor;
                    }
                }
                if(somaL == (*jogo).matriz[i][j].valor){ // Validação da dica e impressão dela em negrito.
                    printf(BOLD("%d "), (*jogo).matriz[i][j].valor);
                }
                else{
                    printf(DIM("%d "), (*jogo).matriz[i][j].valor); // Impressão das dicas com baixa opacidade.
                }
            }
            else if(i == (*jogo).TAM && j != (*jogo).TAM){ // Impressão das dicas de baixo.
                int somaC = 0;
                for(int k = 0; k < (*jogo).TAM; k++){ // Somatório dos números que não estão apagados.
                    if((*jogo).matriz[k][j].estadoU != -1){
                        somaC = somaC + (*jogo).matriz[k][j].valor;
                    }
                }
                if(somaC == (*jogo).matriz[i][j].valor){ // Validação da dica e impressão dela em negrito.
                    printf(BOLD("%d "), (*jogo).matriz[i][j].valor);
                }
                else{
                    printf(DIM("%d "), (*jogo).matriz[i][j].valor); // Impressão das dicas com baixa opacidade.
                }   
            }
            else if((i == (*jogo).TAM) && (j == (*jogo).TAM)){ // Apenas para não printar a posição TAM.TAM.
            } 
            else
                printf(BOLD("%d "), (*jogo).matriz[i][j].valor); // Impressão em negrito (estado usuário nulo).
        } 
        printf("\n"); 
    }
}

void adicionar(Numero **matriz, int TAM){ // Adiciona um número como verdadeiro, estadoU = 1.     
    int l, c; 
    do{
        scanf("%d%d", &l, &c);
        limpar_buffer(); // Limpa o buffer para não cair em loop.
        if(((l <= 0) || (l >= TAM+1)) || ((c <= 0) || (c >= TAM+1))){
            printf("Selecione uma posição válida, \"i\" e \"j\" aceitam valores de 1 a %d: ", TAM);
        }
    } while(((l <= 0) || (l >= TAM+1)) || ((c <= 0) || (c >= TAM+1)));     
    matriz[l-1][c-1].estadoU = 1;
}

void remover(Numero **matriz, int TAM){ // Adiciona um número como falso, estadoU = -1.
    int l, c; 
    do{
        scanf("%d%d", &l, &c);
        limpar_buffer(); // Limpa o buffer para não cair em loop.
        if(((l <= 0) || (l >= TAM+1)) || ((c <= 0) || (c >= TAM+1))){
            printf("Selecione uma posição válida, \"i\" e \"j\" aceitam valores de 1 a %d: ", TAM);
        }
    } while(((l <= 0) || (l >= TAM+1)) || ((c <= 0) || (c >= TAM+1)));  
    matriz[l-1][c-1].estadoU = -1;
}

void dica(Numero **matriz, int *TAM){
    int validador = 0;
    for(int i = 0; i < *TAM && validador == 0; i++){
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

void liberaMatriz(JogoSumplete *jogo){ // Libera a memória alocada dinaminamente.
    for(int i = 0; i < ((*jogo).TAM)+1; i++){
        free((*jogo).matriz[i]);
    }
    free((*jogo).matriz);
}

void ajuda(int *ajuda){ // Printa os comandos disponíveis para o usuário.
    if(*ajuda == 1){ // Comandos dentro do menu.
        printf("\nComo jogar:\nEm cada linha e coluna, os números que ficarem no tabuleiro devem somar exatamente o valor-dica mostrado ao lado (linhas) e acima (colunas).\n\n- Cada célula pode: manter ou remover o número.\n- Números removidos não contam na soma.\n- Você decide quais números apagar até todas as somas baterem.\n\nO puzzle termina quando todas as linhas e todas as colunas atingem suas somas ao mesmo tempo.\n\nComandos disponíveis no menu:\n-[novo]: Começar um novo jogo;\n-[carregar]: Carregar um jogo salvo em arquivo;\n-[ranking]: Exibir o ranking;\n-[sair]: Sair do jogo.\n\nDigite um dos comandos anteriores: ");
    }
    else if(*ajuda == 2){ // Comandos dentro do jogo.
        printf("\nComandos disponíveis:\n - [adicionar \"i\" \"j\"]: seleciona um número para adicionar;\n - [remover \"i\" \"j\"]: seleciona um número para remover;\n - [dica]: retira um número falso;\n - [resolver]: resolve o puzzle;\n - [salvar]: salva o jogo atual.\n\nSeu tabuleiro atual:\n");
    }
}

int verificaVitoria(JogoSumplete *jogo, time_t inicioJogo){
    if((*jogo).estadoUD == (*jogo).estadoPD){ // Verifica a condição de vitória e printa uma mensagem de vitória.
        printf("Parabéns seu fudido, você ganhou!\n");
        time_t fimJogo = time(NULL);
        (*jogo).tempo = difftime(fimJogo, inicioJogo);
        printf("Tempo total: %.0d segundos\n", (*jogo).tempo);
        liberaMatriz(jogo); // Libera a memória alocada dinamicamente.
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