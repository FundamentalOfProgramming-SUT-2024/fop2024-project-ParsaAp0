#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

#define error_delay 10000
#define head_delay 50000
#define success_delay 1400000
#define X 45
#define Y 184
#define KDOWN 258
#define KUP 259
#define KLEFT 260
#define KRIGHT 261

typedef struct {
    char *name, *first_play;
    int gold, point;


} User;

#ifndef MAIN
#define MAIN



#endif

#ifndef SCREEN
#define SCREEN


void init_screen();
void end_screen();
void print_all();
void print_in(int x, int y, char *string, int con, int delay);
void print_inhdr(int x, char *string, int delay);
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

void sign_up();

#endif

#ifndef SIGNIN
#define SIGNIN

User sign_in();

#endif

#ifndef START_MENU
#define START_MENU

int start_menu();

#endif

#ifndef PREGAME_MENU
#define PREGAME_MENU

int pregame_menu();

#endif