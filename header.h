#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <locale.h>

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
	char *name, *first_play, *pass, *email;
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

#define RX 42
#define RY 140
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

typedef struct {
	Coor coor;
	int value;
	int type; // Normal Gold, Black Gold
} Gold;
/*
Floor 1: 500 Gold
Floor 2: 1000 Gold
Floor 3: 1500 Gold
Floor 4: 2000 Gold
*/

typedef struct {
	Coor coor;
	int type;
} Spell, Food;
// Food types: 0: Normal, 1: Supreme, 2: Magic, 3: Rotten
// Spell types: 0: Health(Yellow), 1: Speed(Purple), 2: Power(Red)

typedef struct {
	Coor coor;
	int number, type;
} Weapon;
// Mace, Dagger, Magicw, Arrow, Sword

char* make_map();
char* find_last_map();

#endif

#ifndef INGAME
#define INGAME
#define MAX_HEALTH 100
#define MAX_HUNGER 100

typedef struct {
	Coor coor;
	int floor;
	int health, hunger, gold, point;

	int ssize, fsize;
	int finventory[10], sinventory[10];

	int att[10];
	int satt;
	int mace, dagger, magicw, arrow, sword;
} Player;

void run_game(User *user, char* map_name);

#endif

#ifndef GAMESCREEN
#define GAMESCREEN
// #define Visibility_layer 2

#define CID_MAP 249
#define PORT_CHAR "<"
#define SPORT_CHAR "\u2227"
#define PATH_CHAR "#"
#define DOOR_CHAR "+"
#define INROOM_CHAR "."
#define VWALL_CHAR "|"
#define HWALL_CHAR "_"

#define SPELL_CHAR "\u160F"
#define FOOD_CHAR "\u0DA2"
// #define GOLD_CHAR "\u058E"
#define GOLD_CHAR "\u26C3"
#define BLACK_GOLD_CHAR "\u26C1"

#define MACE_CHAR "\u2692"
#define DAGGER_CHAR "\U0001F5E1"
// #define MAGICWAND_CHAR "\U0001FA84"
#define MAGICWAND_CHAR "\u222E"
#define ARROW_CHAR "\u27B3"
#define SWORD_CHAR "\u2694"
#define AMONGUS_CHAR "\u0D9E"
// Among us u0D9E
// #define FOOD_CHAR "\u1570"

void ginit_screen();
void gend_screen();
void gprint_all();
void food_inventory_empty_massege();
void food_inventory_full_massege();
void spell_inventory_empty_massege();
void spell_inventory_full_massege();
#endif

