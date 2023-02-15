#ifndef __LGEN_REPLAY_LOAD_H
#define __LGEN_REPLAY_LOAD_H

#include "../defs.h"
#include "../map/defs.h"
#include <vector>
using std::vector;

int allt, nowt = 2, savet, lstt, jumpt = 1;
int replayMoveCnt = 0, autoPlay = 0, spd, eta;
int replayMoves[20];
defMove tempMoves[20];
FILE *fpLoadRp;
char loadRpName[1000];
vector<vector<vector<block>>> rep;

inline void printRpMsg() {
    double progress = 100.0 * nowt / allt;
    gotoxy(R + 1, 1);
    printf("Turn %d%c %.3lf%% | Jump Turn = %d\033[K\n", nowt / 2, (nowt % 2? '.' : ' '), progress, jumpt / 2);
    putchar('[');
    for (int i = 1; i <= int(progress); i++)
        putchar('#');
    for (int i = 1; i <= 100 - int(progress); i++)
        putchar(' ');
    putchar(']');
    clearline();
    gotoxy(R + 3, 1);
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
        gotoxy(R + i + 3, 1);
        clearline();
        printf("%s", team[t].name.c_str());
        gotoxy(R + i + 3, 9);
        printf("%d", p[i].land);
        gotoxy(R + i + 3, 15);
        printf("%d", p[i].army);
    }
}

inline void loadMap(const vector<vector<block>> &mp) {
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++) {
            map[i][j].army = mp[i][j].army;
            map[i][j].belong = mp[i][j].belong;
            map[i][j].type = mp[i][j].type;
        }
}

inline void printProgress() {
    double progress = 100.0 * allt / savet;
    gotoxy(4, 1);
    putchar('[');
    for (int i = 1; i <= int(progress); i++)
        putchar('#');
    for (int i = 1; i <= 100 - int(progress); i++)
        putchar(' ');
    putchar(']');
    printf(" %.3lf%%\n", progress);
    gotoxy(5, 1);
    printf("%d turns/s", spd / 2);
    if (eta) printf(" ETA %dm%ds", eta / 60, eta % 60);
    clearline();
}

inline int makeMove(int id, int mv, defPlayer& pos) {
    switch (mv) {
        case -1:
            break;
        case 0:
            pos = gens[id];
            break;
        case 1 ... 4: {
            defPlayer npos = {pos.x + dx[mv], pos.y + dy[mv]};
            if (npos.x < 1 || npos.x > R || npos.y < 1 || npos.y > C || map[npos.x][npos.y].type == 2)
                return 1;
            tempMoves[++replayMoveCnt] = {id, pos, npos};
            pos = npos;
            break;
        }
        case 5 ... 8: {
            defPlayer npos = {pos.x + dx[mv - 4], pos.y + dy[mv - 4]};
            if (npos.x < 1 || npos.x > R || npos.y < 1 || npos.y > C)
                return 1;
            pos = npos;
            break;
        }
        default:
            return -1;
    }
    return 0;
}

inline void generateKill(int x, int p1, int p2) {
    isAlive[p2] = 0;
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++) {
            if (rep[x][i][j].belong == p2 && rep[x][i][j].type != 3) {
                rep[x][i][j].belong = p1;
                rep[x][i][j].army = (rep[x][i][j].army + 1) >> 1;
            }
        }
}

inline void generate(int x) {
    for (int i = 1; i <= replayMoveCnt; i++) {
        defMove p = tempMoves[i];
        if (!isAlive[p.id]) continue;
        if (rep[x][p.from.x][p.from.y].belong != p.id) continue;
        int leftArmy = 1;
        if (rep[x][p.from.x][p.from.y].type == 3 && turn > 150)
            leftArmy = std::max(1LL, rep[x][p.from.x][p.from.y].army >> 1);
        if (rep[x][p.to.x][p.to.y].belong == p.id) {
            rep[x][p.to.x][p.to.y].army += rep[x][p.from.x][p.from.y].army - leftArmy;
            rep[x][p.from.x][p.from.y].army = leftArmy;
        } else {
            rep[x][p.to.x][p.to.y].army -= rep[x][p.from.x][p.from.y].army - leftArmy;
            rep[x][p.from.x][p.from.y].army = leftArmy;
            if (rep[x][p.to.x][p.to.y].army < 0) {
                rep[x][p.to.x][p.to.y].army = -rep[x][p.to.x][p.to.y].army;
                int t = rep[x][p.to.x][p.to.y].belong;
                rep[x][p.to.x][p.to.y].belong = p.id;
                if (rep[x][p.to.x][p.to.y].type == 3) {
                    generateKill(x, p.id, t);
                    rep[x][p.to.x][p.to.y].type = 4;
                    for (int j = i + 1; j <= replayMoveCnt; j++)
                        if (tempMoves[j].id == t)
                            tempMoves[j].id = p.id;
                }
            }
        }
    }
}

