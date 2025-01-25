#include "header.h"


extern char map[X][Y];
extern int cons[X][Y], delayed[X][Y];

void init_screen() {
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	cbreak();
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			map[i][j] = ' ';
			cons[i][j] = 1;
		}
	}
}

void end_screen() {
	echo();
	clear();
	endwin();
}

void print_all() {
	for (int i = 0; i < X; i++) {
		if (~i & 1) {
			attron(A_BOLD);
		}
		for (int j = 0; j < Y; j++) {
			if (map[i][j] != ' ' && delayed[i][j]) {
				usleep(delayed[i][j]);
				refresh();
				delayed[i][j] = 0;
			}
			mvaddch(i, j, map[i][j]);
		}
		if (~i & 1) {
			attroff(A_BOLD);
		}
	}
}

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