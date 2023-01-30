#ifndef __LGEN_CONFIG_H
#define __LGEN_CONFIG_H

#include "defs.h"
#include "map/create.h"
#include "replay/create.h"

FILE *fpConfig;

void readConfig(char *filename) {
    fpConfig = fopen(("config/" + std::string(filename) + ".lgconfig").c_str(), "r");
    fscanf(fpConfig, "%s\n", mapName);
    fscanf(fpConfig, "%d %d %d\n", &delay, &players, &cheatCode);
    fscanf(fpConfig, "%d", &useRep);
    if (useRep) fscanf(fpConfig, "%s", repName);
    if (strcmp(mapName, "__STANDARD__") == 0) createStandardMap();
    else readMap(mapName);
}

void writeConfig(char *filename) {
    if (_access("config/", 0) == -1)
        _mkdir("config/");
    fpConfig = fopen(("config/" + std::string(filename) + ".lgconfig").c_str(), "w");
    fprintf(fpConfig, "%s\n", mapName);
    fprintf(fpConfig, "%d %d %d\n", delay, players, cheatCode);
    fprintf(fpConfig, "%d\n", useRep);
    if (useRep) fprintf(fpConfig, "%s\n", repName);
}

#endif // __LGEN_CONFIG_H