inline void update(int x) {
    rep[x] = rep[x - 1];
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++) {
            if (rep[x][i][j].belong == 0)
                continue;
            switch (rep[x][i][j].type) {
                case 0: /* plain */
                    if (x % 50 == 0)
                        rep[x][i][j].army++;
                    break;
                case 1: /* swamp */
                    if (x % 2 == 0 && rep[x][i][j].army > 0)
                        if (!(--rep[x][i][j].army))
                            rep[x][i][j].belong = 0;
                    break;
                case 3: /* general */
                    if (x % 2 == 0)
                        rep[x][i][j].army++;
                    break;
                case 4: /* city */
                    if (x % 2 == 0)
                        rep[x][i][j].army++;
                    break;
            }
        }
}

inline void loadReplay() {
    printf("Input replay name: ");
inputRpName:
    scanf("%s", loadRpName);
    if (_access(("replay/" + std::string(loadRpName) + ".lgreplay").c_str(), 0) == -1) {
        gotoxy(2, 1);
        clearline();
        printf("You don't have this replay, please enter a valid replay name");
        gotoxy(1, 20);
        clearline();
        goto inputRpName;
    }
    fpLoadRp = fopen(("replay/" + std::string(loadRpName) + ".lgreplay").c_str(), "rb");
    clearline();
    printf("Loading replay %s\n", ("replay/" + std::string(loadRpName) + ".lgreplay").c_str());
    printf("Press any key to continue... ");
    getch();
    binread(fpLoadRp, &R);
    binread(fpLoadRp, &C);
    binread(fpLoadRp, &players);
    binread(fpLoadRp, &savet);
    if (savet) rep.resize(savet + 1);
    for (int i = 1; i <= players; i++) {
        binread(fpLoadRp, &gens[i].x);
        binread(fpLoadRp, &gens[i].y);
        pos[i] = gens[i];
    }
    if (!savet) rep.resize(2);
    rep[1].resize(R + 1);
    for (int i = 1; i <= R; i++)
        rep[1][i].resize(C + 1);
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            binread(fpLoadRp, &rep[1][i][j].army, 8);
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            binread(fpLoadRp, &rep[1][i][j].type);
    for (int i = 1; i <= players; i++)
        rep[1][gens[i].x][gens[i].y].belong = i, isAlive[i] = 1;
    std::chrono::nanoseconds lst = nowTime;
    setvbuf(stdout, nullptr, _IOFBF, 5000000);
    while (binread(fpLoadRp, &allt)) {
        gotoxy(3, 1);
        clearline();
        printf("Loading turn %d%c", allt / 2, (allt % 2? '.' : ' '));
        if (savet) printProgress();
        fflush(stdout);
        if (!savet) rep.resize(allt + 1);
        vector<vector<block>> &rp = rep[allt];
        rp.resize(R + 1);
        for (int i = 1; i <= R; i++)
            rp[i].resize(C + 1);
        replayMoveCnt = 0;
        for (int i = 1; i <= players; i++) {
            binread(fpLoadRp, &replayMoves[i]);
            makeMove(i, replayMoves[i], pos[i]);
        }
        update(allt);
        generate(allt);
        if (nowTime - lst < std::chrono::milliseconds(1000))
            continue;
        spd = allt - lstt;
        if (savet) eta = (savet - allt) / spd;
        lstt = allt;
        lst = nowTime;
    }
    clearall();
    setFontSize(IN_GAME_FONT_SIZE);
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
                case 'j':
                    gotoxy(R + 1, 6);
                    clearline();
                    resetattr();
                    fflush(stdout);
                    scanf("%d", &nowt);
                    nowt <<= 1;
                    nowt = std::min(nowt, allt);
                    nowt = std::max(nowt, 2);
                    break;
                case 'c':
                    resetattr();
                    gotoxy(R + 1, 1);
                    printf("Turn %d%c %.3lf%% | Jump Turn = \033[K", nowt / 2, (nowt % 2? '.' : ' '), 100.0 * nowt / allt);
                    fflush(stdout);
                    scanf("%d", &jumpt);
                    if (jumpt == 0) jumpt = 1;
                    else jumpt *= 2;
                    break;
                case 224: {
                    int tmp = getch();
                    switch (tmp) {
                        case 75:
                            if (nowt - jumpt >= 2) nowt -= jumpt;
                            else nowt = 2;
                            break;
                        case 77:
                            if (nowt + jumpt <= allt) nowt += jumpt;
                            else nowt = allt;
                            break;
                    }
                }
            }
        }
        if (nowt == allt && autoPlay) autoPlay = 0;
        if (autoPlay) nowt++; 
        loadMap(rep[nowt]);
    }
}

#endif // __LGEN_REPLAY_LOAD_H
