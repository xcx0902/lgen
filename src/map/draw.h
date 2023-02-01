#ifndef __LGEN_MAP_DRAW_H
#define __LGEN_MAP_DRAW_H

#include "defs.h"
#include "create.h"

int cmd;
ll army;
FILE *fpMap;
char filename[1000];
defPlayer now = {1, 1};

void writeMap() {
    fpMap = fopen(("map/" + std::string(filename) + ".lgmap").c_str(), "w");
    fprintf(fpMap, "%d %d\n", R, C);
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpMap, "%d ", map[i][j].army);
        fprintf(fpMap, "\n");
    }
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpMap, "%d ", map[i][j].type);
        fprintf(fpMap, "\n");
    }
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpMap, "%d ", map[i][j].visible);
        fprintf(fpMap, "\n");
    }
}

void drawMap() {
    if (_access("map/", 0) == -1)
        _mkdir("map/");
    printf("Input your map name: ");
    fflush(stdout);
    scanf("%s", filename);
    if (_access(("map/" + std::string(filename) + ".lgmap").c_str(), 0) != -1) {
        printf("You've already have this map, we'll load this map for you to update it\n");
        printf("Press any key to continue...");
        getch();
        readMap(filename);
        goto draw;
    }
    printf("Your map will save to: %s.lgmap\n", filename);
    printf("Press any key to continue...");
    fflush(stdout);
    getch();
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
draw:
    clearall();
    setFontSize(8);
    setvbuf(stdout, nullptr, _IOFBF, 5000000);
    while (1) {
        gotoxy(1, 1);
        printMap(0, now, 1);
        printf("w: save | q: save&quit | c: changeArmy | e: changeVisibility | 0: plain | 1: swamp | 2: mountain | 3: general | 4: city | visible = %d | army = %d\n", map[now.x][now.y].visible, army);
        clearline();
        printf("x = %d, y = %d", now.x, now.y);
        fflush(stdout);
        cmd = getch();
        switch (cmd) {
            case 'w': {
                writeMap();
                break;
            }
            case 'q':
                writeMap();
                return;
            case 'c':
                gotoxy(R + 2, 144);
                clearline();
                fflush(stdout);
                scanf("%lld", &army);
                break;
            case 'e':
                map[now.x][now.y].visible ^= 1;
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
                    case 77: if (now.y < C) now.y++; break;
                }
            }
        }
    }
}

#endif // __LGEN_MAP_DRAW_H