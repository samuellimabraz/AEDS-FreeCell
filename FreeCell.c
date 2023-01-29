#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#define MAX 156

// Estrutura das Cartas duplamente encadeadas
typedef struct stCarta{
    short numero;
    short cor;
    char naipe;
    struct stCarta *prox;
    struct stCarta *ant;
}tCarta;
tCarta *Baralho = NULL; // Baralho para o jogo
tCarta *primMesa[8] = {NULL}; // Lista de cartas na mesa
tCarta *primNaipe[4] = {NULL}; // Monte de Naipes
tCarta *temp[4] = {NULL}; // Posições temporarias do jogo
tCarta *primMonte[8] = {NULL}; // Topo das listas de cartas que estao na mesa

char tabuleiro[MAX][8]; // Tabuleiro para impressão do jogo
const char simb[13] = "A23456789XJQK"; // Vetor de referência para os valores das cartas
int pontos = 0; // Pontuação do jogador


// FUNÇÕES DO JOGO //
void jogo(); // Inicia e da sequência ao jogo
// Inicio
void geraBaralho(); // Cria a lista de 52 cartas ordenadas do baralho
void embaralhaBaralho(); // Embaralhamento do Baralho, trocando os valores das posições
void distribuirMesa(); // Distribui o Baralho para as pilhas da mesa
// Movimentos
void moveMesaTemp(); // Movimento das pilhas da mesa para as posições temporarias
void moveTempMesa(); // Movimento das posicções temporarias para as pilhas da mesa
void moveMesaNaipe(); // Movimento das pilhas da mesa para os Montes de Naipes
void moveMesaMesa(); // Movimento entre as pilhas da mesa
void moveTempNaipe(); // Movimento das posições temporarias para os montes de naipes
void moveNaipeMesa(); // Movimento dos montes de naipes para as pilhas da mesa
void moveNaipeTemp(); // Movimento dos montes de naipes para as posições temporarias
// Impressão
void imprime(); // Impressão do jogo
void atualizaTab(); // Atualiza a Matriz de impressão
void jogadaInvalida(); // Mensagem de jogada invalida
// Funções auxiliares
int valida(int n, int l); // Valida coordenadas de entrada
void apagaJogo(); // Reinicia os ponteiros do jogo


// FUNÇÕES DE INTERAÇÃO //
void menu(); // Menu Inicial
void opcoes(); // Opções de jogada
void vitoria();
void billieJeans(int x); // Musica "Billie Jeans" com Beep
void feelGood(int x); // Musica "Feel Good" com Beep


// FUNÇÕES EXTRAS //
void carregando(); // Tela de carregamento
// Regulamento
void regras(); // Texto com os movimentos implementados no jogo
// Histórico de partidas
typedef struct no{
    char data[32];
    char horarioI[32];
    char horarioF[32];
    int min, seg, Dseg;
    int Pontos;
    struct no *esq, *dir;
}Dados;
Dados *raiz = NULL;

const char nome[] = {"Jogos_Salvos.txt"};

void salva(int o); // Salva o horario e a pontuacao de cada partida
void escreveEmOrdem(Dados *no, FILE *ptr);
void ordenaRank(); // Ordena o histórico a partir da pontuação
void historico(); // Acessa o arquivo salvo dos jogos


int main(){
    int op = 1;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 143); // Definição da cor de texto e fundo do terminal

    system("start Playlist.mp3");

    while (op != 4){
        menu();
        scanf("%d", &op);

        Beep(550, 200); // Função para emitir sinais sonoros

        switch(op){
            case 1:
                salva(0);
                jogo();
                break;
            case 2:
                getchar();
                carregando();
                regras();
                Beep(650, 200);
                break;
            case 3:
                getchar();
                carregando();
                historico();
                Beep(650, 200);
                break;
            case 4:
                break;
            default:
                printf("\nOpcao Invalida");
                Beep(350, 500);
                break;
        }
    }

    return 0;
}

