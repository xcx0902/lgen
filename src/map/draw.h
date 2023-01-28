#ifndef __LGEN_MAP_DRAW_H
#define __LGEN_MAP_DRAW_H

#include "defs.h"

int cmd;
ll army;
FILE *fp;
char filename[1000];
defPlayer now = {1, 1};

void writeMap() {
    fprintf(fp, "%d %d\n", R, C);
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fp, "%d ", map[i][j].army);
        fprintf(fp, "\n");
    }
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fp, "%d ", map[i][j].type);
        fprintf(fp, "\n");
    }
    clearall();
    printf("Map saved as %s\n", ("map/" + std::string(filename) + ".lgmap").c_str());
    printf("Press any key to quit...");
    fflush(stdout);
    getch();
}

void drawMap() {
    printf("Input your map name: ");
    fflush(stdout);
    scanf("%s", filename);
    printf("Your map will save to: %s.lgmap\n", filename);
    printf("Press any key to continue...");
    fflush(stdout);
    getch();
    fp = fopen(("map/" + std::string(filename) + ".lgmap").c_str(), "w");
    clearall();
    do {
        gotoxy(1, 1);
        clearline();
        printf("Input map R (<=500): ");
        fflush(stdout);
        scanf("%d", &R);
    } while (R < 1 || R > 500);
    do {
        gotoxy(2, 1);
        clearline();
        printf("Input map C (<=500): ");
        fflush(stdout);
        scanf("%d", &C);
    } while (C < 1 || C > 500);
    clearall();
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            map[i][j].visible = 1;
    setvbuf(stdout, nullptr, _IOFBF, 5000000);
    while (1) {
        gotoxy(1, 1);
        printMap(0, now);
        printf("w: save | c: changeArmy | 0: plain | 1: swamp | 2: mountain | 3: general | 4: city | army = %d\n", army);
        clearline();
        printf("x = %d, y = %d", now.x, now.y);
        fflush(stdout);
        cmd = getch();
        switch (cmd) {
            case 'w': {
                int ret = MessageBoxA(nullptr, "Are you sure to save the map and quit?", "Confirm", MB_YESNO);
                if (ret == IDYES) {
                    writeMap();
                    return;
                }
                break;
            }
            case 'c':
                gotoxy(R + 2, 93);
                clearline();
                fflush(stdout);
                scanf("%lld", &army);
                break;
            case '0':
                map[now.x][now.y].type = 0;
                map[now.x][now.y].army = army;
                break;
            case '1':
                map[now.x][now.y].type = 1;
                map[now.x][now.y].army = army;
                break;
            case '2':
                map[now.x][now.y].type = 2;
                break;
            case '3':
                map[now.x][now.y].type = 3;
                break;
            case '4':
                map[now.x][now.y].type = 4;
                map[now.x][now.y].army = army;
                break;
            case 224: {
                int tmp = getch();
                switch (tmp) {
                    case 72: if (now.x > 1) now.x--; break;
                    case 75: if (now.y > 1) now.y--; break;
                    case 80: if (now.x < R) now.x++; break;
                    case 77: if (now.x < C) now.y++; break;
                }
            }
        }
    }
}

#endif // __LGEN_MAP_DRAW_H