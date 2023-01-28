#ifndef __LGEN_MAP_DEFS_H
#define __LGEN_MAP_DEFS_H

#include "../defs.h"

bool isVisible(int x, int y, int cheat) {
    if (map[x][y].visible) return true;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (cheat & (1 << map[x + i][y + j].belong))
                return true;
    return false;
}

void printNum(bool visible, ll army, int belong, char lc, char rc, char mc = ' ', char rpc = ' ') {
    if (visible) {
        setfcolor(team[belong].color);
        putchar(lc);
        if (army < 0) {
            if (-army < 10) putchar(mc), printf("%2lld", army);
            else if (-army < 100) printf("%3lld", army);
            else if (-army < (ll)(1e13)) {
                std::string p = std::to_string(army);
                printf("%s%c", p.substr(0, 2).c_str(), suf[p.size() - 3]);
            } else fputs("-MX", stdout);
        } else if (army == 0) {
            putchar(mc), putchar(mc);
            if (lc == '[' || lc == '$') putchar('0');
            else putchar(mc);
        } else if (army < 10) putchar(mc), putchar(mc), printf("%1lld", army);
        else if (army < 100) putchar(mc), printf("%2lld", army);
        else if (army < 1000) printf("%3lld", army);
        else if (army < (ll)(1e14)) {
            std::string p = std::to_string(army);
            printf("%s%c", p.substr(0, 2).c_str(), suf[p.size() - 3]);
        } else fputs("MAX", stdout);
        putchar(rc);
    } else putchar(rpc), putchar(rpc), putchar(rpc), putchar(rpc), putchar(rpc);
    resetattr();
}

void printMap(int cheat, defPlayer player) {
    resetattr();
    register int i, j;
    for (i = 1; i <= R; i++) {
        for (j = 1; j <= C; j++) {
            resetattr();
            if (player.x == i && player.y == j)
                setbcolor(0x000080);
            switch (map[i][j].type) {
                case 0: { /* plain */
                    printNum(isVisible(i, j, cheat), map[i][j].army, map[i][j].belong, ' ', ' ');
                    break;
                }
                case 1: { /* swamp */
                    printNum(isVisible(i, j, cheat), map[i][j].army, map[i][j].belong, '=', '=', '=', '=');
                    break;
                }
                case 2: { /* mountain */
                    fputs("#####", stdout);
                    break;
                }
                case 3: { /* general */
                    if (!map[i][j].belong) {
                        if (isVisible(i, j, cheat)) fputs("$GEN$", stdout);
                        else fputs("     ", stdout);
                    } else printNum(isVisible(i, j, cheat), map[i][j].army, map[i][j].belong, '$', '$');
                    break;
                }
                case 4: { /* city */
                    printNum(isVisible(i, j, cheat), map[i][j].army, map[i][j].belong, '[', ']', ' ', '#');
                    break;
                }
            }
        }
        resetattr();
        clearline();
        putchar('\n');
    }
    putchar('\n');
    fflush(stdout);
}

#endif // __LGEN_MAP_DEFS_H