void jogo(){
    int jogada;

    geraBaralho();
    embaralhaBaralho();
    distribuirMesa();

    while (pontos < 52 && jogada!=8){
        Sleep(1000);
        atualizaTab();
        imprime();

        opcoes();
        scanf("%d", &jogada);

        switch(jogada){
            case 1:
                moveMesaTemp();
                break;
            case 2:
                moveTempMesa();
                break;
            case 3:
                moveMesaNaipe();
                break;
            case 4:
                moveMesaMesa();
                break;
            case 5:
                moveTempNaipe();
                break;
            case 6:
                moveNaipeMesa();
                break;
            case 7:
                moveNaipeTemp();
                break;
            case 8:
                Beep(650, 200);
                printf("\nPONTOS: %d | Obrigado por tentar", pontos);
                Sleep(2500);
                break;
            default:
                jogadaInvalida();
                break;
        }
    }
    salva(1);
    ordenaRank();

    if(pontos == 52)
        vitoria();

    apagaJogo();
}

void geraBaralho(){
    int i, j;
    tCarta *novo;

    // Cria o Baralho com 52 cartas empilhadas
    for (i = 3; i <= 6; i++){
        for (j = 1; j <= 13; j++){
            novo = (tCarta*)malloc(sizeof(tCarta));
            novo->numero = j;
            novo->naipe = i;
            if(i<5)
                novo->cor = 0; // Cor 0, representando os naipes vermelhos
            else
                novo->cor = 1; // Cor 1, representando os naipes pretos

            novo->prox = Baralho;
            Baralho = novo;
        }
    }
    // Inicia o tabuleiro
    for (i = 0; i < MAX; i++)
        for (j = 0; j < 8; j++)
            tabuleiro[i][j] = ' ';
}

void embaralhaBaralho(){
    int i, c1, c2;
    tCarta *ct1, *ct2, *aux, *apaga;

    // Gera duas posições aleatorias do baralho e trocam seus valores
    srand(time(NULL));
    for(i=0; i<100; i++){ // 100 trocas de cartas
        c1 = rand()%52;
        c2 = rand()%52;
        ct1 = Baralho;
        ct2 = Baralho;

        while(c1--) ct1 = ct1->prox;

        aux = malloc(sizeof(tCarta));
        aux->numero = ct1->numero;
        aux->naipe = ct1->naipe;
        aux->cor = ct1->cor;
        apaga = aux;

        while(c2--) ct2 = ct2->prox;

        ct1->numero = ct2->numero;
        ct1->naipe = ct2->naipe;
        ct1->cor = ct2->cor;
        ct2->numero = aux->numero;
        ct2->naipe = aux->naipe;
        ct2->cor = aux->cor;

        free(apaga);
    }
}

void distribuirMesa(){
    int j = 0, x = 0;
    tCarta *carta;
    tCarta *atual, *aux;

    // Distribui as cartas do baralho para as pilhas da mesa, uma a uma
    // As cartas da mesa foram encadeadas na forma de lista duplamente encadeadas
    while (Baralho != NULL){
        if(j==8) j=0;

        carta = Baralho; // Carta que esta no topo do baralho
        Baralho = Baralho->prox; // Remoção do topo

        atual = primMesa[j];
        aux = NULL;
        while (atual != NULL){
            aux = atual;
            atual = atual->prox;
        }
        if(aux==NULL) primMesa[j] = carta; // Primeira carta inserida na lista da mesa
        else aux->prox = carta;

        carta->prox = atual;
        carta->ant = aux;
        primMonte[j] = carta; // Ponteiro para carta que está no "topo" de uma pilha, a ultima carta da lista
        j++; // Troca de Pilha

        atualizaTab(); // Impressão repetitiva do jogo a cada carta que é distribuida
        imprime();

        billieJeans(x);
        x++;
        if(x > 15) x = 0;
    }
}

void moveMesaTemp(){
    int m, t;

    printf("Pilha(0-7), Temporario(0-3): ");
    scanf("%d%d", &m, &t);

    if(valida(m, 7) && valida(t, 3) && primMonte[m] != NULL && temp[t] == NULL){

        temp[t] = primMonte[m]; // Atualiza a posição temporaria

        primMonte[m] = primMonte[m]->ant; // Atualizando o ponteiro para o "topo" da pilha
        if(primMonte[m]==NULL) primMesa[m] = NULL; // Remoção da ultima carta da pilha
        else primMonte[m]->prox = NULL;

        printf("[%d/%c] - Carta movida", temp[t]->numero, temp[t]->naipe);
        Beep(550, 200);

    }else jogadaInvalida();
}

