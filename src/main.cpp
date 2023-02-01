#include "defs.h"
#include "game.h"
#include "config.h"
#include "map/defs.h"
#include "map/create.h"
#include "map/draw.h"
#include "replay/create.h"
#include "replay/load.h"

HWND hwnd = GetConsoleWindow();

inline int selectSpeed() {
    printf("Select game speed: \n");
    for (int i = 1; i <= 8; i++)
        printf("   %s\n", delays[i]);
    gotoxy(5, 1);
    printf(">>");
    int choice = 4, input = 0;
    while (input != 13) {
        input = getch();
        gotoxy(choice + 1, 1);
        printf("  ");
        switch (tolower(input)) {
            case 'w': if (choice > 1) choice--; break;
            case 's': if (choice < 8) choice++; break;
        }
        gotoxy(choice + 1, 1);
        printf(">>");
    }
    if (choice == 7) return 0;
	else if (choice == 8) return -1;
    else return 400 / (1 << (choice - 1));
}

inline int selectCheatCode() {
    printf("Select cheat code: \n");
    int choice = 1, ret = 2;
    gotoxy(2, 1);
    printf(">>");
inputCheatCode:
    int input = 0;
    for (int i = 1; i <= players; i++) {
        gotoxy(i + 1, 4);
        if (ret & (1 << i)) underline();
        printTeam(i);
    }
    gotoxy(players + 2, 4);
    if (cheatCode == ((1 << players) - 1) << 1)
        underline();
    printf("Select All");
    resetattr();
    gotoxy(players + 3, 4);
    printf("COMPLETE SELECTING");
    while (input != 13) {
        input = getch();
        gotoxy(choice + 1, 1);
        printf("  ");
        switch (tolower(input)) {
            case 'w': if (choice > 1) choice--; break;
            case 's': if (choice < players + 2) choice++; break;
        }
        gotoxy(choice + 1, 1);
        printf(">>");
    }
    if (choice != players + 2) {
        if (choice == players + 1)
            ret = ((ret == ((1 << players) - 1) << 1)? 2 : ((1 << players) - 1) << 1);
        else ret ^= (1 << choice);
        goto inputCheatCode;
    }
    return ret;
}

inline void selectMap() {
    printf("Select map: \n");
    int choice = 1, input = 0;
    gotoxy(2, 4); printf("Use standard map");
    gotoxy(3, 4); printf("Import a map created by this program");
    gotoxy(2, 1); printf(">>");
    while (input != 13) {
        input = getch();
        gotoxy(choice + 1, 1);
        printf("  ");
        switch (tolower(input)) {
            case 'w': if (choice > 1) choice--; break;
            case 's': if (choice < 2) choice++; break;
        }
        gotoxy(choice + 1, 1);
        printf(">>");
    }
    if (choice == 1) {
        strcpy(mapName, "__STANDARD__");
        createStandardMap();
    }
    else {
        gotoxy(5, 1);
        char name[1000];
        printf("Enter map name (without prefix 'map/' and suffix '.lgmap'): ");
inputMapName:
        scanf("%s", name);
        if (_access(("map/" + std::string(name) + ".lgmap").c_str(), 0) == -1) {
            gotoxy(6, 1);
            clearline();
            printf("You don't have this map, please enter a valid map name");
            gotoxy(5, 61);
            clearline();
            goto inputMapName;
        }
        strcpy(mapName, name);
        readMap(name);
    }
}

inline void selectReplay() {
    printf("Do you want to save replay? \n");
    printf("Replay file will be very large, please be careful.\n");
    int choice = 2, input = 0;
    gotoxy(3, 4); printf("Yes");
    gotoxy(4, 4); printf("No");
    gotoxy(4, 1); printf(">>");
    while (input != 13) {
        input = getch();
        gotoxy(choice + 2, 1);
        printf("  ");
        switch (tolower(input)) {
            case 'w': if (choice > 1) choice--; break;
            case 's': if (choice < 2) choice++; break;
        }
        gotoxy(choice + 2, 1);
        printf(">>");
    }
    if (choice == 2) {
        useRep = false;
        return;
    }
    gotoxy(6, 1);
    printf("Please input your replay name: ");
    scanf("%s", repName);
    printf("Your replay will save to: %s.lgreplay\n", repName);
}

