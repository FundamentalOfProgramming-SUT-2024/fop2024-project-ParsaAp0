#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "header.h"

char normalch[100] = {
    32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 0
};
char numberch[11] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 0
};

void invalidch_error() {
    print_in(X - 1, 0, "Invalid character", 1, error_delay);
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

int is_new_user(char *line) {
    FILE *players = fopen("players.txt", "r");
    if (players == NULL)
        return 1;
    char name[100], email[100], password[100];
    while (fscanf(players, "{\n\tname: %s\n\temail: %*s\n\tpassword: %*s\n}\n", name) == 1) {
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
        if (c == 10) { // '\n'
            if (size < min) {
                tooshort_error(min);
            }
            else if (max < size) {
                toolong_error(max);
            }
            else {
                if (is_new_user(line)) {
                    break;
                }
                else {
                    alreadyexist_error();
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
        if (c == 10) { // '\n'
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

char *get_password(int x, int y, char *validch, int min, int max, int hide) {
    char *line = (char *) calloc(Y, sizeof(char));
    char *sline = (char *) calloc(Y, sizeof(char));
    int size = 0, c;
    while (TRUE) {
        c = getch();
        refresh_all();
        if (c == 10) { // '\n'
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
                print_in(x, y - size / 2, sline, 1, 0);
                print_all();
            }
            else {
                invalidch_error();
            }
        }
        print_in(x, y - size / 2, sline, 1, 0);
        print_all();
    }
    print_in(x, y - size / 2, sline, 0, 0);
    return line;
}

void sign_up() {
    print_in(0, Y / 2 - 11, "Rogue. Game of legends!", 0, head_delay);
    print_in(2, Y / 2 - 4, "Sign Up", 0, 50000);

    print_in(4, Y / 2 - 12, "What should we call you?", 0, head_delay);
    print_all();
    char *name = get_name(5, Y / 2, normalch, 5, 20);

    print_in(6, Y / 2 - 13, "What is your email address?", 0, head_delay);
    print_all();
    char *email = get_email(7, Y / 2, normalch);

    print_in(8, Y / 2 - 11, "Type a strong password!", 0, head_delay);
    print_all();
    char *password = get_password(9, Y / 2, normalch, 7, 30, 1);

    FILE *fptr = fopen("players.txt", "a");
    fprintf(fptr, "{\n\tname: %s\n\temail: %s\n\tpassword: %s\n}\n", name, email, password);
    fclose(fptr);
}
/*
int main() {
    init_screen();

    // FILE *players = fopen("players.txt", "r");
    // if (players == NULL) {
    //    fclose(players);
        sign_up();
    // }
    // fclose(players);
    



    //getch();
    end_screen();
    // move(1, 60);
    
}
// */