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
    char nome[27]; // Nome do jogador.
    int TAM; // Tamanho da matriz: 3x3, 5x5 ou 7x7.
    int estadoPL; // Somatório dos números definidos pelo programa que estão ligados.
    int estadoUL; // Somatório dos números definidos pelo usuário que estão ligados.
    int estadoPD; // Somatório dos números definidos pelo programa que estão desligados.
    int estadoUD; // Somatório dos números definidos pelo usuário que estão desligados.
    int jogadas; // Somatório de todas as jogadas do jogador.
    int tempo; // Tempo de cada jogo.
} JogoSumplete;

// Protótipos para as funções:
void jogar(); 
void ajuda(int *ajuda); // Apresenta ao usuário suas opções de comandos.
Numero **criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD, char nome[27]); // Função responsável por criar uma matriz.
void imprimeMatriz(JogoSumplete *jogo); // Impressão da matriz na tela.
void adicionar(Numero **matriz, int TAM); // Adiciona um número como verdadeiro, estadoU = 1.
void salvarJogo(JogoSumplete jogo, time_t inicioJogo); // Salva o progresso do jogador.
void exibirRanking(); // Acessa o arquivo de ranking.
JogoSumplete carregarJogo(); // Puxa o progresso de um jogo através de um arquivo.
void remover(Numero **matriz, int TAM); // Adiciona um número como falso, estadoU = -1.
void dica(Numero **matriz, int *TAM); // Remove da matriz um número não pertecente a soma.
void resolver(Numero **matriz, int *TAM); // Resolve o jogo.
int verificaVitoria(JogoSumplete *jogo, time_t inicioJogo, int arregou); // Testa se o usuário terminou o jogo normalmente ou usando o comando "resolver".
void liberaMatriz(JogoSumplete *jogo); // Libera a memória alocada para a matriz.
void limpar_buffer(); // Limpa o buffer.

int main(){ // Main, link menu/funções
    setlocale(LC_ALL, "Portuguese"); // Para acentuações brasileiras funcionarem 

    char opcao[MAX];
    printf("Bem vindo ao Jogo SUMPLETE:\n\nComandos do jogo:\n-[novo]-\n-[carregar]-\n-[ranking]-\n-[ajuda]-\n-[sair]-\n\nSelecione um dos comandos anteriores escrevendo-o a seguir: ");
    do{
        scanf("%s", opcao);
        limpar_buffer();
        if(strcmp(opcao, "novo") == 0){ // Seleciona um novo jogo do zero.
            jogar(1);
        }
        else if(strcmp(opcao, "ajuda") == 0){ // Imprime os comandos de ajuda do Menu.
            int ajudaMenu = 1;
            ajuda(&ajudaMenu);
        }
        else if(strcmp(opcao, "sair") == 0){ // Fecha o jogo.
            return 0;
        }
        else if(strcmp(opcao, "carregar") == 0){ // Carrega um save através de um arquivo.
            jogar(2);
        }
        else if(strcmp(opcao, "ranking") == 0){ // Mostra o ranking para o jogador 
            exibirRanking();
        }
        else{
            printf("Você selecionou uma opção inválida, escreva \"ajuda\" para acessar os comandos disponíveis: ");
        }
    } while(strcmp(opcao, "sair") != 0);

    return 0;
}   