void moveTempMesa(){
    int m, t;
    tCarta *atual;

    printf("Temporario(0-3), Pilha(0-7): ");
    scanf("%d%d", &t, &m);

    if(valida(t, 3) && valida(m, 7) && temp[t] != NULL &&// Validação das coordenadas
      (primMonte[m] == NULL || (temp[t]->cor != primMonte[m]->cor && temp[t]->numero == primMonte[m]->numero-1))){ // A pilha esta vazia ou possui uma carta de cor diferente e maior valor

            atual = primMonte[m];
            if(atual==NULL){ // lista vazia
                primMesa[m] = temp[t]; // Nova primeira carta da lista
                primMonte[m] = primMesa[m]; // Atualizando o ponteiro do "topo"
                primMonte[m]->ant = NULL;

            }else{
                atual->prox = temp[t]; // Atualizando o ponteiro para o "topo" da pilha
                primMonte[m] = primMonte[m]->prox;
                primMonte[m]->ant = atual;
            }
            temp[t] = NULL; // Atualizando a posição temporaria

            printf("[%d/%c] - Carta movida", primMonte[m]->numero, primMonte[m]->naipe);
            Beep(550, 200);

    }else jogadaInvalida();
}

void moveMesaNaipe(){
    int m, n;
    tCarta *aux;

    printf("Pilha(0-7), Naipe(0-3): ");
    scanf("%d%d", &m, &n);

    if(valida(m, 7) && valida(n, 3) && primMonte[m] != NULL && primMonte[m]->naipe == (n+3) && // A carta existe e o naipe de inserseção esta correto
     ((primNaipe[n] == NULL && primMonte[m]->numero == 1) || // Primeira carta do monte
      (primNaipe[n] != NULL && primMonte[m]->numero == primNaipe[n]->numero+1))){ // Demais cartas do monte

                aux = primMonte[m]->ant;

                primMonte[m]->prox = primNaipe[n];
                primNaipe[n] = primMonte[m]; // Movendo a carta para o monte de naipes

                primMonte[m] = aux; // Atualizando o ponteiro para o "topo" da pilha
                if(primMonte[m] == NULL) primMesa[m] = NULL; // Remoção da ultima carta da pilha
                else primMonte[m]->prox = NULL;

                printf("[%d/%c] - Carta movida", primNaipe[n]->numero, primNaipe[n]->naipe);
                Beep(550, 200);
                pontos++; // Contador de cartas inseridas nos montes de naipes

    }else jogadaInvalida();
}

void moveMesaMesa(){
    int i, j, n;
    tCarta *aux = NULL, *auxant = NULL;

    printf("Pilha(0-7), Pilha(0-7), Quantidade: ");
    scanf("%d%d%d", &i, &j, &n);

    if(valida(i, 7) && valida(j, 7) && i!=j) aux = primMonte[i]; // Ponteiro para carta inicial da pilha a ser movida
    if(aux != NULL) auxant = aux->ant; // Ponteiro para carta anterior que ficará na pilha incial

    while(auxant != NULL && n>1){ // Verifica se o bloco esta ordenado
        if(aux->ant->numero != aux->numero+1 || aux->ant->cor == aux->cor) break;
        else{
            aux = aux->ant;
            auxant = aux->ant;
        }
        n--;
    }

    // As cartas a serem movidas existem, o bloco a ser movido esta ordenado e existe na pilha
    if(aux != NULL && n==1 &&
      (primMesa[j] == NULL || (primMonte[j]->numero == aux->numero+1 && primMonte[j]->cor != aux->cor))){ // A posição a ser inserida é valida

        if(primMesa[j] == NULL){ // Lista vazia, na posição a ser inserida
            primMesa[j] = aux; // Nova primeira carta da lista
            primMonte[j] = primMonte[i];
            primMesa[j]->ant = NULL;
        }else{
            primMonte[j]->prox = aux; // Inserindo e atualizando o ponteiro para o "topo"
            aux->ant = primMonte[j];
            primMonte[j] = primMonte[i];
        }

        primMonte[i] = auxant; // Atualizando o ponteiro para o "topo"
        if(primMonte[i] == NULL) primMesa[i] = NULL; // Remoção da ultima carta da pilha
        else primMonte[i]->prox = NULL;

        printf("[%d/%c] : [%d/%c] Cartas movidas", aux->numero, aux->naipe, primMonte[j]->numero, primMonte[j]->naipe);
        Beep(550, 200);

    }else jogadaInvalida();
}

