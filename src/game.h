#ifndef __LGEN_GAME_H
#define __LGEN_GAME_H

#include "defs.h"
#include "map/defs.h"

int turn, msgCnt;
bool gameEnd, isAlive[20];
defMessage msg[205];
defPlayer gens[20], pos[20];
std::deque<defMove> todoMove;

inline void addMessage(int turn, int id, std::string message) {
    ++msgCnt;
    msg[msgCnt].turn = turn;
    msg[msgCnt].id = id;
    msg[msgCnt].msg = message;
}

inline void initGen() {
    std::deque<defPlayer> tmp;
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            if (map[i][j].type == 3)
                tmp.push_back({i, j});
    while (tmp.size() < players) {
        int x, y;
        do x = random(1, R), y = random(1, C); while (map[x][y].type != 0);
        tmp.push_back({x, y});
        map[x][y].type = 3;
        map[x][y].army = 0;
    }
    std::shuffle(tmp.begin(), tmp.end(), mtrand);
    for (int i = 1; i <= players; i++) {
        pos[i] = gens[i] = tmp[i - 1];
        map[gens[i].x][gens[i].y].belong = i;
        map[gens[i].x][gens[i].y].army = 0;
    }
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++)
            if (map[i][j].type == 3 && map[i][j].belong == 0)
                map[i][j].type = 0;
}

void updateMap() {
    turn++;
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++) {
            if (map[i][j].belong == 0)
                continue;
            switch (map[i][j].type) {
                case 0: /* plain */
                    if (turn % 25 == 0)
                        map[i][j].army++;
                    break;
                case 1: /* swamp */
                    if (map[i][j].army > 0)
                        if (!(--map[i][j].army))
                            map[i][j].belong = 0;
                    break;
                case 3: /* general */
                    map[i][j].army++;
                    break;
                case 4: /* city */
                    map[i][j].army++;
                    break;
            }
        }
}

