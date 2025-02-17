#include "header.h"
#include <time.h>

extern char normalch[100], numberch[11];

int is_new_user(char *line) {
	FILE *players = fopen("players.txt", "r");
	if (players == NULL)
		return 1;
	char name[100], email[100], password[100];
	while (fscanf(players, "{\n\tname: %s\n\temail: %*s\n\tpassword: %*s\n\tpoint: %*d\n\tgold: %*d\n\tregister_second: %*d\n\twins: %*d\n}\n", name) == 1) {
		if (strcmp(line, name) == 0) {
			fclose(players);
			return 0;
		}
	}
	fclose(players);
	return 1;
}

char *get_name(int x, int y, char *validch, int min, int max) {
	char *line = (char *) calloc(Y, sizeof(char));
	int size = 0, c;
	while (TRUE) {
		c = getch();
		refresh_all();
		if (c == 27) { // Esc
			line[0] = 0;
			return line;
		}
		else if (c == 10) { // '\n'
			if (size < min) {
				tooshort_error(min);
			}
			else if (max < size) {
				toolong_error(max);
			}
			else {
				if (!is_new_user(line)) {
					alreadyexist_error();
				}
				else if (strcmp(line, "GUEST MODE") == 0) {
					reserved_error();
				}
				else {
					break;
				}
			}
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

int is_correct_email(char *line) {
	char c = '@';
	int ht = 0, cid = 0, len = strlen(line);
	for (int i = 0; i < len; i++) {
		if (c == line[i]) {
			ht++;
			cid = i;
		}
	}
	if (ht != 1 || cid == 0)
		return 0;
	
	char c2 = '.';
	int c2id = -1;
	for (int i = len - 1; i > cid; i--) {
		if (c2 == line[i]) {
			c2id = i;
			break;
		}
	}
	if (c2id == -1 || c2id - cid == 1 || c2id == len - 1)
		return 0;

	return 1;
}

char *get_email(int x, int y, char *validch) {
	char *line = (char *) calloc(Y, sizeof(char));
	int size = 0, c;
	while (TRUE) {
		c = getch();
		refresh_all();
		if (c == 27) { // Esc
			line[0] = 0;
			return line;
		}
		else if (c == 10) { // '\n'
			if (is_correct_email(line)) {
				break;
			}
			else {
				notvalidemail_error();
			}
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

int valid_password(char *line) {
	int len = strlen(line);
	int capc = 0, smallc = 0, digc = 0;
	for (int i = 0; i < len; i++) {
		capc |= ('A' <= line[i] && line[i] <= 'Z');
		smallc |= ('a' <= line[i] && line[i] <= 'z');
		digc |= ('0' <= line[i] && line[i] <= '9');
	}
	return capc && smallc && digc;
}

void get_random_password(char *line, int min, int max) {
	int sz = min + rand() % (max - min + 1);
	line[sz] = '\0';
	for (int i = 0; i < sz; i++) {
		int x = rand() % (26 + 26 + 10);
		if (x < 26) {
			line[i] = 'A' + x;
		}
		else if (x < 52) {
			line[i] = 'a' + x - 26;
		}
		else {
			line[i] = '0' + x - 52;
		}
	}
}

char *get_password(int x, int y, char *validch, int min, int max, int hide) {
	char *line = (char *) calloc(Y, sizeof(char));
	char *sline = (char *) calloc(Y, sizeof(char));
	int size = 0, c;
	while (TRUE) {
		c = getch();
		refresh_all();
		if (c == 27) { // Esc
			line[0] = 0;
			return line;
		}
		else if (c == 265) {
			hide = 1 - hide;
		}
		else if (c == 266) {
			get_random_password(line, min, max);
			size = strlen(line);
			for (int i = 0; i < size; i++) {
				sline[i] = '*';
			}
			sline[size] = '\0';
		}
		else if (c == 10) { // '\n'
			if (size < min) {
				tooshort_error(min);
			}
			else if (max < size) {
				toolong_error(max);
			}
			else {
				if (valid_password(line)) {
					break;
				}
				else {
					notvalidpassword_error();
				}
			}
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

void sign_up() {
	print_inhdr(0, "Rogue. Game of legends!", 0);
	print_inhdr(2, "Sign Up", head_delay);
	// print_in(X - 3, 0, "Back [Esc]", 0, head_delay);

	print_inhdr(4, "What should we call you?", head_delay);
	print_all();
	char *name = get_name(5, Y / 2, normalch, 5, 20);
	if (strlen(name) == 0) {
		clear_all();
		return;
	}

	print_inhdr(6, "What is your email address?", head_delay);
	print_all();
	char *email = get_email(7, Y / 2, normalch);
	if (strlen(email) == 0) {
		clear_all();
		return;
	}

	print_inhdr(8, "Type a strong password!", head_delay);
	print_inhdr(11, "Press F1 to hide/reveal your password.", head_delay);
	print_inhdr(12, "Press F2 to make a random password.", head_delay);

	print_all();
	char *password = get_password(9, Y / 2, normalch, 7, 30, 1);
	if (strlen(password) == 0) {
		clear_all();
		return;
	}

	// time_t time_raw_format;
    // char timestring[50];
    // time(&time_raw_format);
    // if(strftime(timestring,50,"%Y.%d.%m,%H:%M:%S",localtime(&time_raw_format)) == 0){
    //     perror("Couldn't prepare formatted string");
    // }

	print_in(X - 1, 0, "The sign up process is completed successfully....", 1, error_delay);
	print_all();
	usleep(success_delay);
	

	FILE *fptr = fopen("players.txt", "a");
	fprintf(fptr, "{\n\tname: %s\n\temail: %s\n\tpassword: %s\n\tpoint: 0\n\tgold: 0\n\tregister_second: %lld\n\twins: 0\n}\n", name, email, password, (long long int) time(NULL));
	fclose(fptr);


	clear_all();
}