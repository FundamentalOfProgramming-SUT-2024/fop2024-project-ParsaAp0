#include "header.h"

void print_hero(int number, char* title, int color, User user) {
	attron(color);
	attron(A_BOLD);
	char res[200];
	sprintf(res, "%d: The %s --> %20s  %5d %5d %20s\n", number, title, user.name, user.point, user.gold, user.first_play);
	for (int i = 0; i < strlen(res); i++) {
		usleep(error_delay);
		mvaddch(4 + number * 2, 1 + i, res[i]);
		refresh();
	}
	attroff(color);
	attroff(A_BOLD);
}

void print_noname(int x, char *string) {
	attron(A_BOLD);
	for (int i = 0; i < strlen(string); i++) {
		usleep(error_delay);
		mvaddch(x, i, string[i]);
		refresh();
	}
	attroff(A_BOLD);
}

void print_title(int x, char *string) {
	int y = Y / 2 - strlen(string) / 2;
	int len = strlen(string);
	attron(A_BOLD);
	for (int i = 0; i < len; i++) {
		usleep(error_delay);
		mvaddch(x, y + i, string[i]);
		refresh();
	}
	attroff(A_BOLD);
}

void scoreboard() {
	// GOAT, Legend, Master

	print_title(0, "Rogue. Game of legends!");
	print_title(2, "Scoreboard");

	FILE *fp = fopen("players.txt", "r");

	User **acc = (User **) calloc(1, sizeof(User*));
	acc[0] = (User *) calloc(1, sizeof(User));
	acc[0]->name = (char *) calloc(50, sizeof(char));
	acc[0]->first_play = (char *) calloc(100, sizeof(char));
	int size = 0;
	while (fscanf(fp, "{\n\tname: %s\n\temail: %*s\n\tpassword: %*s\n\tpoint: %d\n\tgold: %d\n\tfirst_play: %s\n}\n", acc[size]->name, &acc[size]->point, &acc[size]->gold, acc[size]->first_play) == 4) {
		size++;
		acc = (User **) realloc(acc, (size + 1) * sizeof(User*));
		acc[size] = (User *) calloc(1, sizeof(User));
		acc[size]->name = (char *) calloc(50, sizeof(char));
		acc[size]->first_play = (char *) calloc(100, sizeof(char));
	}
	//printw("size: %d\n", size);

	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (acc[i]->point < acc[j]->point) {
				User *temp = acc[i];
				acc[i] = acc[j];
				acc[j] = temp;
			}
		}
	}

	print_noname(4, "                                    Name  Point  Gold        Register time\n");
	if (size >= 1) {
		print_hero(1, "GOAT   ", COLOR_PAIR(226), *acc[0]); // Color: Gold
	}
	if (size >= 2) {
		print_hero(2, "Legend ", COLOR_PAIR(247), *acc[1]); // Color: Silver
	}
	if (size >= 3) {
		print_hero(3, "Master ", COLOR_PAIR(130), *acc[2]); // Color: Bronze
	}
	for (int i = 3; i < size; i++) {
		print_hero(i + 1, "Soldier", COLOR_PAIR(15), *acc[i]); // Color: white
	}

	getch();
	clear();
}