void moveTempNaipe(){
    int i, j;

    printf("Temp(0-3), Naipe(0-3): ");
    scanf("%d%d", &i, &j);

    if(valida(i, 3) && valida(j, 3) && temp[i] != NULL && temp[i]->naipe == (j+3) && // A carta existe e o naipe de inserseção esta correto
      ((primNaipe[j] == NULL && temp[i]->numero == 1) || // Primeira carta do monte
      (primNaipe[j] != NULL && temp[i]->numero == primNaipe[j]->numero+1))){ // Demais cartas do monte

        temp[i]->prox = primNaipe[j];
        primNaipe[j] = temp[i]; // Movendo a carta para o monte de naipes

        temp[i] = NULL;
        pontos++;

        printf("[%d/%c] - Carta movida", primNaipe[j]->numero, primNaipe[j]->naipe);
        Beep(550, 200);

      }else jogadaInvalida();
}

void moveNaipeMesa(){
    int i, j;
    tCarta *atual;

    printf("Naipe(0-3), Mesa(0-7): ");
    scanf("%d%d", &i, &j);

    // As coordenadas são validas, a carta existe, e a pilha da mesa está vazia ou segue a ordem das cartas
    if(valida(i, 3) && valida(j, 7) && primNaipe[i] != NULL &&
      (primMesa[j] == NULL || (primMonte[j] != NULL && primMonte[j]->numero == primNaipe[i]->numero+1 && primMonte[j]->cor != primNaipe[i]->cor))){

        atual = primMonte[j];
        if(primMesa[j]==NULL){ // Lista vazia, na posição a ser inserida
            primMesa[j] = primNaipe[i];
            primMonte[j] = primMesa[j];
            primMonte[j]->ant = NULL;
        }else{
            primMonte[j]->prox = primNaipe[i]; // Atualizando ponteiro para o "topo"
            primMonte[j] = primMonte[j]->prox;
            primMonte[j]->ant = atual;
        }
        primNaipe[i] = primNaipe[i]->prox; // Desempilhando o monte de naipes
        primMonte[j]->prox = NULL;
        pontos--;

        printf("[%d/%c] - Carta movida", primMonte[j]->numero, primMonte[j]->naipe);
        Beep(550, 200);

    }else jogadaInvalida();
}

void moveNaipeTemp(){
    int i, j;

    printf("Naipe(0-3), Temp(0-3): ");
    scanf("%d%d", &i, &j);

    // As coordenadas são validas, a carta existe e o temporario esta vazio
    if(valida(i, 3) && valida(j, 3) && primNaipe[i] != NULL && temp[j] == NULL){

        temp[j] = primNaipe[i];
        primNaipe[i] = primNaipe[i]->prox; // Desempilhando o monte de naipes
        pontos--;

        printf("[%d/%c] - Carta movida", temp[j]->numero, temp[j]->naipe);
        Beep(550, 200);

    }else jogadaInvalida();
}

