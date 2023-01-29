#ifndef __LGEN_REPLAY_LOAD_H
#define __LGEN_REPLAY_LOAD_H

#include "../defs.h"
#include "../map/defs.h"
#include <vector>
using std::vector;

int allt, nowt = 2, savet, lstt, autoPlay = 0, spd, eta;
FILE *fpLoadRp;
char loadRpName[1000];
vector<vector<block>> rep[200001];

inline void printRpMsg() {
    gotoxy(R + 1, 1);
    printf("Turn %d%c %.3lf%%", nowt / 2, (nowt % 2? '.' : ' '), 100.0 * nowt / allt);
    clearline();
    gotoxy(R + 2, 1);
    printf("Team    Land  Army");
    struct node {int army, land, team;} p[20];
    for (int i = 1; i <= players; i++)
        p[i].army = p[i].land = 0, p[i].team = i;
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++) {
            p[map[i][j].belong].army += map[i][j].army;
            p[map[i][j].belong].land++;
        }
    std::sort(p + 1, p + players + 1, [](node a, node b){return a.army > b.army;});
    for (int i = 1; i <= players; i++) {
        int t = p[i].team;
        setfcolor(team[t].color);
        gotoxy(R + i + 2, 1);
        clearline();
        printf("%s", team[t].name.c_str());
        gotoxy(R + i + 2, 9);
        printf("%d", p[i].land);
        gotoxy(R + i + 2, 15);
        printf("%d", p[i].army);
    }
}

inline void loadMap(const vector<vector<block>> &mp) {
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            map[i][j].army = mp[i][j].army, map[i][j].belong = mp[i][j].belong;
}

inline void printProgress() {
    double progress = 100.0 * allt / savet;
    gotoxy(3, 1);
    putchar('[');
    for (int i = 1; i <= int(progress); i++)
        putchar('#');
    for (int i = 1; i <= 100 - int(progress); i++)
        putchar(' ');
    putchar(']');
    printf(" %.3lf%%\n", progress);
    gotoxy(4, 1);
    printf("%d turns/s", spd / 2);
    if (eta) printf(" ETA %dm%ds", eta / 60, eta % 60);
    clearline();
}

inline void loadReplay() {
    printf("Input replay name: ");
    scanf("%s", loadRpName);
    fpLoadRp = fopen(("replay/" + std::string(loadRpName) + ".lgreplay").c_str(), "r");
    printf("Loaded replay %s\n", ("replay/" + std::string(loadRpName) + ".lgreplay").c_str());
    printf("Press any key to continue... ");
    getch(); clearall();
    fscanf(fpLoadRp, "%d %d %d %d", &R, &C, &players, &savet);
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            fscanf(fpLoadRp, "%d", &map[i][j].army);
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            fscanf(fpLoadRp, "%d", &map[i][j].type);
    printf("Loading replay, it will take a while...");
    std::chrono::nanoseconds lst = std::chrono::steady_clock::now().time_since_epoch();
    setvbuf(stdout, nullptr, _IOFBF, 5000000);
    while (fscanf(fpLoadRp, "%d", &allt) != EOF) {
        gotoxy(2, 1);
        printf("Loading turn %d%c", allt / 2, (allt % 2? '.' : ' '));
        if (savet) printProgress();
        fflush(stdout);
        vector<vector<block>> &rp = rep[allt];
        rp.resize(R + 1);
        for (int i = 1; i <= R; i++)
            rp[i].resize(C + 1);
        for (int i = 1; i <= R; i++)
            for (int j = 1; j <= C; j++)
                fscanf(fpLoadRp, "%d", &rp[i][j].army);
        for (int i = 1; i <= R; i++)
            for (int j = 1; j <= C; j++)
                fscanf(fpLoadRp, "%d", &rp[i][j].belong);
        if (std::chrono::steady_clock::now().time_since_epoch() - lst < std::chrono::milliseconds(1000))
            continue;
        spd = allt - lstt;
        if (savet) eta = (savet - allt) / spd;
        lstt = allt;
        lst = std::chrono::steady_clock::now().time_since_epoch();
    }
    clearall();
    int input = 0;
    while (1) {
        gotoxy(1, 1);
        printMap(0, {0, 0}, 1);
        printRpMsg();
        fflush(stdout);
        if (_kbhit()) {
            input = getch();
            switch (input) {
                case 'q':
                    return;
                case 'a':
                    autoPlay ^= 1;
                    break;
                case 224: {
                    int tmp = getch();
                    switch (tmp) {
                        case 75: if (nowt > 2) nowt--; break;
                        case 77: if (nowt < allt) nowt++; break;
                    }
                }
            }
        }
        if (autoPlay && nowt < allt)
            nowt++; 
        loadMap(rep[nowt]);
    }
}

#endif // __LGEN_REPLAY_LOAD_H