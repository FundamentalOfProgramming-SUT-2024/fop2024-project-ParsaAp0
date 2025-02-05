#include "header.h"

static char *game_name = "Rogue. Game of legends!", *title = "Setting";
static char *difficulty = "Difficulty", *hero_color = "Hero Color", *song = "Song", *back = "Back";
static char *diff[3] = {"Easy", "Normal", "Hard"};
static char *col[7] = {"White", "Red", "Green", "Blue", "Magenta", "Cyan", "Yellow"};
static char *son[4] = {"All of The Freedoms - Kohta Yamamoto", "Footsteps of Doom - Kohta Yamamoto", "MAN-Child - Kohta Yamamoto", "None"};

void print_pret(int x, int y, char *string, int delay) {
	for (int i = 0; i < strlen(string); i++) {
		mvaddch(x, y + i - strlen(string) / 2, string[i]);
		if (delay) {
			usleep(delay);
			refresh();
		}
	}
}

void focus_on(int b) {
	if (b) {
		attron(COLOR_PAIR(15));
		// attron(A_BOLD);
	}
	else {
		attron(COLOR_PAIR(244));
	}
}
void focus_off(int b) {
	if (b) {
		attroff(COLOR_PAIR(15));
		// attroff(A_BOLD);
	}
	else {
		attroff(COLOR_PAIR(244));
	}
}

void cfocus_on(int b) {
	if (b) {
		attron(A_UNDERLINE);
	}
	else {
	}
}
void cfocus_off(int b) {
	if (b) {
		attroff(A_UNDERLINE);
	}
	else {
	}
}

void sfocus_on(int b) {
	if (b) {
		attron(A_BOLD);
	}
	else {
	}
}
void sfocus_off(int b) {
	if (b) {
		attroff(A_BOLD);
	}
	else {
	}
}

void print_window(int cursor, int selected, int *cs, int delay) {
	clear();
	attron(COLOR_PAIR(15));
	attron(A_BOLD);
	print_pret(0, Y / 2, game_name, delay);
	print_pret(2, Y / 2, title, delay);
	attroff(A_BOLD);

	focus_on(cursor == 0);
	sfocus_on(selected == 0);
	print_pret(4, 20, difficulty, delay);
	for (int i = 0; i < 3; i++) {
		cfocus_on(cs[0] == i);
		print_pret(6 + 2 * i, 20, diff[i], delay);
		cfocus_off(cs[0] == i);
	}
	focus_off(cursor == 0);
	sfocus_off(selected == 0);

	focus_on(cursor == 1);
	sfocus_on(selected == 1);
	print_pret(4, 65, hero_color, delay);
	for (int i = 0; i < 7; i++) {
		cfocus_on(cs[1] == i);
		print_pret(6 + 2 * i, 65, col[i], delay);
		cfocus_off(cs[1] == i);
	}
	focus_off(cursor == 1);
	sfocus_off(selected == 1);
	
	focus_on(cursor == 2);
	sfocus_on(selected == 2);
	print_pret(4, 115, song, delay);
	for (int i = 0; i < 4; i++) {
		cfocus_on(cs[2] == i);
		print_pret(6 + 2 * i, 115, son[i], delay);
		cfocus_off(cs[2] == i);
	}	
	focus_off(cursor == 2);
	sfocus_off(selected == 2);

	focus_on(cursor == 3);
	sfocus_on(selected == 3);
	print_pret(4, 165, back, delay);
	focus_off(cursor == 3);
	sfocus_off(selected == 3);


	mvprintw(X - 1, Y - 1, " ");
}

void setting_menu() {
	clear();
	FILE *sett = fopen("setting.txt", "r");
	int Diff, Color, Song;
	if (sett == NULL) {
		Diff = 1;
		Color = 0;
		Song = 3;
	}
	else {
		fscanf(sett, "%*s %d %*s %d %*s %d", &Diff, &Color, &Song);
		fclose(sett);
	}
	int cursor = 0, selected = -1;
	int cs[3] = {Diff, Color, Song};

	print_window(cursor, selected, cs, error_delay);
	while (1) {
		int c = getch();

		if (selected == -1) {
			if (c == KRIGHT) {
				cursor = (cursor + 1) % 4;
			}
			else if (c == KLEFT) {
				cursor = (cursor + 3) % 4;
			}
			else if (c == '\n') {
				selected = cursor;
				if (selected == 3) {
					break;
				}
			}
			else if (c == 27) {
				break;
			}
		}
		else if (selected == 0) {
			if (c == KDOWN) {
				cs[selected] = (cs[selected] + 1) % 3;
			}
			else if (c == KUP) {
				cs[selected] = (cs[selected] + 2) % 3;
			}
			else if (c == 27) {
				break;
			}
			else if (c == '\n') {
				selected = -1;
			}
		}
		else if (selected == 1) {
			if (c == KDOWN) {
				cs[selected] = (cs[selected] + 1) % 7;
			}
			else if (c == KUP) {
				cs[selected] = (cs[selected] + 6) % 7;
			}
			else if (c == 27) {
				break;
			}
			else if (c == '\n') {
				selected = -1;
			}
		}
		else if (selected == 2) {
			if (c == KDOWN) {
				cs[selected] = (cs[selected] + 1) % 4;
			}
			else if (c == KUP) {
				cs[selected] = (cs[selected] + 3) % 4;
			}
			else if (c == 27) {
				break;
			}
			else if (c == '\n') {
				selected = -1;
			}
		}
		else if (selected == 3) {
			break;
		}

		print_window(cursor, selected, cs, 0);
	}

	Diff = cs[0];
	Color = cs[1];
	Song = cs[2];
	sett = fopen("setting.txt", "w");
	fprintf(sett, "Difficulty: %d\nColor: %d\nSong: %d", Diff, Color, Song);
	fclose(sett);

	clear();
	return;
}