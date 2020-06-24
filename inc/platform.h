#pragma once

// windows macro
#ifdef _WIN32
#define IN_WINDOWS
#endif

// linux macro
#ifdef __linux__
#define IN_LINUX
#endif

#ifdef linux
#define IN_LINUX
#endif

// determine wrong
#if defined(IN_LINUX) && defined(IN_WINDOWS)
#error Detected on two different platforms
#endif
// platform determine end

void clean_screen();

void get_screen_size(int& row, int& col);

// origin row = 1 col = 1
void set_cursor_position(int row = 1, int col = 1);

// key macro
#define NO_INPUT -1
#define CTRL_C 3
#define CTRL_D 4
#define ENTER 10
#define ESC 27
#define CHAR_U_CAP 'W'
#define CHAR_D_CAP 'S'
#define CHAR_L_CAP 'A'
#define CHAR_R_CAP 'D'
#define CHAR_U 'w'
#define CHAR_D 's'
#define CHAR_L 'a'
#define CHAR_R 'd'
#define ARROW_U 301
#define ARROW_D 302
#define ARROW_L 304
#define ARROW_R 303

#define IS_QUIT(i) (i == ESC || i == CTRL_C || i == CTRL_D)
#define IS_U(i) (i == CHAR_U || i == CHAR_U_CAP || i == ARROW_U)
#define IS_D(i) (i == CHAR_D || i == CHAR_D_CAP || i == ARROW_D)
#define IS_L(i) (i == CHAR_L || i == CHAR_L_CAP || i == ARROW_L)
#define IS_R(i) (i == CHAR_R || i == CHAR_R_CAP || i == ARROW_R)

class Keyboard {
  public:
    Keyboard();
    ~Keyboard();
    int get();    // Keyboard::get() 不处理end_flag 仅供内部调用
    int get_blocking();
    void clean_buffer();
};
extern Keyboard keyboard;
// 为true时退出程序 按esc ctrl^c ctrl^d时触发
extern bool end_flag;

// color macro
#define CLR_RESET 0

#define CLR_BLACK 1
#define CLR_RED 2
#define CLR_GREEN 3
#define CLR_YELLOW 4
#define CLR_BLUE 5
#define CLR_MAGENTA 6
#define CLR_CYAN 7
#define CLR_WHITE 8

#define CLR_ON_BLACK 9
#define CLR_ON_RED 10
#define CLR_ON_GREEN 11
#define CLR_ON_YELLOW 12
#define CLR_ON_BLUE 13
#define CLR_ON_MAGENTA 14
#define CLR_ON_CYAN 15
#define CLR_ON_WHITE 16

void setcolor(const int&);

void resetcolor();