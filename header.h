#ifndef MAIN
#define MAIN
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#endif

#ifndef SCREEN
#define SCREEN

#include <ncurses.h>
#define X 45
#define Y 184
extern char map[X][Y];
extern int cons[X][Y], delayed[X][Y];
extern char normalch[100], numberch[11];


void init_screen();
void end_screen();
void print_all();
void print_in(int x, int y, char *string, int con, int delay);
void refresh_all();
void clear_all();
void clear_all_row(int x);
void invalidch_error();
void usernamenotfound_error();
void notcorrectpass_error();
void tooshort_error(int lim);
void toolong_error(int lim);
void alreadyexist_error();
void notvalidemail_error();
void notvalidpassword_error();

#endif

#ifndef SIGNUP
#define SIGNUP

#define error_delay 10000
#define head_delay 50000
void sign_up();

#endif

#ifndef SIGNIN
#define SIGNIN
void sign_in();

#endif

#ifndef START_MENU
#define START_MENU

int start_menu();

#endif