void jogar(int novoOuCarregar){ // Onde o jogo vai rodar.
    JogoSumplete jogo; // Criação de uma variável que vai conter informações importantes para o jogo funcionar.
    time_t inicioJogo;
    jogo.estadoPL = 0; jogo.estadoPD = 0; // Não sei se isso deveria estar aqui. 

    if(novoOuCarregar == 1){ // Verifica se é um novo jogo ou está carregando um save.
        jogo.matriz = criaMatriz(jogo.matriz, &jogo.TAM, &jogo.estadoPL, &jogo.estadoPD, jogo.nome); // Cria uma matriz e ela é adicionada a variável "matriz".
        inicioJogo = time(NULL); // Inicia a contagem do tempo assim que a matriz é montada.
    }
    else if(novoOuCarregar == 2){ // Verifica se é um novo jogo ou está carregando um save.
        jogo = carregarJogo();
        inicioJogo = time(NULL) - jogo.tempo; // Início do jogo recebe o tempo atual menos o tempo já passado no save inteiro para realizar a contagem final depois.
    }

    char opcao[MAX]; // Para selecionar opção.
    int arregou = 0; // Recebe um se o usuário usar o comando resolver e, desse modo, não recebe a vitória.

    do{ 
        imprimeMatriz(&jogo); // Printa todo o tabuleiro colorido na tela.

        int testeVitoria = verificaVitoria(&jogo, inicioJogo, arregou); // Testa se o usuário terminou o jogo normalmente ou usando o comando "resolver", valor armazenado em uma variável para não testar duas vezes.
        if(testeVitoria == 1 || testeVitoria == -1){ 
            printf("\nEsse jogo terminou, escreva \"ajuda\" para acessar seus comandos: ");
            return;
        }

        printf("\n%s, digite um comando: ", jogo.nome); // Seleção de comando.
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
                arregou = 1;
                resolver(jogo.matriz, &jogo.TAM);
            }
            else if(strcmp(opcao, "salvar") == 0){ // Salva o jogo e recolhe o nome do arquivo de save.
                salvarJogo(jogo, inicioJogo); 
            }
            else if(strcmp(opcao, "ajuda") == 0){ // Mostra ao jogador os comandos disponíveis na área de jogo.
                int ajudaJogo = 2;
                ajuda(&ajudaJogo);
            }
            else if(strcmp(opcao, "sair") == 0){ // Sai do jogo atual e pergunta o usuário se ele quer salvar, mexer aqui depois
                printf("Você deseja salvar o jogo?\nTecle \"s\" seguido de um nome para seu arquivo para salvar ou qualquer outra tecla para não salvar: ");
                char s;
                scanf(" %c", &s);
                if(s == 's'){
                    salvarJogo(jogo, inicioJogo); 
                    liberaMatriz(&jogo);
                }   
                else{
                    liberaMatriz(&jogo);
                    limpar_buffer();
                }
                printf("\nEsse jogo terminou, escreva \"ajuda\" para acessar seus comandos: ");
                return;
            }
            else{
                printf("Você selecionou uma opção inválida, escreva \"ajuda\" para acessar os comandos disponíveis: ");
                limpar_buffer(); // Para mensagem de erro não repetir.
            }
        } while((strcmp(opcao, "adicionar") != 0) && (strcmp(opcao, "remover") != 0) && (strcmp(opcao, "dica") != 0) && (strcmp(opcao, "resolver") != 0) && (strcmp(opcao, "salvar") != 0) && (strcmp(opcao, "ajuda") != 0) && (strcmp(opcao, "sair") != 0)); 

    } while(strcmp(opcao, "sair") != 0);
}

