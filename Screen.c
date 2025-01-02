#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "header.h"

// unsigned int atts[X][Y][10];
char map[X][Y];
int cons[X][Y], delayed[X][Y];

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