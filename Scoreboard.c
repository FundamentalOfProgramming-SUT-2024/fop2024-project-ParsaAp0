#include "header.h"
#include <time.h>

void print_hero(int number, char* title, int color, User user, int bold, int ntime) {
	attron(color);
	attron(A_BOLD);
	bold && attron(A_UNDERLINE);
	char res[200];
	if (number <= 3) {
		int diff = ntime - user.regs, sec = diff % 60, min = diff / 60 % 60, hour = diff / 3600 % 24, day = diff / 3600 / 24;
		sprintf(res, "  The %s --> %20s  %5d %5d                                      %3d, %02d:%02d:%02d       %d\n", title, user.name, user.point, user.gold, day, hour, min, sec, user.wins);
		if (number == 1)
			mvprintw(4 + number * 2, 0, "\U0001f947");
		else if (number == 2)
			mvprintw(4 + number * 2, 0, "\U0001f948");
		else
			mvprintw(4 + number * 2, 0, "\U0001f949");
		for (int i = 0; i < strlen(res); i++) {
			usleep(error_delay);
			mvaddch(4 + number * 2, 2 + i, res[i]);
			refresh();
		}
	}
	else {
		int diff = ntime - user.regs, sec = diff % 60, min = diff / 60 % 60, hour = diff / 3600 % 24, day = diff / 3600 / 24;
		sprintf(res, "%d: The %s --> %20s  %5d %5d                                      %3d, %02d:%02d:%02d       %d\n", number, title, user.name, user.point, user.gold, day, hour, min, sec, user.wins);
		for (int i = 0; i < strlen(res); i++) {
			usleep(error_delay);
			mvaddch(4 + number * 2, 1 + i, res[i]);
			refresh();
		}
	}
	bold && attroff(A_UNDERLINE);
	attroff(A_BOLD);
	attroff(color);
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

void scoreboard(User user) {
	// GOAT, Legend, Master

	print_title(0, "Rogue. Game of legends!");
	print_title(2, "Scoreboard");

	FILE *fp = fopen("players.txt", "r");

	User **acc = (User **) calloc(1, sizeof(User*));
	acc[0] = (User *) calloc(1, sizeof(User));
	acc[0]->name = (char *) calloc(50, sizeof(char));
	// acc[0]->first_play = (char *) calloc(100, sizeof(char));
	int size = 0;
	while (fscanf(fp, "{\n\tname: %s\n\temail: %*s\n\tpassword: %*s\n\tpoint: %d\n\tgold: %d\n\tregister_second: %d\n\twins: %d\n}\n", acc[size]->name, &acc[size]->point, &acc[size]->gold, &acc[size]->regs, &acc[size]->wins) == 5) {
		size++;
		acc = (User **) realloc(acc, (size + 1) * sizeof(User*));
		acc[size] = (User *) calloc(1, sizeof(User));
		acc[size]->name = (char *) calloc(50, sizeof(char));
		// acc[size]->first_play = (char *) calloc(100, sizeof(char));
	}
	

	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (acc[i]->point < acc[j]->point) {
				User *temp = acc[i];
				acc[i] = acc[j];
				acc[j] = temp;
			}
		}
	}

	int ntime = time(NULL);
	print_noname(4, "                                    Name  Point  Gold        Register time (Days, Hours:Minutes:Seconds)    Wins\n");
	if (size >= 1) {
		print_hero(1, "GOAT   ", COLOR_PAIR(226), *acc[0], !strcmp(user.name, acc[0]->name), ntime); // Color: Gold
	}
	if (size >= 2) {
		print_hero(2, "Legend ", COLOR_PAIR(247), *acc[1], !strcmp(user.name, acc[1]->name), ntime); // Color: Silver
	}
	if (size >= 3) {
		print_hero(3, "Master ", COLOR_PAIR(130), *acc[2], !strcmp(user.name, acc[2]->name), ntime); // Color: Bronze
	}
	for (int i = 3; i < size; i++) {
		print_hero(i + 1, "Soldier", COLOR_PAIR(15), *acc[i], !strcmp(user.name, acc[i]->name), ntime); // Color: white
	}

	getch();
	clear();
}