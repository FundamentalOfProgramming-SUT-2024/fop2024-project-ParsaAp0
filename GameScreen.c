#include "header.h"

extern char lmap[FLOOR_NUMBER][5][X][Y];
extern int lcons[FLOOR_NUMBER][5][X][Y], ldelayed[FLOOR_NUMBER][5][X][Y], att[FLOOR_NUMBER][5][X][Y][10], satt[FLOOR_NUMBER][5][X][Y];
extern int Visibility_power[FLOOR_NUMBER];

void gend_screen() {
	echo();
	clear();
	endwin();
}

void ginit_screen() {
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	cbreak();
	//init_color(COLOR_WHITE, 700, 700, 700);
	//init_color(COLOR_BLACK, 200, 200, 200);
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	attron(COLOR_PAIR(214));
}

void gprint_all(int f) {
	
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			int c = ' ';
			for (int k = 4; k >= 0; k--) {
				if (lmap[f][k][i][j] && (k != Visibility_layer || Visibility_power[f] == false)) {
					c = lmap[f][k][i][j];
					for (int q = 0; q < satt[f][k][i][j]; q++) {
						attron(att[f][k][i][j][q]);
					}
					break;
				}
			}
			mvaddch(i, j, c);
			for (int k = 4; k >= 0; k--) {
				if (lmap[f][k][i][j] && (k != Visibility_layer || Visibility_power[f] == false)) {
					for (int q = 0; q < satt[f][k][i][j]; q++) {
						attroff(att[f][k][i][j][q]);
					}
					break;
				}
			}
			/*
			if (map[i][j] != ' ' && delayed[i][j]) {
				usleep(delayed[i][j]);
				refresh();
				delayed[i][j] = 0;
			}
			mvaddch(i, j, map[i][j]);
			*/
		}
	}
}
/*
void refresh_all() {
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			if (cons[i][j]) {
				map[i][j] = ' ';
			}
			delayed[i][j] = 0;
		}
	}
	clear();
	print_all();
}

void print_in(int x, int y, char *string, int con, int delay) {
	int nx = x, ny = y, len = strlen(string);
	for (int i = 0; i < len; i++) {
		//mvaddch(nx, ny, string[i]);
		delayed[nx][ny] = delay;
		map[nx][ny] = string[i];
		cons[nx][ny] = con;
		ny++;
	}
}

void print_inhdr(int x, char *string, int delay) {
	print_in(x, Y / 2 - strlen(string) / 2, string, 0, delay);
}

void clear_all() {
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			map[i][j] = ' ';
			delayed[i][j] = 0;
			cons[i][j] = 0;
		}
	}
	clear();
	refresh();
}

void clear_all_row(int i) {
	for (int j = 0; j < Y; j++) {
		map[i][j] = ' ';
		delayed[i][j] = 0;
		cons[i][j] = 0;
	}
	clear();
	refresh();
}

void invalidch_error() {
	print_in(X - 1, 0, "Invalid character", 1, error_delay);
}

void usernamenotfound_error() {
	print_in(X - 1, 0, "Wrong username! Try again!", 1, error_delay);
}

void notcorrectpass_error() {
	print_in(X - 1, 0, "Wrong password! Try again!", 1, error_delay);
}

void tooshort_error(int lim) {
	char mess[50] = "Too short!! at least ";
	sprintf(mess + strlen(mess), "%d", lim);
	strcat(mess, " characters are needed.");
	print_in(X - 1, 0, mess, 1, error_delay);
}

void toolong_error(int lim) {
	char mess[50] = "Too long!! at most ";
	sprintf(mess + strlen(mess), "%d", lim);
	strcat(mess, " characters are needed.");
	print_in(X - 1, 0, mess, 1, error_delay);
}

void alreadyexist_error() {
	print_in(X - 1, 0, "This name already exist! Try again!", 1, error_delay);
}

void notvalidemail_error() {
	print_in(X - 1, 0, "The email format is not correct. It should be like \"xx@yy.zz", 1, error_delay);
}

void notvalidpassword_error() {
	print_in(X - 1, 0, "Password should contains at least one digit, one capital letter and one small letter", 1, error_delay);
}
*/