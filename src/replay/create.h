#ifndef __LGEN_REPLAY_CREATE_H
#define __LGEN_REPLAY_CREATE_H

#include "../defs.h"
#include "../map/defs.h"

FILE *fpReplay;
bool useRep = true;
char repName[1000];

void initReplay() {
    if (_access("replay/", 0) == -1)
        _mkdir("replay/");
    strcpy(repName, ("replay/" + std::string(repName) + ".lgreplay").c_str());
    fpReplay = fopen(repName, "w");
    fprintf(fpReplay, "%d %d\n", R, C);
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpReplay, "%d ", map[i][j].army);
        fprintf(fpReplay, "\n");
    }
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpReplay, "%d ", map[i][j].type);
        fprintf(fpReplay, "\n");
    }
    fprintf(fpReplay, "\n");
}

void saveReplay() {
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpReplay, "%d ", map[i][j].army);
        fprintf(fpReplay, "\n");
    }
    for (int i = 1; i <= R; i++) {
        for (int j = 1; j <= C; j++)
            fprintf(fpReplay, "%d ", map[i][j].belong);
        fprintf(fpReplay, "\n");
    }
    fprintf(fpReplay, "\n");
}

#endif // __LGEN_REPLAY_CREATE_H