#ifndef MAIN
#define MAIN
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// #include "SignUp.c"
// #include "Screen.c"

#endif

#ifndef SCREEN
#define SCREEN

#include <ncurses.h>
#define X 45
#define Y 184
extern char map[X][Y];
extern int cons[X][Y], delayed[X][Y];


void init_screen();
void end_screen();
void print_all();
void print_in(int x, int y, char *string, int con, int delay);
void refresh_all();
void clear_all();

#endif

#ifndef SIGNUP
#define SIGNUP

#define error_delay 10000
#define head_delay 50000
void sign_up();

#endif