void imprime(){
    int i, j, v=1, m; // Variaveis de controle de indices
    int k[8], l, c, s; // Contadores e flags
    tCarta *atual; // Ponteiro auxiliar para impressão das cores

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");

    // Cabeçalho
    printf("\n\t  Temporario          %c            Monte      \n   ", 184);

    // Temporario e Monte//
    for (i = -1; i < 3; i++){
        for (j = 0; j < 8; j++){
            if(j < 4) atual = temp[j];
            else atual = primNaipe[j-4];

            if(atual != NULL){ // Definição de cores a serem impressas
                if(atual->cor == 0) SetConsoleTextAttribute(h, 132);
                else                SetConsoleTextAttribute(h, 128);
            }

            if(j == 4) printf("      ");

            if(i==-1)           printf("   __ ");
            else if(i==0)       printf("|%c  | ", tabuleiro[i][j]);
            else if(i==1)       printf("| %c | ", tabuleiro[i][j]);
            else if(i==2)       printf("|__%c| ", tabuleiro[i][j]);

            SetConsoleTextAttribute(h, 143);
        }
        printf("\n    ");
    }
    printf("  0     1     2     3           %c     %c     %c     %c\n    ", 3,4,5,6);

    for (m = 0; m < 8; m++) k[m] = -1; // Indicador de cor
    l = c = s = 0; // Contadores para partes de impressão

    // Mesa //
    for (i = 3; i < MAX; i += v){
        for (j = 0; j < 8; j++){
            if((tabuleiro[i][j]==3 || tabuleiro[i][j]==4) || k[j]==0){ // Definição de cores a serem impressas
                SetConsoleTextAttribute(h, 132); // Cor vermalha
                k[j] = 0;
            }else if((tabuleiro[i][j]==5 || tabuleiro[i][j]==6) || k[j]==1 ){
                SetConsoleTextAttribute(h, 128); // Cor preta
                k[j] = 1;
            }

            if(j==4) printf("      "); // Espaçamento entre 4 pilhas

            if(tabuleiro[i][j] == ' '){ // Sem cartas
                printf("      ");
                s++; // Contador para quebra de impressão
                continue;
            }else if(l==0){ // Inicio da carta
                printf("  __  ");
                v=c=0;
                continue;
            }else v = 1;

            if(c==1)        printf("|%c  | ", tabuleiro[i][j]);
            else if(c==2)   printf("| %c | ", tabuleiro[i][j]);
            else if(c==3){
                printf("|__%c| ", tabuleiro[i][j]);

                for(m=0;m<8;m++) k[m] = -1; // Reincia o indicador de cores
                l=-1;
            }
            SetConsoleTextAttribute(h, 143);
        }
        if(s==8) break;
        l++; // Contador para cabeçalho
        c++; // Contador para as outras partes da carta
        s=0;
        printf("\n    ");
    }

     /*
    DESENHO DA CARTA
         __
       |n  |
       | v |
       |__n|
    */
}

void atualizaTab(){
    int i, j;
    tCarta *atual;

    // Temporario e Monte Naipes//
    for (i = 0; i < 8; i++){
        if(i < 4) atual = temp[i];
        else    atual = primNaipe[i-4];

        if(atual == NULL){
            tabuleiro[0][i] = ' ';
            tabuleiro[1][i] = ' ';
            tabuleiro[2][i] = ' ';
            continue;
        }
        tabuleiro[0][i] = atual->naipe; //naipe
        tabuleiro[1][i] = simb[atual->numero-1]; // valor
        tabuleiro[2][i] = atual->naipe; // naipe
    }

    // Pilhas Mesa //
     for (i = 0; i < 8; i++){
        atual = primMesa[i];
        for (j = 3; j < MAX; j += 3){
             if(atual==NULL){
                tabuleiro[j][i] = ' ';
                tabuleiro[j+1][i] = ' ';
                tabuleiro[j+2][i] = ' ';
                continue;
            }
            tabuleiro[j][i] = atual->naipe;
            tabuleiro[j+1][i] = simb[atual->numero-1];
            tabuleiro[j+2][i] = atual->naipe;
            atual = atual->prox;
        }
    }
}

int valida(int n, int l){
    return n>=0 && n<=l;
}

void jogadaInvalida(){
    printf("Jogada Invalida...!");
    Beep(350, 440);
}

void apagaJogo(){
    int i;
    tCarta *apaga, *atual;

    // Anula os ponteiros e desaloca as cartas que foram geradas
    pontos = 0;
    Baralho = NULL;

    for (i = 0; i < 8; i++){
        if(i < 4){
            atual = primNaipe[i];
            while(atual!=NULL){
                apaga = atual;
                atual = atual->prox;
                free(apaga);
            }
            primNaipe[i] = NULL;

            apaga = temp[i];
            if(apaga != NULL) free(apaga);
            temp[i] = NULL;
        }

        atual = primMesa[i];
        while(atual != NULL){
            apaga = atual;
            atual = atual->prox;
            free(apaga);
        }
        primMesa[i] = NULL;
        primMonte[i] = NULL;
    }
}

