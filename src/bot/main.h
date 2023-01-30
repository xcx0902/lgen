#ifndef __LGEN_BOT_MAIN_H
#define __LGEN_BOT_MAIN_H

#include "../defs.h"
#include "../map/defs.h"

namespace mainBot {
    struct node {
        int to, belong, type;
        ll army, del;
        bool operator < (node b) {
            return army < b.army || (army == b.army && del < b.del);
        }
    };
    node p[10];
    int cnt = 0;
    int mainBot(int id, defPlayer pos, int turn) {
        if (map[pos.x][pos.y].belong != id || map[pos.x][pos.y].army == 0) return 0;
        if (map[pos.x][pos.y].belong == id && map[pos.x][pos.y].type == 3 && turn > 150) // my gen
            for (int i = 1; i <= 4; i++) {
                int tx = pos.x + dx[i], ty = pos.y + dy[i];
                if (map[tx][ty].type != 2 && tx >= 1 && tx <= R && ty >= 1 && ty <= C)
                    return i + 4;
            }
        for (int i = 1; i <= 4; i++) {
            int tx = pos.x + dx[i], ty = pos.y + dy[i];
            if (map[tx][ty].type == 2 || tx < 1 || tx > R || ty < 1 || ty > C) continue;
            p[++cnt] = {i, map[tx][ty].belong, map[tx][ty].type, map[tx][ty].army, map[tx][ty].army};
            if (p[cnt].type != 1 && p[cnt].type != 3 && p[cnt].belong == id) // my plain/city
                p[cnt].army = -p[cnt].army-random(10), p[cnt].del = -p[cnt].del;
            if (p[cnt].type == 4 && p[cnt].belong != id) // other's city
                p[cnt].army = 2 * p[cnt].army - (ll)(1e16);
            else if (p[cnt].type == 0 && p[cnt].belong != id) // other's plain
                p[cnt].army = p[cnt].army - (ll)(1e16);
            else if (p[cnt].type == 1) // swamp
                p[cnt].del = 10, p[cnt].army = (ll)(1e17);
            else if (p[cnt].type == 3 && p[cnt].belong != id) // other's gen
                p[cnt].del = -(ll)(1e18), p[cnt].army = -(ll)(1e18);
        }
        std::sort(p + 1, p + cnt + 1);
        for (int i = 1; i <= cnt; i++)
            if (p[i].del < map[pos.x][pos.y].army)
                return p[i].to;
        return -1;
    }
}

#endif // __LGEN_BOT_MAIN_H