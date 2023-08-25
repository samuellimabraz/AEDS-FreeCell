#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "freecell.h"
#include "resources.h"

int main()
{
    int op = 1;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 143); // Definição da cor de texto e fundo do terminal

    while (op != 4)
    {
        menu();
        scanf("%d", &op);

        Beep(550, 200); // Função para emitir sinais sonoros

        switch (op)
        {
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