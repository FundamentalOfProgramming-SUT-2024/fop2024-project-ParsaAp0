#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#define forever while (TRUE)
#define X 45
#define Y 184
#define error_delay 10000
#define head_delay 50000

unsigned int atts[X][Y][10];
char map[X][Y];
int cons[X][Y], delayed[X][Y];
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

void init_screen() {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            map[i][j] = ' ';
            cons[i][j] = 1;
        }
    }
    //attron(A_BLINK);
}

void end_screen() {
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

void print_in(int x, int y, char *string, int dela, int delay) {
    int nx = x, ny = y, len = strlen(string);
    for (int i = 0; i < len; i++) {
        //mvaddch(nx, ny, string[i]);
        delayed[nx][ny] = delay;
        map[nx][ny] = string[i];
        cons[nx][ny] = dela;
        ny++;
    }
}

void invalidch_error() {
    print_in(X - 1, 0, "Invalid character", 1, error_delay);
//    print_all();
}

void tooshort_error(int lim) {
    char mess[50] = "Too short!! at least ";
    sprintf(mess + strlen(mess), "%d", lim);
    strcat(mess, " characters are needed.");
    print_in(X - 1, 0, mess, 1, error_delay);
//    print_all();
}

void toolong_error(int lim) {
    char mess[50] = "Too long!! at most ";
    sprintf(mess + strlen(mess), "%d", lim);
    strcat(mess, " characters are needed.");
    print_in(X - 1, 0, mess, 1, error_delay);
//    print_all();

}

char *get_line(int x, int y, char *validch, int min, int max, int hide) {
    move(x, y);
    //char name[Y];
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
            else break;
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

		// attron(A_BOLD);
		// attroff(A_BOLD);
    }
    print_in(x, y - size / 2, sline, 0, 0);
    return line;
}

void sign_up() {
    print_in(0, Y / 2 - 11, "Rogue. Game of legends!", 0, head_delay);
    print_in(2, Y / 2 - 3, "Sign Up", 0, 50000);

    print_in(4, Y / 2 - 11, "What should we call you?", 0, head_delay);
    print_all();
//    print_all();
    char *name = get_line(5, Y / 2, normalch, 5, 20, 0);

    print_in(6, Y / 2 - 7, "How old are you?", 0, head_delay);
    print_all();
    char *age = get_line(7, Y / 2, numberch, 1, 2, 0);

    print_in(8, Y / 2 - 11, "Type a strong password!", 0, head_delay);
    print_all();
    char *password = get_line(9, Y / 2, normalch, 8, 30, 1);

    FILE *fptr = fopen("players.txt", "a");
    fprintf(fptr, "{\n\tname: %s\n\tage: %s\n\tpassword: %s\n}\n", name, age, password);
    fclose(fptr);

}

int main() {
    init_screen();

    FILE *players = fopen("players.txt", "r");
    if (players == NULL) {
        sign_up();
    }


    //getch();
    end_screen();
    // move(1, 60);
    
}