Numero **criaMatriz(Numero **matriz, int *TAM, int *estadoPL, int *estadoPD, char nome[27]){
    srand(time(NULL));
    char dificuldade;

    printf("Você iniciu um novo jogo, digite seu nome para continuar: ");
    scanf("%s", nome);

    printf("Selecione a dificuldade do jogo:\n- [f]: nível fácil, tamanho 3x3;\n- [m]: nível médio, tamanho 5x5;\n- [d]: nível difícil, tamanho 7x7.\n");
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

// Toda essa parte serve para imprimir a matriz na tela, validando os estados de ligado ou desligado dos números e realizando a contagem das dicas:
void imprimeMatriz(JogoSumplete *jogo){ // Talvez procurar deixar mais bonito se sobrar tempo.
    (*jogo).estadoUL = 0; (*jogo).estadoUD = 0; (*jogo).jogadas = 0; // Zera o contador do estado usuário ligado, do estado usuário desligado e a quantidade de jogadas toda vez.
    
    printf("\n"); // Um salto apenas pra ficar bunitin.
    printf("   "); // Espaço para alinhar.
    for(int i = 1; i <= (*jogo).TAM; i++){ // Impressão dos números das colunas.
        printf(BOLD("%2d "), i);
    }
    printf("\n");

    for(int i = 0; i < (*jogo).TAM+1; i++){ // Impressão da matriz na tela.
        
        if(i < (*jogo).TAM){
            printf(BOLD("%2d "), i + 1); // Impreme o número das linhas.
        } 
        else {
            printf("   "); // Espaço na linha das dicas.
        }

        for(int j = 0; j < (*jogo).TAM+1; j++){ 
            if((*jogo).matriz[i][j].estadoU == 1){
                printf(BOLD(GREEN("%2d ")), (*jogo).matriz[i][j].valor); // Impressão do valor em cor verde (estado usuário ligado).
                (*jogo).jogadas++;
                if((*jogo).matriz[i][j].estadoU == (*jogo).matriz[i][j].estadoP){ 
                    (*jogo).estadoUL++; // Acrescenta mais um no contador do estado usário ligado.
                }
            }
            else if((*jogo).matriz[i][j].estadoU == -1){
                printf(BOLD(RED("%2d ")), (*jogo).matriz[i][j].valor); // Impressão do valor em cor vermelha (estado usuário desligado).
                (*jogo).jogadas++;
                if((*jogo).matriz[i][j].estadoU == (*jogo).matriz[i][j].estadoP){
                    (*jogo).estadoUD++; // Acrescenta mais um no contador do estado usuário desligado.
                }
                else{
                    (*jogo).estadoUD--; // Retira um no contador do estado usuário ligado caso ele tenha errado um número.
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
                    printf(BOLD("%2d "), (*jogo).matriz[i][j].valor);
                }
                else{
                    printf(DIM("%2d "), (*jogo).matriz[i][j].valor); // Impressão das dicas com baixa opacidade.
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
                    printf(BOLD("%2d "), (*jogo).matriz[i][j].valor);
                }
                else{
                    printf(DIM("%2d "), (*jogo).matriz[i][j].valor); // Impressão das dicas com baixa opacidade.
                }   
            }
            else if((i == (*jogo).TAM) && (j == (*jogo).TAM)){ // Apenas para não printar a posição TAM.TAM.
            } 
            else
                printf(BOLD("%2d "), (*jogo).matriz[i][j].valor); // Impressão em negrito (estado usuário nulo).
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

void salvarJogo(JogoSumplete jogo, time_t inicioJogo){
    time_t fimJogo = time(NULL); 
    jogo.tempo = difftime(fimJogo, inicioJogo); // jogo.tempo recebe o tempo do jogador.
    char arqNome[MAX];
    scanf("%s", arqNome);
    strcat(arqNome, ".sum");
    FILE *save = fopen(arqNome, "w");
    fprintf(save, "%d\n", jogo.TAM); // printa o tamanho da matriz.
    for(int i = 0; i < jogo.TAM; i++){ // printa a atriz de tamanho TAM.
        for(int j = 0; j < jogo.TAM; j++){
            fprintf(save, "%d ", jogo.matriz[i][j].valor);
        }
        fprintf(save, "\n");
    }
    for(int i = 0; i < jogo.TAM; i++){ // Printa as dicas das linhas.
        fprintf(save, "%d ", jogo.matriz[i][jogo.TAM].valor);
    }
    fprintf(save, "\n");
    for(int i = 0; i < jogo.TAM; i++){ // Printa as dicas das colunas.
        fprintf(save, "%d ", jogo.matriz[jogo.TAM][i].valor);
    }
    fprintf(save, "\n");
    fprintf(save, "%d\n", jogo.estadoPD); // Printa quantas números no total estão desligados.
    for(int i = 0; i < jogo.TAM; i++){ // Printa as posições dos numeros que o usuário ainda precisa remover pra vencer.
        for(int j = 0; j < jogo.TAM; j++){
            if(jogo.matriz[i][j].estadoP == -1){
                fprintf(save, "%d %d\n", i+1, j+1);
            }
        }
    }
    fprintf(save, "%d\n", jogo.jogadas); // Printa a quantidade de escolhas que o usuário fez.
    for(int i = 0; i < jogo.TAM; i++){ // Printa a posição das jogadas que o usuário fez.
        for(int j = 0; j < jogo.TAM; j++){
            if(jogo.matriz[i][j].estadoU == 1){ // Para números marcados como verdadeiros.
                fprintf(save, "a %d %d\n", i+1, j+1);
            }
            else if(jogo.matriz[i][j].estadoU == -1){ // Para números marcados como falso.
                fprintf(save, "r %d %d\n", i+1, j+1);
            }
        }
    }
    fprintf(save, "%s\n", jogo.nome); // Printa o nome do jogador.
    fprintf(save, "%d", jogo.tempo); // Printa o tempo de jogo do jogador.
    fclose(save); // Fecha o arquivo.
    printf("\nVocê salvou o jogo com sucesso!\n");
}

JogoSumplete carregarJogo(){ // Puxa o progresso de um jogo através de um arquivo.
    JogoSumplete jogo;
    char arqNome[MAX];
    
    printf("Digite o nome do arquivo para carregar (sem o .sum): ");
    scanf("%s", arqNome);
    limpar_buffer();
    strcat(arqNome, ".sum");
    
    FILE *load = fopen(arqNome, "r"); // Abre pra leitura 
    fscanf(load, "%d", &jogo.TAM); // Lê o tamanho da matriz carregada.
    
    jogo.matriz = malloc((jogo.TAM+1) * sizeof(Numero*)); // Aloca um espaço de memória para a matriz carregada.
    for(int i = 0; i < jogo.TAM+1; i++){
        jogo.matriz[i] = malloc((jogo.TAM+1) * sizeof(Numero));
    }
    
    for(int i = 0; i < jogo.TAM+1; i++){ // Coloca valores neutros para as posições da matriz carregada.
        for(int j = 0; j < jogo.TAM+1; j++){ 
            jogo.matriz[i][j].estadoP = 1; 
            jogo.matriz[i][j].estadoU = 0;
            jogo.matriz[i][j].valor = 0;
        }
    }
    
    for(int i = 0; i < jogo.TAM; i++){ // Lê os valores da matriz carregada.
        for(int j = 0; j < jogo.TAM; j++){
            fscanf(load, "%d", &jogo.matriz[i][j].valor);
        }
    }
    
    for(int i = 0; i < jogo.TAM; i++){ // Lê os valores das dicas.
        fscanf(load, "%d", &jogo.matriz[i][jogo.TAM].valor); 
    }
    for(int i = 0; i < jogo.TAM; i++){ // Lê os valores das dicas.
        fscanf(load, "%d", &jogo.matriz[jogo.TAM][i].valor); 
    }
    
    int falsos; // Se trata dos números falsos
    fscanf(load, "%d", &falsos); 
    
    for(int i = 0; i < falsos; i++){
        int l, c;
        fscanf(load, "%d %d", &l, &c);
        jogo.matriz[l-1][c-1].estadoP = -1;
    }
    
    fscanf(load, "%d", &jogo.jogadas); // Quantidade de jogadas feitas pelo usuário.
    
    for(int i = 0; i < jogo.jogadas; i++){ // Lê as jogadas já realizadas pelo jogador.
        char tipo;
        int l, c;
        fscanf(load, " %c %d %d", &tipo, &l, &c);
        if(tipo == 'a'){ // Verifica se o número foi adicionado ou removido.
            jogo.matriz[l-1][c-1].estadoU = 1;
        }
        else if(tipo == 'r'){ // Verifica se o número foi adicionado ou removido.
            jogo.matriz[l-1][c-1].estadoU = -1;
        }
    }
    
    fscanf(load, "%s", jogo.nome); // Leitura do nome do jogo carregado.
    fscanf(load, "%d", &jogo.tempo); // Leitura do tempo do jogo carregado.
    
    fclose(load);
    
    jogo.estadoPL = 0; jogo.estadoPD = 0; // Zera os contadores do programa para não dar erro na verificação da vitória.
    jogo.estadoUL = 0; jogo.estadoUD = 0; // Zera os contadores do usuário para não dar erro na verificação da vitória.
    
    for(int i = 0; i < jogo.TAM; i++){
        for(int j = 0; j < jogo.TAM; j++){
            if(jogo.matriz[i][j].estadoP == 1){ // Faz a contagem dos números ligados pelo programa.
                jogo.estadoPL++;
            }
            else if(jogo.matriz[i][j].estadoP == -1){ // Faz a contagem dos números desligados pelo programa.
                jogo.estadoPD++;
            }
            
            if(jogo.matriz[i][j].estadoU == 1 && jogo.matriz[i][j].estadoP == 1){ // Faz a contagem dos números ligados e certos marcados pelo usuário.
                jogo.estadoUL++;
            }
            if(jogo.matriz[i][j].estadoU == -1 && jogo.matriz[i][j].estadoP == -1){ // Faz a contagem dos números desligados e certos marcaados pelo usuário.
                jogo.estadoUD++;
            }
        }
    }
    
    printf("Jogo carregado com sucesso!\n");
    return jogo; 
}
 

void liberaMatriz(JogoSumplete *jogo){ // Libera a memória alocada dinaminamente.
    for(int i = 0; i < ((*jogo).TAM)+1; i++){
        free((*jogo).matriz[i]);
    }
    free((*jogo).matriz);
}

void exibirRanking() {
    FILE *arqRank = fopen("sumplete.rnk", "r");
    if (arqRank == NULL){ // Verifica se holve erro pra abrir o arquivo, caso não exista arquivo o programa solicita uma vitória para criar o arquivo.
        printf("\nAinda não há nenhum ranking salvo! Vença uma partida primeiro.\n\n");
        return;
    }

    typedef struct { // Apenas para facilitar a manipulação das informações.
        char nome[27];
        int tempo;
    } Rank;
    
    Rank jogadores[10]; // Máximo de 10 jogadores.
    int qtd = 0;

    while(fscanf(arqRank, "%s %d", jogadores[qtd].nome, &jogadores[qtd].tempo) != EOF){ // Lê os dados que vamos colocar no ranking.
        qtd++;
        if (qtd >= 10) break; // Trava de segurança para não estourar o vetor.
    }
    fclose(arqRank); // Fecha o arquivo.

    if (qtd == 0){ // Caso haja o arquivo e não tenha informações.
        printf("\nO ranking está vazio!\n\n");
        return;
    }

    for (int i = 0; i < qtd; i++) { // fixa a posição 'i' e compara com as próximas 'j'.   
        for (int j = i + 1; j < qtd; j++) { // Se o jogador de cima (i) for mais lento que o de baixo (j), troca!
            if (jogadores[i].tempo > jogadores[j].tempo){
                Rank aux = jogadores[i];
                jogadores[i] = jogadores[j];
                jogadores[j] = aux;
            }
            
        }
    }

    printf("\n      --- RANKING DOS MELHORES TEMPOS ---\n");
    printf(" %-20s | %-10s\n", "Nome do Jogador", "Tempo (s)"); // Apresenta as informações com alguns espaçamentos para enquadrar.
    printf(" -------------------------------------------------\n");
    
    for (int i = 0; i < qtd; i++){
        printf(" %-20s |    %d\n", jogadores[i].nome, jogadores[i].tempo); // Printa o nome do jogador e o tempo.
    }
    printf(" -------------------------------------------------\n\n");
}

void ajuda(int *ajuda){ // Printa os comandos disponíveis para o usuário.
    if(*ajuda == 1){ // Comandos dentro do menu.
        printf("\nComo jogar:\nEm cada linha e coluna, os números que ficarem no tabuleiro devem somar exatamente o valor-dica mostrado ao lado (linhas) e acima (colunas).\n\n- Cada célula pode: manter ou remover o número.\n- Números removidos não contam na soma.\n- Você decide quais números apagar até todas as somas baterem.\n\nO puzzle termina quando todas as linhas e todas as colunas atingem suas somas ao mesmo tempo.\n\nComandos disponíveis no menu:\n-[novo]: Começar um novo jogo;\n-[carregar]: Carregar um jogo salvo em arquivo;\n-[ranking]: Exibir o ranking;\n-[sair]: Sair do jogo.\n\nDigite um dos comandos anteriores: ");
    }
    else if(*ajuda == 2){ // Comandos dentro do jogo.
        printf("\nComandos disponíveis:\n - [adicionar \"i\" \"j\"]: seleciona um número para adicionar;\n - [remover \"i\" \"j\"]: seleciona um número para remover;\n - [dica]: retira um número falso;\n - [resolver]: resolve o puzzle;\n - [salvar \"nome do arquivo\"]: salva o jogo atual.\n\nSeu tabuleiro atual:\n");
    }
}

int verificaVitoria(JogoSumplete *jogo, time_t inicioJogo, int arregou){ // Testa se o usuário terminou o jogo normalmente ou usando o comando "resolver".
    if(((*jogo).estadoUD == (*jogo).estadoPD) && arregou == 0){ // Terminou o jogo da maneira correta.
        printf("Parabéns, você ganhou!!!\n");
        time_t fimJogo = time(NULL);
        (*jogo).tempo = difftime(fimJogo, inicioJogo);
        printf("Tempo total: %.0d segundos\n", (*jogo).tempo);
        FILE *arqRank = fopen("sumplete.rnk", "a"); // Abre para adicionar no fim do arquivo
        fprintf(arqRank, "%s %d\n", (*jogo).nome, (*jogo).tempo); // Adiciona ao arquivo o nome do player e o tempo de jogo.
        fclose(arqRank); // Fecha o arquivo.
        liberaMatriz(jogo); // Libera a memória alocada dinamicamente.
        return 1;
    }
    else if(arregou == 1){ // Usou o comando resolver 
        printf("\nMais sorte na próxima vez.\n");
        liberaMatriz(jogo); // Libera a memória alocada dinamicamente.
        return -1;
    }
    else{ // Ainda não terminou o jogo.
        return 0;
    }
}

void limpar_buffer(){ // Para limpar o buffer.
   int ch;
   while ((ch = getchar()) != '\n' && ch != EOF);
}

////////////////////////////////////////////////////