#include "header.h"

void setting_menu() {
	FILE *sett = fopen("setting.txt", "r");
	int diff, col;
	if (sett == NULL) {
		diff = 1;
		col = 0;

		/*
		printf("ERROR");
		exit(1);
		*/
	}
	else {
		fscanf(sett, "%*s %d %*s %d", &diff, &col);
		fclose(sett);
	}

	print_inhdr(0, "Rogue. Game of legends!", 0);
	print_inhdr(2, "Setting", error_delay);
	print_inhdr(4, "Difficulty", error_delay);
	// print_inhdr(6, "", error_delay);
	print_inhdr(14, "Hero Color", error_delay);
	print_inhdr(32, "Back", error_delay);

	int cursor = 1;
	while (TRUE) {
		if (cursor == 1)
			print_in(4, Y / 2 - 10, "-->", 1, 0);
		else if (cursor == 2)
			print_in(14, Y / 2 - 10, "-->", 1, 0);
		else if (cursor == 3)
			print_in(32, Y / 2 - 10, "-->", 1, 0);
		print_all();

		int c = getch();

		if (c == 27) { // Esc
			break;
		}
		else if (c == KDOWN) {
			cursor = (cursor % 3 + 1);
		}
		else if (c == KUP) {
			cursor = ((cursor + 1) % 3 + 1);
		}
		else if (c == '\n') {
			refresh_all();
			if (cursor == 1) {
				int ndif = diff;
				while (true) {
					print_in(7, Y / 2 - 4, "Easy", 1, 0);
					print_in(9, Y / 2 - 4, "Normal", 1, 0);
					print_in(11, Y / 2 - 4, "Hard", 1, 0);
					print_in(7 + ndif * 2, Y / 2 - 10, "-->", 1, 0);
					print_all();
					int c2 = getch();
					if (c == 27) {
						ndif = diff;
						break;
					}
					else if (c2 == KDOWN) {
						ndif = ((ndif + 1) % 3);
					}
					else if (c2 == KUP) {
						ndif = ((ndif + 2) % 3);
					}
					else if (c2 == '\n') {
						break;
					}
					refresh_all();
				}

				diff = ndif;
			}
			else if (cursor == 2) {
				int ncol = col;
				while (true) {
					print_in(17, Y / 2 - 4, "White", 1, 0);
					print_in(19, Y / 2 - 4, "Red", 1, 0);
					print_in(21, Y / 2 - 4, "Green", 1, 0);
					print_in(23, Y / 2 - 4, "Blue", 1, 0);
					print_in(25, Y / 2 - 4, "Magenta", 1, 0);
					print_in(27, Y / 2 - 4, "Cyan", 1, 0);
					print_in(29, Y / 2 - 4, "Yellow", 1, 0);
					print_in(17 + ncol * 2, Y / 2 - 10, "-->", 1, 0);
					print_all();
					int c2 = getch();
					if (c2 == 27) {
						ncol = col;
						break;
					}
					else if (c2 == KDOWN) {
						ncol = ((ncol + 1) % 7);
					}
					else if (c2 == KUP) {
						ncol = ((ncol + 6) % 7);
					}
					else if (c2 == '\n') {
						break;
					}
					refresh_all();
				}

				col = ncol;
			}
			else {
				break;
			}
		}
		refresh_all();

	}
	clear_all();

	sett = fopen("setting.txt", "w");
	fprintf(sett, "Difficulty: %d\nColor: %d", diff, col);
	fclose(sett);
}