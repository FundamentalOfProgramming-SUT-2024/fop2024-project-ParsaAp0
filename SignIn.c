#include "header.h"

extern char normalch[100], numberch[11];

int find_user(char *name, char names[100][50], int sz) {
	for (int i = 0; i < sz; i++) {
		if (strcmp(name, names[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

int correct_password(char *pass, char passes[100][50], int sz) {
	for (int i = 0; i < sz; i++) {
		if (strcmp(pass, passes[i]) == 0) {
			return 1;
		}
	}
	return 0;
}
char *rget_name(int x, int y, char *validch, int min, int max) {
	char *line = (char *) calloc(Y, sizeof(char));
	int size = 0, c;
	while (TRUE) {
		c = getch();
		refresh_all();

		if (c == 27) { // Esc
			line[0] = 1;
			return line;
		}
		else if (c == 10) { // '\n'
			break;
		}
		else if ((c < 0 || c >= 128) && c != 263) {
			invalidch_error();
		}
		else if (c == 263) {
			size--;
			if (size < 0)
				size = 0;

			line[size] = '\0';
		}
		else {
			int valid = 0;
			for (int i = 0; i < strlen(validch); i++) {
				valid |= c == validch[i];
			}
			
			if (valid) {
				line[size] = c;
				size++;

				line[size] = '\0';
			}
			else {
				invalidch_error();
			}
		}
		print_in(x, y - size / 2, line, 1, 0);
		print_all();
	}
	print_in(x, y - size / 2, line, 0, 0);
	return line;
}

char *rget_password(int x, int y, char *validch, int min, int max, int hide) {
	char *line = (char *) calloc(Y, sizeof(char));
	char *sline = (char *) calloc(Y, sizeof(char));
	int size = 0, c;
	while (TRUE) {
		c = getch();
		refresh_all();
		if (c == 27) { // Esc
			line[0] = 1;
			return line;
		}
		else if (c == 10) { // '\n'
			break;
		}
		else if (c == 265) {
			hide = 1 - hide;
		}
		else if ((c < 0 || c >= 128) && c != 263) {
			invalidch_error();
		}
		else if (c == 263) {
			size--;
			if (size < 0)
				size = 0;

			line[size] = '\0';
			sline[size] = '\0';
		}
		else {
			int valid = 0;
			for (int i = 0; i < strlen(validch); i++) {
				valid |= c == validch[i];
			}
			
			if (valid) {
				line[size] = c;
				sline[size] = (hide? '*': c);
				size++;

				line[size] = '\0';
				sline[size] = '\0';
				if (hide == 1)
					print_in(x, y - size / 2, sline, 1, 0);
				else
					print_in(x, y - size / 2, line, 1, 0);
				print_all();
			}
			else {
				invalidch_error();
			}
		}
		if (hide == 1)
			print_in(x, y - size / 2, sline, 1, 0);
		else
			print_in(x, y - size / 2, line, 1, 0);
		print_all();
	}
	if (hide == 1)
		print_in(x, y - size / 2, sline, 0, 0);
	else
		print_in(x, y - size / 2, line, 0, 0);
	return line;
}

User* sign_in() {
	FILE *pls = fopen("players.txt", "r");
	int sz = 0;
	char names[100][50], passes[100][50];
	while (pls != NULL && fscanf(pls, "{\n\tname: %s\n\temail: %*s\n\tpassword: %s\n\tpoint: %*s\n\tgold: %*s\n\tregister_second: %*d\n\twins: %*d\n}\n", names[sz], passes[sz]) == 2) {
		sz++;
	}
	if (pls != NULL) fclose(pls);

	print_inhdr(0, "Rogue. Game of legends!", 0);
	print_inhdr(2, "Sign In", head_delay);
	print_inhdr(4, "What's your name?", head_delay);
	print_all();
	
	char *name = rget_name(5, Y / 2, normalch, 5, 20);
	if (name[0] == 1) {
		clear_all();
		User *user = calloc(1, sizeof(User));
		user->name = calloc(1, sizeof(char));
		user->name[0] = 0;
		return user;
	}
	while (find_user(name, names, sz) == 0) {
		usernamenotfound_error();
		clear_all_row(5);
		print_all();

		name = rget_name(5, Y / 2, normalch, 5, 20);
		if (name[0] == 1) {
			clear_all();
			User *user = calloc(1, sizeof(User));
			user->name = calloc(1, sizeof(char));
			user->name[0] = 0;
			return user;
		}
	}

	print_inhdr(6, "What's your password?", head_delay);
	print_inhdr(8, "Press F1 to hide/reveal your password.", head_delay);
	print_all();
	char *password = rget_password(7, Y / 2, normalch, 7, 30, 1);
	if (password[0] == 1) {
		User *user = calloc(1, sizeof(User));
		user->name = calloc(1, sizeof(char));
		user->name[0] = 0;
		return user;
	}
	while (correct_password(password, passes, sz) == 0) {
		notcorrectpass_error();
		clear_all_row(7);
		print_all();
		password = rget_password(7, Y / 2, normalch, 7, 30, 1);
		if (password[0] == 1) {
			User *user = calloc(1, sizeof(User));
			user->name = calloc(1, sizeof(char));
			user->name[0] = 0;
			return user;
		}
	}

	print_in(X - 1, 0, "The sign in process is completed successfully..", 1, error_delay); // It don't print the last '.'.
	print_all();
	usleep(success_delay);
	
	clear_all();

	User *res = calloc(1, sizeof(User));
	pls = fopen("players.txt", "r");
	char rname[30];
	int rgold, rpoint, rregs, rwins;
	while (pls != NULL && fscanf(pls, "{\n\tname: %s\n\temail: %*s\n\tpassword: %*s\n\tpoint: %d\n\tgold: %d\n\tregister_second: %d\n\twins: %d\n}\n", rname, &rpoint, &rgold, &rregs, &rwins) == 5) {
		if (strcmp(name, rname) == 0) {
			res->name = calloc(30, sizeof(char));
			strcpy(res->name, rname);
			res->wins = rwins;
			res->point = rpoint;
			res->regs = rregs;
			res->gold = rgold;
			break;
		}
	}
	fclose(pls);
	return res;
}