#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "header.h"

int start_menu() {
    print_inhdr(0, "Rogue. Game of legends!", head_delay);
    // print_in(0, Y / 2 - 11, "Rogue. Game of legends!", 0, head_delay);
    print_inhdr(2, "Sign in", head_delay);
    // print_in(2, Y / 2 - 3, "Sign in", 0, head_delay);
    print_inhdr(4, "Guest mode", head_delay);
    // print_in(4, Y / 2 - 5, "Guest mode", 0, head_delay);
    print_inhdr(6, "Sign up", head_delay);
    // print_in(6, Y / 2 - 3, "Sign up", 0, head_delay);
    print_inhdr(8, "Exit", head_delay);
    // print_in(8, Y / 2 - 2, "Exit", 0, head_delay);
    print_all();

    int cursor = 2;
    while (TRUE) {
        print_in(cursor, Y / 2 - 10, "-->", 1, 0);
        print_all();
        int c = getch();

        if (c == 258) {
            // Down
            if (cursor == 8) continue;

            cursor += 2;
        }
        else if (c == 259) {
            // Up
            if (cursor == 2) continue;
            cursor -= 2;
        }
        else if (c == '\n') {
            break;
        }
        refresh_all();

    }
    clear_all();
    return cursor / 2;
    /*
    1 -> Sign in
    2 -> Sign up
    3 -> Exit
    */
}