inline int move(int id, int mv, defPlayer& pos) {
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
            todoMove.push_back({id, pos, npos});
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

void kill(int p1, int p2) {
    if (p2 == 1)
        MessageBoxA(nullptr, ("YOU ARE KILLED BY PLAYER " + team[p1].name + " AT TURN " + std::to_string(turn) + ".").c_str(), "", MB_OK);
    isAlive[p2] = 0;
    for (int i = 1; i <= R; i++)
        for (int j = 1; j <= C; j++) {
            if (map[i][j].belong == p2 && map[i][j].type != 3) {
                map[i][j].belong = p1;
                map[i][j].army = (map[i][j].army + 1) >> 1;
            }
        }
    int col = team[p2].color;
    addMessage(turn, p1, ("KILLED PLAYER \033[38;2;" + std::to_string(col / 65536) + ";" + std::to_string(col / 256 % 256) + ";" + std::to_string(col % 256) + "m" + team[p2].name + "."));
    fflush(stdout);
}

void flushMove() {
    while (!todoMove.empty()) {
        defMove p = todoMove.front();
        todoMove.pop_front();
        if (!isAlive[p.id]) continue;
        if (map[p.from.x][p.from.y].belong != p.id) continue;
        if (map[p.to.x][p.to.y].belong == p.id) {
            map[p.to.x][p.to.y].army += map[p.from.x][p.from.y].army - 1;
            map[p.from.x][p.from.y].army = 1;
        } else {
            map[p.to.x][p.to.y].army -= map[p.from.x][p.from.y].army - 1;
            map[p.from.x][p.from.y].army = 1;
            if (map[p.to.x][p.to.y].army < 0) {
                map[p.to.x][p.to.y].army = -map[p.to.x][p.to.y].army;
                int t = map[p.to.x][p.to.y].belong;
                map[p.to.x][p.to.y].belong = p.id;
                if (map[p.to.x][p.to.y].type == 3) {
                    kill(p.id, t);
                    map[p.to.x][p.to.y].type = 4;
                    for (auto &mv: todoMove)
                        if (mv.id == t)
                            mv.id = p.id;
                }
            }
        }
    }
}

inline void runGame() {
    for (int i = 1; i <= players; i++)
        isAlive[i] = true;
    initGen();
    updateMap();
    printMap(cheatCode, gens[1]);
    std::deque<int> moves;
    std::chrono::nanoseconds lst = std::chrono::steady_clock::now().time_since_epoch();
    while (1) {
        if (_kbhit()) {
            int ch = _getch();
            switch(ch = tolower(ch)) {
                case int(' '): while (getch() != ' '); break;
                case int('c'): clearall(); break;
                case int('w'): moves.emplace_back(1); break;
                case int('a'): moves.emplace_back(2); break;
                case int('s'): moves.emplace_back(3); break;
                case int('d'): moves.emplace_back(4); break;
                case 224:
                    ch = getch();
                    switch (ch) {
                        case 72: moves.emplace_back(5); break;
                        case 75: moves.emplace_back(6); break;
                        case 80: moves.emplace_back(7); break;
                        case 77: moves.emplace_back(8); break;
                    }
                    break;
                case int('g'): moves.emplace_back(0); break;
                case int('e'): if (!moves.empty()) moves.pop_back(); break;
                case int('q'): moves.clear(); break;
                case 27:
                    MessageBoxA(nullptr, "YOU QUIT THE GAME.", "EXIT", MB_OK);
                    return;
                case int('\b'):
                    if (!isAlive[1]) break;
                    int confirmSur = MessageBoxA(nullptr, "ARE YOU SURE TO SURRENDER?", "CONFIRM SURRENDER", MB_YESNO);
                    if (confirmSur == 7) break;
                    isAlive[1] = 0;
                    for (int i = 1; i <= R; ++i) {
                        for (int j = 1; j <= C; ++j) {
                            if (map[i][j].belong == 1) {
                                map[i][j].belong = 0;
                                if (map[i][j].type == 3)
                                    map[i][j].type = 4;
                            }
                        }
                    }
                    addMessage(turn, 1, "SURRENDERED.");
                    break;
            }
        }
        if (std::chrono::steady_clock::now().time_since_epoch() - lst < std::chrono::milliseconds(delay))
            continue;
        updateMap();
        while (!moves.empty() && move(1, moves.front(), pos[1]))
            moves.pop_front();
        if (!moves.empty())
            moves.pop_front();
        flushMove();
        if (cheatCode != ((1 << players) - 1) << 1) {
            int allDead = 0;
            for (int i = 1; i <= players && !allDead; i++) {
                if (cheatCode & (1 << i))
                    if (isAlive[i])
                        allDead = 1;
            }
            if (!allDead) {
                cheatCode = ((1 << players) - 1) << 1;
                MessageBoxA(nullptr, "ALL THE PLAYERS YOU SELECTED TO BE SEEN IS DEAD.\nTHE OVERALL CHEAT MODE WILL BE SWITCHED ON.", "TIP", MB_OK);
            }
        }
        if (!gameEnd) {
            int ed = 0;
            for(int i = 1; i <= players; i++)
                ed |= (isAlive[i] << i);
            if (__builtin_popcount(ed) == 1) {
                MessageBoxA(nullptr, ("PLAYER " + team[std::__lg(ed)].name + " WON!" + "\n" + "THE GAME WILL CONTINUE." + "\n" + "YOU CAN PRESS [ESC] TO EXIT.").c_str(), "GAME END", MB_OK);
                gameEnd = 1;
                cheatCode = ((1 << players) - 1) << 1;
                addMessage(turn, std::__lg(ed), "WON!");
            }
        }
        gotoxy(1, 1);
        printMap(cheatCode, pos[1]);
        gotoxy(R + 1, 1);
        printf("Turn %d\n", turn);
        // ranklist(pos);
        // printGameMessage();
        fflush(stdout);
        lst = std::chrono::steady_clock::now().time_since_epoch();
    }
}

#endif // __LGEN_GAME_H