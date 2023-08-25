#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "resources.h"

/*
 * Menu do jogo
 */
void menu()
{
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

void opcoes()
{
    char i;
    const char jogadas[8][24] = {"Mesa Temp", "Temp Mesa", "Mesa Naipe", "Mesa Mesa",
                                 "Temp Naipe", "Naipe Mesa", "Naipe Temp", "Desistir"};

    for (i = 0; i < 8; i++)
    {
        printf("%s(%d)", jogadas[i], i + 1);
        if (i == 3 || i == 7)
            printf("\n");
        else
            printf(" \\ ");
    }
    printf("\nJogada: ");
}

void vitoria()
{
    char i, j, c, v = 0;
    const char msg[7][200] = {"                 ..          ..          ..       ...       ....     ...            ",
                              "                  ..        ....        ..        ...       .....    ...            ",
                              "                   ..      ..  ..      ..         ...       ... ..   ...            ",
                              "                    ..    ..    ..    ..          ...       ...  ..  ...            ",
                              "                     ..  ..      ..  ..           ...       ...   .. ...            ",
                              "                      ....        ....            ...       ...    .....            ",
                              "                       ..          ..             ...       ...     ....            "};

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));

    // Mensagem e musica de vitoria
    for (c = 0; c < 32; c++)
    {
        system("cls");
        printf("\n\n\n\n\n");
        for (i = 0; i < 7; i++)
        {
            for (j = 0; j < strlen(msg[i]); j++)
            {
                if (msg[i][j] == '.')
                    printf("%c", 1 + (rand() % 6));
                else
                    printf(" ");
            }
            printf("\n");
        }
        SetConsoleTextAttribute(h, 0 + rand() % 255);
        if (v > 15)
            v = 0;
        billieJeans(v);
        v++;
    }
    v = 0;
    for (c = 0; c < 4; c++)
    {
        system("cls");
        printf("\n\n\n\n\n");
        for (i = 0; i < 7; i++)
        {
            for (j = 0; j < strlen(msg[i]); j++)
            {
                if (msg[i][j] == '.')
                    printf("%c", 1 + (rand() % 6));
                else
                    printf(" ");
            }
            printf("\n");
            if (v > 11)
                v = 0;
            feelGood(v);
            v++;
        }
        SetConsoleTextAttribute(h, 0 + rand() % 255);
    }
    SetConsoleTextAttribute(h, 143);
}

void carregando()
{
    char i;

    system("cls");
    // Tela de carregamento de enfeite
    printf("\n\n\n\n\n\t\t\t\t\tCarregando\n\t\t\t\t\t");
    for (i = 1; i <= 18; i++)
    {
        if (i == 10)
            printf("\r\r\r\t\t\t\t\t         \r\r\t\t\t\t\t");
        printf("%c", 219);
        Sleep(260);
    }
}

void regras()
{
    char i, j, t = 0;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");
    // Descrição das regras do jogo
    printf("\n\t\t\t\t%c%c%c Movimentos Validos %c%c%c\n\n", 176, 177, 178, 178, 177, 176);
    printf("\n %c MT - Movimento da carta do topo da pilha de uma das 8 mesas para uma das 4 posicoes temporarias.\n Apenas uma carta pode ser movida por vez, e a posicao temporaria deve estar vazia.\n", 250);
    printf("\n %c TM - Movimento da carta de alguma posicao temporaria para alguma das 8 pilhas da mesa.\n Apenas uma carta pode ser movida por vez, a carta a ser movida deve ser de cor diferente e \n menor valor da que se encontra na mesa.\n", 250);
    printf("\n %c MN - Movimento da carta do topo da pilha de uma das mesas para um dos 4 montes de naipes.\n Os montes devem ser sequenciados, do As ao K, do mesmo naipe.\n", 250);
    printf("\n %c MM - Movimento entre as pilhas da mesa. Eh possivel movimentar blocos de carta, desde de que\n estejam ordenados entre si e com a posicao de insercao.\n", 250);
    printf("\n %c TN - Movimento de uma carta temporaria para um monte de naipe. Os montes devem ser sequenciados,\n do As ao K, do mesmo naipe.\n", 250);
    printf("\n %c NM - Movimento do monte de naipes para uma das pilhas da mesa. A carta a ser movida deve \n ser de cor diferente e menor valor da que se encontra na mesa.\n", 250);
    printf("\n %c NT - Movimento do monte de naipes para uma posicao temporaria. O temporario deve estar vazio.\n", 250);

    printf("\n\n          Temporario                          Monte      \n   ");
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (j == 4)
            {
                SetConsoleTextAttribute(h, 143);
                if (i == 1)
                    printf("  TN ->   ");
                else if (i == 3)
                    printf("  <- NT   ");
                else
                    printf("          ");
            }

            if (t == 4)
                t = 0;
            if ((t + 3) == 3 || (t + 3) == 4)
                SetConsoleTextAttribute(h, 132);
            else
                SetConsoleTextAttribute(h, 128);

            if (i == 0)
                printf("   __ ");
            else if (i == 1)
                printf("|%c  | ", t + 3);
            else if (i == 2)
                printf("| %d | ", t);
            else if (i == 3)
                printf("|__%c| ", t + 3);
            t++;
        }
        printf("\n    ");
    }
    SetConsoleTextAttribute(h, 143);
    printf("\n\n       %c MT      %c TM                     %c MN      %c NM\n\n   ", 24, 25, 24, 25);
    printf("                           Pilhas\n\n   ");
    t = -1;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (j == 4)
            {
                SetConsoleTextAttribute(h, 143);
                if (i == 1)
                    printf("    MM    ");
                else if (i == 2)
                    printf("   <-->   ");
                else
                    printf("          ");
            }
            if (t == 4)
                t = 0;
            if ((t + 3) == 3 || (t + 3) == 4)
                SetConsoleTextAttribute(h, 132);
            else
                SetConsoleTextAttribute(h, 128);

            if (i == 0)
                printf("   __ ");
            else if (i == 1)
                printf("|%c  | ", t + 3);
            else if (i == 2)
                printf("| %d | ", j);
            else if (i == 3)
                printf("|__%c| ", t + 3);
            t++;
        }
        printf("\n    ");
    }
    SetConsoleTextAttribute(h, 143);

    printf("\n\n Pressione Enter para voltar... ");
    getchar();
}

// ------------- Sounds --------------- //

void billieJeans(int x)
{
    // fS   cS   e   b    fsm   bm
    int notas[6] = {373, 280, 332, 249, 370, 494};
    int ordem[16] = {4, 1, 2, 0, 2, 1, 3, 1, 5, 1, 2, 0, 2, 1, 3, 1};

    Beep(notas[ordem[x]], 144);
}

void feelGood(int x)
{
    // e    fs   g    c    b    a
    int notas[6] = {332, 373, 396, 528, 498, 444};
    int tempo[12] = {570, 270, 270, 270, 470, 770, 570, 270, 270, 270, 470, 770};
    int ordem[12] = {0, 0, 1, 2, 3, 4, 5, 5, 3, 4, 2, 0};

    Beep(notas[ordem[x]], tempo[x]);
    Sleep(tempo[x] - 260);
}
