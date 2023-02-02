#ifndef __LGEN_BOT_MAIN_H
#define __LGEN_BOT_MAIN_H

#include "../defs.h"
#include "../map/defs.h"
#include <queue>

namespace mainBot {
    const int extdx[9] = {0, 1, 0, -1, 0, 1, 1, -1, -1};
    const int extdy[9] = {0, 0, 1, 0, -1, 1, -1, 1, -1};
    defPlayer previousPos[20];
    int checkOrder[5] = {0, 1, 2, 3, 4};
    int armyNow, found[20], visitTime[20][505][505];
    bool vis[MAX_SIZE + 5][MAX_SIZE + 5];
    struct node {
        int x, y, type, army, belong, dir;
    };
    struct general {
        ll army;
        defPlayer pos;
        int id;
        bool operator < (general b) {
            return army < b.army;
        }
    } gen[20][20];
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
    int mainBot(int id, defPlayer pos, int turn) {
        armyNow = map[pos.x][pos.y].army;
        visitTime[id][pos.x][pos.y]++;
        if (map[pos.x][pos.y].army == 0 || map[pos.x][pos.y].belong != id) {
            memset(visitTime[id], 0, sizeof(visitTime[id]));
            return 0;
        }
        std::random_shuffle(checkOrder + 1, checkOrder + 5);
        if (map[pos.x][pos.y].belong == id && map[pos.x][pos.y].type == 3 && turn > 150) // my gen
            for (int j = 1; j <= 4; j++) {
                int i = checkOrder[j];
                int tx = pos.x + dx[i], ty = pos.y + dy[i];
                if (map[tx][ty].belong == id && map[tx][ty].type != 1 && map[tx][ty].type != 2 && tx >= 1 && tx <= R && ty >= 1 && ty <= C)
                    return i + 4;
            }
        for (int i = 1; i <= 8; i++) {
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
            int mindis = 0x3f3f3f3f, minid = 0;
            for (int i = 1; i <= 4; i++) {
                int tx = pos.x + dx[i], ty = pos.y + dy[i];
                if (map[tx][ty].type != 2 && tx >= 1 && tx <= R && ty >= 1 && ty <= C && dist(gen[id][1].pos.x, gen[id][1].pos.y, tx, ty) < mindis)
                    mindis = dist(gen[id][1].pos.x, gen[id][1].pos.y, tx, ty), minid = i;
            }
            return minid;
        }
        int okDir = 0;
        for (int j = 1; j <= 4; j++) {
            int i = checkOrder[j];
            node des;
            des.dir = i;
            des.x = pos.x + dx[i];
            des.y = pos.y + dy[i];
            if (map[des.x][des.y].type == 2) continue;
            if (des.x < 1 || des.x > R || des.y < 1 || des.y > C) continue;
            okDir = i;
            des.type = map[des.x][des.y].type;
            des.army = map[des.x][des.y].army;
            des.belong = map[des.x][des.y].belong;
            if (map[des.x][des.y].belong != id && map[des.x][des.y].type == 3) {
                previousPos[id] = pos;
                return i;
            }
            if (des.type == 4 && des.army <= map[pos.x][pos.y].army && des.belong != id) {
                previousPos[id] = pos;
                return i;
            }
        }
        int i, timeToTry = 1000;
        srand(time(0));
        while (timeToTry--) {
            i = (mtrand() % 4 + rand() % 4 + rand() * rand() % 4) % 4 + 1;
            node des;
            des.dir = i;
            des.x = pos.x + dx[i];
            des.y = pos.y + dy[i];
            if (map[des.x][des.y].type == 2) continue;
            if (des.x < 1 || des.x > R || des.y < 1 || des.y > C) continue;
            des.type = map[des.x][des.y].type;
            des.army = map[des.x][des.y].army;
            des.belong = map[des.x][des.y].belong;
            if (map[des.x][des.y].belong != id && map[des.x][des.y].type == 3)
                return i;
            if (des.type == 4 && des.army <= map[pos.x][pos.y].army && des.belong == 0)
                return i;
            int cnt = 4;
            if (des.x == previousPos[id].x && des.y == previousPos[id].y)
                cnt += turn * 10;
            if (des.belong != id && des.belong != 0) cnt--;
            if (des.type == 0) cnt--;
            if (des.type == 1) cnt += 2;
            if (des.belong == 0) cnt--;
            if (des.belong == id && des.army >= 2000) cnt--;
            cnt += std::max(0, visitTime[id][des.x][des.y] * 10);
            if (mtrand() % cnt == 0) {
                previousPos[id] = pos;
                return i;
            }
        }
        return okDir;
    }
}

#endif // __LGEN_BOT_MAIN_H