void menu(){
    system("cls");
    printf("\n\n\n");
    printf("            _____    _____     _____    _____     _____    _____                     \n");
    printf("           |        |     |   |        |         |        |        |        |        \n");
    printf("           |_____   |_____|   |_____   |_____    |        |_____   |        |        \n");
    printf("           |        |    \\    |        |         |        |        |        |        \n");
    printf("           |        |     \\   |_____   |_____    |_____   |_____   |_____   |_____   \n\n\n");
    printf("\n       1.Jogar               2.Regras               3.Historico               4.Sair\n");
    printf("\n  Escolha: ");
}

void opcoes(){
    char i;
    const char jogadas[8][24] = {"Mesa Temp", "Temp Mesa", "Mesa Naipe", "Mesa Mesa",
                          "Temp Naipe", "Naipe Mesa", "Naipe Temp", "Desistir"};

    printf("\n");
    printf("Pontos: %d\n", pontos);
    for(i = 0; i < 8; i++){
        printf("%s(%d)", jogadas[i], i+1);
        if(i == 3 || i == 7)  printf("\n");
        else              printf(" \\ ");
    }
    printf("\nJogada: ");
}

void billieJeans(int x){
                    //fS   cS   e   b    fsm   bm
    int notas[6] = {373, 280, 332, 249, 370, 494};
    int ordem[16] = {4, 1, 2, 0, 2, 1, 3, 1, 5, 1, 2, 0, 2, 1, 3, 1};

    Beep(notas[ordem[x]], 144);
}

void feelGood(int x){
                    //e    fs   g    c    b    a
    int notas[6] = {332, 373, 396, 528, 498, 444};
    int tempo[12] = {570, 270, 270, 270, 470, 770, 570, 270, 270, 270, 470, 770};
    int ordem[12] = {0, 0, 1, 2, 3, 4, 5, 5, 3, 4, 2, 0};

    Beep(notas[ordem[x]], tempo[x]);
    Sleep(tempo[x]-260);
}

void vitoria(){
    char i, j, c, v = 0;
    const char msg[7][200] ={"                 ..          ..          ..       ...       ....     ...            ",
                       "                  ..        ....        ..        ...       .....    ...            ",
                       "                   ..      ..  ..      ..         ...       ... ..   ...            ",
                       "                    ..    ..    ..    ..          ...       ...  ..  ...            ",
                       "                     ..  ..      ..  ..           ...       ...   .. ...            ",
                       "                      ....        ....            ...       ...    .....            ",
                       "                       ..          ..             ...       ...     ....            "};

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));

    // Mensagem e musica de vitoria
    for(c = 0; c < 32; c++){
        system("cls");
        printf("\n\n\n\n\n");
        for(i = 0; i < 7; i++){
            for(j = 0; j < strlen(msg[i]); j++){
                if(msg[i][j] == '.')    printf("%c", 1+(rand()%6));
                else                    printf(" ");
            }
            printf("\n");
        }
        SetConsoleTextAttribute(h, 0 + rand()%255);
        if(v > 15) v=0;
        billieJeans(v);
        v++;
    }
    v = 0;
    for(c = 0; c < 4; c++){
        system("cls");
        printf("\n\n\n\n\n");
        for(i = 0; i < 7; i++){
            for(j = 0; j < strlen(msg[i]); j++){
                if(msg[i][j] == '.')    printf("%c", 1+(rand()%6));
                else                    printf(" ");
            }
            printf("\n");
            if(v > 11) v=0;
            feelGood(v);
            v++;
        }
        SetConsoleTextAttribute(h, 0 + rand()%255);
    }
    SetConsoleTextAttribute(h, 143);
}

void carregando(){
    char i;

    system("cls");
    // Tela de carregamento de enfeite
    printf("\n\n\n\n\n\t\t\t\t\tCarregando\n\t\t\t\t\t");
    for(i = 1; i <= 18; i++){
        if(i == 10) printf("\r\r\r\t\t\t\t\t         \r\r\t\t\t\t\t");
        printf("%c", 219);
        Sleep(260);
    }
}

