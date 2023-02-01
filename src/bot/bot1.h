#ifndef __LGEN_BOT_BOT1_H
#define __LGEN_BOT_BOT1_H

#include "../defs.h"
#include "../map/defs.h"

namespace bot1 {
    int bot1(int id, defPlayer pos, int turn) {
        if (map[pos.x][pos.y].belong != id || map[pos.x][pos.y].army == 0)
            return 0;
        struct node {
            int type, team, dir;
            ll army;
        } p[5];
        int cnt = 0;
        for (int i = 1; i <= 4; i++) {
            if (pos.x + dx[i] < 1 || pos.x + dx[i] > R || pos.y + dy[i] < 1 || pos.y + dy[i] > C || map[pos.x + dx[i]][pos.y + dy[i]].type == 2) continue;
            p[++cnt] = {map[pos.x + dx[i]][pos.y + dy[i]].type, map[pos.x + dx[i]][pos.y + dy[i]].belong, i, map[pos.x + dx[i]][pos.y + dy[i]].army};
        }
        bool rd = random(2);
        auto cmp = [&](node a, node b) -> bool {
            if (a.type == 3 && a.team != id) return true;
            if (b.type == 3 && b.team != id) return false;
            if (a.team == 0) return rd;
            if (b.team == 0) return !rd;
            if (a.team == id && b.team != id) return false;
            if (a.team != id && b.team == id) return true;
            if (a.team == id && b.team == id) return a.army > b.army;
            return a.army < b.army;
        };
        std::sort(p + 1, p + cnt + 1, cmp);
        return p[1].dir;
    }
}

#endif // __LGEN_BOT_MAIN_H
