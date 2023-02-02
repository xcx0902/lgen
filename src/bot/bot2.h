#ifndef __LGEN_BOT_BOT2_H
#define __LGEN_BOT_BOT2_H

#include "../defs.h"
#include "../map/defs.h"

namespace bot2 {
    defPlayer previousPos[20];
    int checkOrder[5] = {0, 1, 2, 3, 4};
    int id, armyNow, visitTime[20][505][505];
    struct node {
        int x, y, type, army, belong, dir;
    };
    int bot2(int playerId, defPlayer pos, int turn) {
        id = playerId;
        armyNow = map[pos.x][pos.y].army;
        visitTime[id][pos.x][pos.y]++;
        if (map[pos.x][pos.y].army == 0 || map[pos.x][pos.y].belong != id) {
            memset(visitTime[id], 0, sizeof(visitTime[id]));
            return 0;
        }
        std::random_shuffle(checkOrder + 1, checkOrder + 5);
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
            if (map[des.x][des.y].belong != id &&
                map[des.x][des.y].type == 3) {
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

#endif // __LGEN_BOT_BOT2_H
