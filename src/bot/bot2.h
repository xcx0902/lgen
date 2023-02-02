#ifndef __LGEN_BOT_BOT2_H
#define __LGEN_BOT_BOT2_H

#include "../defs.h"
#include "../map/defs.h"
#include <queue>

namespace bot2 {
    const int extdx[5] = {0, 1, 1, -1, -1};
    const int extdy[5] = {0, 1, -1, 1, -1};
    struct node {
        int to, belong, type;
        ll army, del;
        bool operator < (node b) {
            return army < b.army || (army == b.army && del < b.del);
        }
    } p[10];
    struct general {
        ll army;
        defPlayer pos;
        int id;
        bool operator < (general b) {
            return army < b.army;
        }
    } gen[20][20];
    int cnt, swamps, lstSwamp, found[20];
    bool vis[MAX_SIZE + 5][MAX_SIZE + 5];
    struct Node {
        defPlayer pos;
        int dist;
    };
    inline int dist(int x1, int y1, int x2, int y2) {
        memset(vis, 0, sizeof vis);
        std::queue<Node> q;
        q.push({{x1, y1}, 0});
        while (!q.empty()) {
            Node u = q.front(); q.pop();
            if (u.pos.x == x2 && u.pos.y == y2)
                return u.dist;
            for (int i = 1; i <= 4; i++) {
                int tx = u.pos.x + dx[i], ty = u.pos.y + dy[i];
                if (tx < 1 || tx > R || ty < 1 || ty > C || vis[tx][ty] || map[tx][ty].type == 2)
                    continue;
                vis[tx][ty] = 1;
                q.push({{tx, ty}, u.dist + 1});
            }
        }
    }
    inline int countSwamp() {
        if (swamps) return swamps;
        for (int i = 1; i <= R; i++)
            for (int j = 1; j <= C; j++)
                swamps += map[i][j].type == 1;
        return swamps;
    }
    int bot2(int id, defPlayer pos, int turn) {
        cnt = 0;
        if (map[pos.x][pos.y].belong != id || map[pos.x][pos.y].army == 0) return 0;
        if (map[pos.x][pos.y].belong == id && map[pos.x][pos.y].type == 3 && turn > 150 && random(4)) // my gen
            for (int i = 1; i <= 4; i++) {
                int tx = pos.x + dx[i], ty = pos.y + dy[i];
                if (map[tx][ty].belong == id && map[tx][ty].type != 1 && map[tx][ty].type != 2 && tx >= 1 && tx <= R && ty >= 1 && ty <= C)
                    return i + 4;
            }
        if (map[pos.x][pos.y].type == 1)
            lstSwamp++;
        else lstSwamp = 0;
        for (int i = 1; i <= 4; i++) {
            int tx = pos.x + extdx[i], ty = pos.y + extdy[i];
            if (tx >= 1 && tx <= R && ty >= 1 && ty <= C && map[tx][ty].belong != id && map[tx][ty].type == 3)
                gen[id][++found[id]] = {map[tx][ty].army, {tx, ty}, map[tx][ty].belong};
        }
        int killed = 0;
        for (int i = 1; i <= found[id]; i++)
            if (!isAlive[gen[id][i].id])
                gen[id][i].army = (ll)(1e18), killed++;
        std::sort(gen[id] + 1, gen[id] + found[id] + 1);
        found[id] -= killed;
        if (found[id] && map[pos.x][pos.y].army > gen[id][1].army + 100) { // found a gen: if have enough army, attack
            // gotoxy(R + 1 + players + id, 1);
            // clearline();
            // printTeam(id);
            // printf(": Found %d gens, goto %d", found[id], gen[id][1].id);
            // fflush(stdout); getch();
            int mindis = 0x3f3f3f3f, minid = 0;
            for (int i = 1; i <= 4; i++) {
                int tx = pos.x + dx[i], ty = pos.y + dy[i];
                if (map[tx][ty].type != 2 && tx >= 1 && tx <= R && ty >= 1 && ty <= C && dist(gen[id][1].pos.x, gen[id][1].pos.y, tx, ty) < mindis)
                    mindis = dist(gen[id][1].pos.x, gen[id][1].pos.y, tx, ty), minid = i;
            }
            return minid;
        }
        for (int i = 1; i <= 4; i++) {
            int tx = pos.x + dx[i], ty = pos.y + dy[i];
            if (map[tx][ty].type == 2 || tx < 1 || tx > R || ty < 1 || ty > C) continue;
            p[++cnt] = {i, map[tx][ty].belong, map[tx][ty].type, map[tx][ty].army, map[tx][ty].army};
            if (p[cnt].type == 3 && p[cnt].belong == id) // my gen
                p[cnt].army = p[cnt].del = (ll)(2e18) * (random(4)? 1 : -1);
            else if (p[cnt].type != 1 && p[cnt].belong == id) // my plain/city
                p[cnt].army = -p[cnt].army-random(50), p[cnt].del = -p[cnt].del;
            if (p[cnt].type == 4 && p[cnt].belong != id) // other's city
                p[cnt].army = 2 * p[cnt].army - (ll)(1e16) - random(50), (random(4)? 0 : p[cnt].del = 0);
            else if (p[cnt].type == 0 && p[cnt].belong != id) // other's plain
                p[cnt].army = p[cnt].army - (ll)(1e16) - random(50);
            else if (p[cnt].type == 1) // swamp
                p[cnt].del = 5, p[cnt].army = random(lstSwamp > 10? 0 : -std::max(100, countSwamp()), 100);
            else if (p[cnt].type == 3 && p[cnt].belong != id) { // other's gen
                p[cnt].del = -(ll)(1e18), p[cnt].army = -(ll)(1e18);
                bool flag = false;
                for (int i = 1; i <= players; i++)
                    if (gen[id][i].id == p[cnt].belong) {
                        gen[id][i] = {map[tx][ty].army, {tx, ty}, p[cnt].belong};
                        flag = true;
                        break;
                    }
                if (!flag)
                    gen[id][++found[id]] = {map[tx][ty].army, {tx, ty}, p[cnt].belong};
            }
        }
        std::sort(p + 1, p + cnt + 1);
        // gotoxy(R + 1 + players + id, 1);
        // clearline();
        // printTeam(id);
        // printf(": ");
        // for (int i = 1; i <= cnt; i++)
        //     printf("{%d %d %lld %lld %d} ", p[i].to, p[i].belong, p[i].army, p[i].del, p[i].type);
        // fflush(stdout); getch();
        for (int i = 1; i <= cnt; i++)
            if (p[i].del < map[pos.x][pos.y].army)
                return p[i].to;
        return -1;
    }
}

#endif // __LGEN_BOT_BOT2_H
