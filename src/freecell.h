#ifndef FREECELL_H
#define FREECELL_H

#include <stdio.h>

#define MAX 156

// Estrutura das Cartas duplamente encadeadas
typedef struct stCarta
{
    short numero;
    short cor;
    char naipe;
    struct stCarta *prox;
    struct stCarta *ant;
} tCarta;

void jogo(); // Inicia e da sequência ao jogo

void geraBaralho();      // Cria a lista de 52 cartas ordenadas do baralho
void embaralhaBaralho(); // Embaralhamento do Baralho, trocando os valores das posições
void distribuirMesa();   // Distribui o Baralho para as pilhas da mesa

void moveMesaTemp();  // Movimento das pilhas da mesa para as posições temporarias
void moveTempMesa();  // Movimento das posicções temporarias para as pilhas da mesa
void moveMesaNaipe(); // Movimento das pilhas da mesa para os Montes de Naipes
void moveMesaMesa();  // Movimento entre as pilhas da mesa
void moveTempNaipe(); // Movimento das posições temporarias para os montes de naipes
void moveNaipeMesa(); // Movimento dos montes de naipes para as pilhas da mesa
void moveNaipeTemp(); // Movimento dos montes de naipes para as posições temporarias

int valida(int n, int l); // Valida coordenadas de entrada
void jogadaInvalida();    // Mensagem de jogada invalida
void apagaJogo();         // Reinicia os ponteiros do jogo

// Impressão
void imprime();     // Impressão do jogo
void atualizaTab(); // Atualiza a Matriz de impressão

// Histórico de partidas
typedef struct no
{
    char data[32];
    char horarioI[32];
    char horarioF[32];
    int min, seg, Dseg;
    int Pontos;
    struct no *esq, *dir;
} Dados;

void salva(int o); // Salva o horario e a pontuacao de cada partida
void escreveEmOrdem(Dados *no, FILE *ptr);
void ordenaRank(); // Ordena o histórico a partir da pontuação
void historico();  // Acessa o arquivo salvo dos jogos

#endif