void regras(){
    char i, j, t = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");
    // Descrição das regras do jogo
    printf("\n\t\t\t\t%c%c%c Movimentos Validos %c%c%c\n\n", 176, 177, 178, 178, 177, 176);
    printf("\n %c MT - Movimento da carta do topo da pilha de uma das 8 mesas para uma das 4 posicoes temporarias.\n Apenas uma carta pode ser movida por vez, e a posicao temporaria deve estar vazia.\n", 250);
    printf("\n %c TM - Movimento da carta de alguma posicao temporaria para alguma das 8 pilhas da mesa.\n Apenas uma carta pode ser movida por vez, a carta a ser movida deve ser de cor diferente e \n menor valor da que se encontra na mesa.\n",250);
    printf("\n %c MN - Movimento da carta do topo da pilha de uma das mesas para um dos 4 montes de naipes.\n Os montes devem ser sequenciados, do As ao K, do mesmo naipe.\n", 250);
    printf("\n %c MM - Movimento entre as pilhas da mesa. Eh possivel movimentar blocos de carta, desde de que\n estejam ordenados entre si e com a posicao de insercao.\n", 250);
    printf("\n %c TN - Movimento de uma carta temporaria para um monte de naipe. Os montes devem ser sequenciados,\n do As ao K, do mesmo naipe.\n", 250);
    printf("\n %c NM - Movimento do monte de naipes para uma das pilhas da mesa. A carta a ser movida deve \n ser de cor diferente e menor valor da que se encontra na mesa.\n", 250);
    printf("\n %c NT - Movimento do monte de naipes para uma posicao temporaria. O temporario deve estar vazio.\n", 250);

    printf("\n\n          Temporario                          Monte      \n   ");
    for(i = 0; i < 4; i++){
        for(j = 0; j < 8; j++){
            if(j == 4){
                SetConsoleTextAttribute(h, 143);
                if(i == 1)        printf("  TN ->   ");
                else if(i == 3)   printf("  <- NT   ");
                else             printf("          ");
            }


            if(t == 4) t=0;
            if((t+3) == 3 || (t+3) == 4) SetConsoleTextAttribute(h, 132);
            else                     SetConsoleTextAttribute(h, 128);

            if(i==0)        printf("   __ ");
            else if(i==1)   printf("|%c  | ", t+3);
            else if(i==2)   printf("| %d | ", t);
            else if(i==3)   printf("|__%c| ", t+3);
            t++;
        }
        printf("\n    ");
    }
    SetConsoleTextAttribute(h, 143);
    printf("\n\n       %c MT      %c TM                     %c MN      %c NM\n\n   ", 24, 25, 24, 25);
    printf("                           Pilhas\n\n   ");
    t=-1;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 8; j++){
            if(j==4){
                SetConsoleTextAttribute(h, 143);
                if(i==1)        printf("    MM    ");
                else if(i==2)   printf("   <-->   ");
                else            printf("          ");
            }
            if(t == 4) t=0;
            if((t+3)==3 || (t+3)==4) SetConsoleTextAttribute(h, 132);
            else                     SetConsoleTextAttribute(h, 128);

            if(i==0)        printf("   __ ");
            else if(i==1)   printf("|%c  | ", t+3);
            else if(i==2)   printf("| %d | ", j);
            else if(i==3)   printf("|__%c| ", t+3);
            t++;
        }
        printf("\n    ");
    }
    SetConsoleTextAttribute(h, 143);

    printf("\n\n Pressione Enter para voltar... ");
    getchar();
}

void salva(int o){
    FILE *ptr;
    struct tm *data;
    time_t segundos;
    static int min, sec; // Variaveis para calculo da duração

    time(&segundos);
    data = localtime(&segundos);

    ptr = fopen("Jogos_Salvos.txt", "a");

    if(!o){
        fprintf(ptr, " %d/%d/%d \n", data->tm_mday, data->tm_mon+1, data->tm_year+1900);
        fprintf(ptr, " %d:%d:%d \n", data->tm_hour, data->tm_min, data->tm_sec);
        min = data->tm_min;
        sec = data->tm_sec;
    }else{
        min = data->tm_min - min;
        sec = data->tm_sec - sec;
        if(min < 0) min += 60;
        if(sec < 0){
            sec += 60;
            min--;
        }

        fprintf(ptr, " %d:%d:%d \n", data->tm_hour, data->tm_min, data->tm_sec);
        fprintf(ptr, " %d:%d \n", min, sec);
        fprintf(ptr, " %d \n\n", pontos);
    }

    fclose(ptr);
}

