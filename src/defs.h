#ifndef __LGEN_DEFS_H
#define __LGEN_DEFS_H

#include <cstdio>
#include <string>
#include <windows.h>
#include <conio.h>

#define ll long long

// Console functions

void getxy(int& x, int& y) {
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bInfo);
	y = bInfo.dwCursorPosition.X;
	x = bInfo.dwCursorPosition.Y;
}

inline void clearline() { fputs("\033[K", stdout); }
inline void gotoxy(int x, int y) { printf("\033[%d;%dH", x, y); }
inline void curup   (int c = 1) { printf("\033[%dA", c); }
inline void curdown (int c = 1) { printf("\033[%dB", c); }
inline void curright(int c = 1) { printf("\033[%dC", c); }
inline void curleft (int c = 1) { printf("\033[%dD", c); }

inline void hideCursor() { fputs("\033[?25l", stdout); }
inline void showCursor() { fputs("\033[?25h", stdout); }

inline void setfcolor(int RGB) { printf("\033[38;2;%d;%d;%dm", RGB / 65536, RGB / 256 % 256, RGB % 256); }
inline void setbcolor(int RGB) { printf("\033[48;2;%d;%d;%dm", RGB / 65536, RGB / 256 % 256, RGB % 256); }
inline void setfcolor(int R, int G, int B) { printf("\033[38;2;%d;%d;%dm", R, G, B); }
inline void setbcolor(int R, int G, int B) { printf("\033[48;2;%d;%d;%dm", R, G, B); }

inline int initattr() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hOut == INVALID_HANDLE_VALUE)
		return GetLastError();
	DWORD dwMode = 0;
	if(!GetConsoleMode(hOut, &dwMode))
		return GetLastError();
	dwMode |= 0x0004;
	if(!SetConsoleMode(hOut, dwMode))
		return GetLastError();
	return 0;
}

inline void resetattr() { fputs("\033[0m\033[38;2;255;255;255m", stdout); }

// Defines

struct block {
    int belong, type;
    bool visible;
    ll army;
};

int R, C;
const int MAX_SIZE = 500;
block map[MAX_SIZE + 5][MAX_SIZE + 5];

struct defTeam {
    std::string name;
    int color;
};

defTeam team[64] = {
	{"White", 0xffffff},
	{"Red", 0xff0000},
	{"Green", 0x00ff00},
	{"Blue", 0x0000ff},
	{"Yellow", 0xffff00},
	{"Cyan", 0x00ffff},
	{"Rose", 0xff00ff},
	{"Orange", 0xff8000},
	{"Lime", 0x80ff00},
	{"Brown", 0x804000},
	{"Grey", 0x808080},
	{"Teal", 0x008080},
	{"Purple", 0xc000c0},
	{"Silver", 0xc0c0c0},
	{"Maroon", 0xc00000},
	{"Emerald", 0x00ce80},
	{"Olive", 0x808000},
};

struct defPlayer {
    int x, y;
};

const char suf[20] = {0, 'H', 'K', 'W', 'L', 'M', 'Q', 'I', 'G', 'B', 'N', 'T'};

#endif // __LGEN_DEFS_H