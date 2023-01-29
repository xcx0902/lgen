#ifndef __LGEN_REPLAY_CREATE_H
#define __LGEN_REPLAY_CREATE_H

#include "../defs.h"
#include "../map/defs.h"

FILE *fpCreateRep;
bool useRep = true;
char repName[1000];

void initReplay() {
    if (_access("replay/", 0) == -1)
        _mkdir("replay/");
    strcpy(repName, ("replay/" + std::string(repName) + ".lgreplay").c_str());
    fpCreateRep = fopen(repName, "w");
    fprintf(fpCreateRep, "%d %d %d %d              \n", R, C, players, 0);
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpCreateRep, "%d ", map[i][j].army);
        fprintf(fpCreateRep, "\n");
    }
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpCreateRep, "%d ", map[i][j].type);
        fprintf(fpCreateRep, "\n");
    }
}

void saveReplay(int turn) {
    fprintf(fpCreateRep, "%d\n", turn);
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpCreateRep, "%d ", map[i][j].army);
        fprintf(fpCreateRep, "\n");
    }
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpCreateRep, "%d ", map[i][j].belong);
        fprintf(fpCreateRep, "\n");
    }
}

void setTurn(int turn) {
    rewind(fpCreateRep);
    fprintf(fpCreateRep, "%d %d %d %d", R, C, players, turn);
}

#endif // __LGEN_REPLAY_CREATE_H