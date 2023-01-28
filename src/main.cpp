#include "defs.h"
#include "game.h"
#include "map/defs.h"
#include "map/create.h"

HWND hwnd = GetConsoleWindow();

inline int selectSpeed() {
    printf("Select game speed: \n");
    for (int i = 1; i <= 7; i++)
        printf("   %s\n", delays[i]);
    gotoxy(2, 1);
    printf(">>");
    int choice = 1, input = 0;
    while (input != 13) {
        input = getch();
        gotoxy(choice + 1, 1);
        printf("  ");
        switch (tolower(input)) {
            case 'w': if (choice > 1) choice--; break;
            case 's': if (choice < 7) choice++; break;
        }
        gotoxy(choice + 1, 1);
        printf(">>");
    }
    if (choice == 7) return 0;
	else return 1000 / (1 << (choice - 1));
}

int main() {
    ShowWindow(hwnd, SW_MAXIMIZE);
    system("title Local Generals v1.0.0");
    initattr();
    hideCursor();
    printf("Welcome to Local Generals!\n");
    printf("Press any key to start...\n");
    getch();
    clearall();
    delay = selectSpeed();
    gotoxy(9, 1);
    fprintf(stderr, "delay = %d\n", delay);
    getch();
    clearall();
inputPlayers:
    printf("Input players (<=16): ");
    scanf("%d", &players);
    if (players < 2 || players > 16) {
        printf("Invalid input! Please input a valid number of players.\n");
        gotoxy(1, 1);
        clearline();
        goto inputPlayers;
    }
    fprintf(stderr, "players = %d\033[K\n", players);
    getch();
    clearall();
    printf("Creating map...\n");
    createStandardMap();
    setvbuf(stdout, nullptr, _IOFBF, 5000000);
    runGame();
    return 0;
}