void escreveEmOrdem(Dados *no, FILE *ptr){
    // Gravar os dados no arquivo de forma ordenada
    if(no!=NULL){
        escreveEmOrdem(no->dir, ptr);
        fprintf(ptr, " %s \n", no->data);
        fprintf(ptr, " %s \n", no->horarioI);
        fprintf(ptr, " %s \n", no->horarioF);
        fprintf(ptr, " %d:%d \n", no->min, no->seg);
        fprintf(ptr, " %d \n\n", no->Pontos);
        escreveEmOrdem(no->esq, ptr);
    }
}

void ordenaRank(){
    Dados *ant, *atual, *novo;
    raiz = NULL;
    char c[3];

    FILE *ptr;
    ptr = fopen(nome, "r");
    rewind(ptr);

    if(ptr != NULL){
        while(1){

            ant = NULL;
            atual = raiz;

            novo = (Dados*)malloc(sizeof(Dados));
            fscanf(ptr, "%s", novo->data);
            fscanf(ptr, "%s", novo->horarioI);
            fscanf(ptr, "%s", novo->horarioF);
            fscanf(ptr, "%d %c %d", &novo->min, c, &novo->seg);
            fscanf(ptr, "%d", &novo->Pontos);
            novo->Dseg = (novo->min*60) + (novo->seg);
            novo->esq = NULL;
            novo->dir = NULL;

            if(feof(ptr)) break;

            if(strlen(novo->data) > 11 || strlen(novo->horarioI) > 9 || strlen(novo->horarioF) > 9 ||
                novo->Pontos > 52) continue;
            if(strlen(novo->data) < 7 || strlen(novo->horarioI) < 7 || strlen(novo->horarioF) < 7 ||
                novo->Pontos < 0) continue;

            if(raiz != NULL){
                while(atual!=NULL){
                    ant = atual;
                    if(novo->Pontos < atual->Pontos) atual = atual->esq;
                    else if(novo->Pontos == atual->Pontos){
                        if(novo->Dseg > atual->Dseg) atual = atual->esq;
                        else atual = atual->dir;
                    }else atual = atual->dir;
                }
                if(novo->Pontos < ant->Pontos) ant->esq = novo;
                else if(novo->Pontos == ant->Pontos){
                    if(novo->Dseg > ant->Dseg) ant->esq = novo;
                    else ant->dir = novo;
                }else ant->dir = novo;

            }else raiz = novo;
        }
        fclose(ptr);
        remove(nome);

        ptr = fopen(nome, "a");
        escreveEmOrdem(raiz, ptr);
        fclose(ptr);
    }
}

void historico(){
    int i = 1;
    char c[3];
    Dados *novo = NULL;
    FILE *ptr;
    ptr = fopen(nome, "r");

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");
    // Tela para mostrar os dados do historico
    printf("\n\t\t\t\t%c%c%c  HISTORICO  %c%c%c\n\n", 176, 177, 178, 178, 177, 176);

    if(ptr == NULL) printf("\n\n %c NAO HA JOGOS SALVOS %c\n", 174, 175);
    else{
        novo = (Dados*)malloc(sizeof(Dados));
        while(1){
            fscanf(ptr, "%s", novo->data);
            fscanf(ptr, "%s", novo->horarioI);
            fscanf(ptr, "%s", novo->horarioF);
            fscanf(ptr, "%d %c %d", &novo->min, c, &novo->seg);
            fscanf(ptr, "%d", &novo->Pontos);

            if(feof(ptr)) break;

            printf(" (%d)  ", i);
            if(i==1) SetConsoleTextAttribute(h, 134), printf("Ouro");
            if(i==2) SetConsoleTextAttribute(h, 129), printf("Prata");
            if(i==3) SetConsoleTextAttribute(h, 128), printf("Bronze");

            printf("\n Data: %s\n", novo->data);
            printf(" Inicio: %s\n", novo->horarioI);
            printf(" Fim: %s\n", novo->horarioF);
            printf(" Duracao: %d:%d\n", novo->min, novo->seg);
            printf(" Pontos: %d\n\n", novo->Pontos);
            i++;
            SetConsoleTextAttribute(h, 143);
        }
        free(novo);
    }
    fclose(ptr);

    printf("\n\n Pressione Enter para voltar... ");
    getchar();
}
