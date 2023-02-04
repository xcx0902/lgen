#ifndef __LGEN_MAP_CREATE_H
#define __LGEN_MAP_CREATE_H

#include "defs.h"

inline void createStandardMap(int N = -1, int M = -1) {
    if (N < 0) R = random(30, 50); else R = N;
    if (M < 0) C = random(30, 50); else C = M;
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++) {
            int x1 = 0, x2 = 0, f = random(20);
            if (i - 2 > 0 && map[i - 2][j].type == 2) x1 = 1;
            if (j - 2 > 0 && map[i][j - 2].type == 2) x1 = 1;
            if (i - 2 > 0 && j + 2 <= C && map[i - 2][j + 2].type == 2) x1 = 1;
            if (i - 2 > 0 && j - 2 > 0  && map[i - 2][j - 2].type == 2) x1 = 1;
            if (i - 1 > 0 && map[i - 1][j].type == 1) x2 = 1;
            if (j - 1 > 0 && map[i][j - 1].type == 1) x2 = 1;
            map[i][j].army = map[i][j].visible = 0;
            if (!x1) {
                if (f < 8 - x2) map[i][j].type = 2;
                else if (f < 18) map[i][j].type = 0;
                else if (f < 19) map[i][j].type = 3;
                else map[i][j].type = 4, map[i][j].army = random(30, 40);
            } else {
                if (f < 18) map[i][j].type = 0;
                else if (f < 19) map[i][j].type = 3;
                else map[i][j].type = 4, map[i][j].army = random(30, 40);
            }
        }
}

inline void readMap(char *name) {
    FILE *fp = fopen(("map/" + std::string(name) + ".lgmap").c_str(), "r");
    fscanf(fp, "%d %d", &R, &C);
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            fscanf(fp, "%lld", &map[i][j].army);
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            fscanf(fp, "%d", &map[i][j].type);
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            fscanf(fp, "%d", &map[i][j].visible);
}

#endif // __LGEN_MAP_CREATE_H
