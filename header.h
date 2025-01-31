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

#ifndef MENUSCREEN
#define MENUSCREEN


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
void reserved_error();
void notvalidemail_error();
void notvalidpassword_error();
void nosavedgame_error();

#endif

#ifndef SIGNUP
#define SIGNUP

void sign_up();

#endif

#ifndef SIGNIN
#define SIGNIN

User* sign_in();

#endif

#ifndef START_MENU
#define START_MENU

int start_menu();

#endif

#ifndef PREGAME_MENU
#define PREGAME_MENU

int pregame_menu();

#endif

#ifndef SETTING_MENU
#define SETTING_MENU

void setting_menu();

#endif

#ifndef SCOREBOARD
#define SCOREBOARD

void scoreboard();

#endif

#ifndef CREATEMAP
#define CREATEMAP

#define RX 43
#define RY 150
#define ROOM_NUMBER 9
#define PATH_NUMBER 12
#define FLOOR_NUMBER 4

typedef struct {
	int x, y;
} Coor;

typedef struct {
	Coor *coor;
	int type; // Type 0: Normal, Type 1: Enchant, Type 2: Treasure, Type 3: Nightmare
	int hidden;
//	int *coor;
} Room;

typedef struct {
	int r1, r2, size;
	Coor *coor;
} Path;

char* make_map();
char* find_last_map();

#endif

#ifndef INGAME
#define INGAME

typedef struct {
	Coor coor;
	int floor;
	int health;
	int att[10];
	int satt;

} Player;

void run_game(User user, char* map_name);

#endif

#ifndef GAMESCREEN
#define GAMESCREEN
// #define Visibility_layer 2

#define CID_MAP 249
#define PLAYER_CHAR "P"
#define PORT_CHAR "<"
#define SPORT_CHAR "^"
#define PATH_CHAR "#"
#define DOOR_CHAR "+"
#define INROOM_CHAR "."
#define VWALL_CHAR "|"
#define HWALL_CHAR "_"

void ginit_screen();
void gend_screen();
void gprint_all();
/*
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
*/
#endif