inline bool selectConfig() {
    printf("Do you want to use a saved config? \n");
    int choice = 2, input = 0;
    gotoxy(2, 4); printf("Yes");
    gotoxy(3, 4); printf("No");
    gotoxy(3, 1); printf(">>");
    while (input != 13) {
        input = getch();
        gotoxy(choice + 1, 1);
        printf("  ");
        switch (tolower(input)) {
            case 'w': if (choice > 1) choice--; break;
            case 's': if (choice < 2) choice++; break;
        }
        gotoxy(choice + 1, 1);
        printf(">>");
    }
    if (choice == 2) return false;
    gotoxy(5, 1);
    char configName[1000];
    printf("Please input your config name (without prefix 'config/' and suffix '.lgconfig'): ");
inputConfigName:
    scanf("%s", configName);
    if (_access(("config/" + std::string(configName) + ".lgconfig").c_str(), 0) == -1) {
        gotoxy(6, 1);
        clearline();
        printf("You don't have this config, please enter a valid config name");
        gotoxy(5, 82);
        clearline();
        goto inputConfigName;
    }
    readConfig(configName);
    return true;
}

inline void selectSaveConfig() {
    printf("Do you want to save your choice as a config? \n");
    int choice = 1, input = 0;
    gotoxy(2, 4); printf("Yes");
    gotoxy(3, 4); printf("No");
    gotoxy(2, 1); printf(">>");
    while (input != 13) {
        input = getch();
        gotoxy(choice + 1, 1);
        printf("  ");
        switch (tolower(input)) {
            case 'w': if (choice > 1) choice--; break;
            case 's': if (choice < 2) choice++; break;
        }
        gotoxy(choice + 1, 1);
        printf(">>");
    }
    if (choice == 2) return;
    gotoxy(5, 1);
    char configName[1000];
    printf("Please input your config name: ");
    scanf("%s", configName);
    writeConfig(configName);
}

int main() {
    ShowWindow(hwnd, SW_MAXIMIZE);
    system("title Local Generals v1.3.1");
    initattr();
    hideCursor();
    setFontSize(MAIN_PAGE_FONT_SIZE);
    printf("Welcome to Local Generals!\n");
    printf("Input a number to start: \n");
    printf("0: Start the game\n");
    printf("1: Draw a map\n");
    printf("2: Watch a saved replay\n");
    printf("Other: Quit\n");
    int stat = getch() - '0';
    if (stat == 1) {
        clearall();
        drawMap();
        return 0;
    }
    if (stat == 2) {
        clearall();
        loadReplay();
        return 0;
    }
    if (stat != 0)
        return 0;
    getch();
    clearall();
    if (selectConfig()) {
        clearall();
        goto gameStart;
    }
    clearall();
    delay = selectSpeed();
    gotoxy(10, 1);
    fprintf(stderr, "delay = %d\n", delay);
    getch();
    clearall();
inputPlayers:
    printf("Input players (<=16): ");
    scanf("%d", &players);
    if (players < 2 || players > 16) {
        clearall();
        printf("Invalid input! Please input a valid number of players.\n");
        getch();
        clearall();
        gotoxy(1, 1);
        goto inputPlayers;
    }
    fprintf(stderr, "players = %d\033[K\n", players);
    getch();
    clearall();
    cheatCode = selectCheatCode();
    gotoxy(players + 4, 1);
    fprintf(stderr, "cheatCode = %d\n", cheatCode);
    getch();
    clearall();
    selectMap();
    clearall();
    selectReplay();
    clearall();
    selectSaveConfig();
    clearall();
gameStart:
    printf("Press any key to start the game...");
    getch();
    clearall();
    setFontSize(IN_GAME_FONT_SIZE);
    setvbuf(stdout, nullptr, _IOFBF, 5000000);
    runGame();
    return 0;
}