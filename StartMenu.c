#include "header.h"

int start_menu(int delay) {
	print_inhdr(0, "Rogue. Game of legends!", delay);
	print_inhdr(2, "Sign in", delay);
	print_inhdr(4, "Guest mode", delay);
	print_inhdr(6, "Sign up", delay);
	print_inhdr(8, "Exit", delay);
	print_all();

	int cursor = 1;
	while (TRUE) {
		print_in(cursor * 2, Y / 2 - 10, "-->", 1, 0);
		print_all();
		int c = getch();

		if (c == 27) { // Esc
			cursor = 4;
			break;
		}
		else if (c == KDOWN) {
			cursor = (cursor % 4 + 1);
		}
		else if (c == KUP) {
			cursor = (cursor + 2) % 4 + 1;
		}
		else if (c == '\n') {
			break;
		}
		refresh_all();

	}
	clear_all();
	return cursor;
}