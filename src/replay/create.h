#ifndef __LGEN_REPLAY_CREATE_H
#define __LGEN_REPLAY_CREATE_H

#include "../defs.h"
#include "../map/defs.h"

FILE *fpCreateRep;
bool useRep = true;
char repName[1000];

inline void initReplay() {
    if (_access("replay/", 0) == -1)
        _mkdir("replay/");
    strcpy(repName, ("replay/" + std::string(repName) + ".lgreplay").c_str());
    fpCreateRep = fopen(repName, "wb");
    int zero = 0;
    binwrite(fpCreateRep, &R);
    binwrite(fpCreateRep, &C);
    binwrite(fpCreateRep, &players);
    binwrite(fpCreateRep, &zero);
    for (int i = 1; i <= players; i++) {
        binwrite(fpCreateRep, &gens[i].x);
        binwrite(fpCreateRep, &gens[i].y);
    }
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            binwrite(fpCreateRep, &map[i][j].army, 8);
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            binwrite(fpCreateRep, &map[i][j].type);
}

inline void saveReplay(int turn, int moves[]) {
    binwrite(fpCreateRep, &turn);
    for (int i = 1; i <= players; i++)
        binwrite(fpCreateRep, &moves[i]);
}

inline void setTurn(int turn) {
    rewind(fpCreateRep);
    binwrite(fpCreateRep, &R);
    binwrite(fpCreateRep, &C);
    binwrite(fpCreateRep, &players);
    binwrite(fpCreateRep, &turn);
}

#endif // __LGEN_REPLAY_CREATE_H
