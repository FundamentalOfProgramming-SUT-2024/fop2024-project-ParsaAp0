#include "header.h"

int pregame_menu() {
    print_inhdr(0, "Rogue. Game of legends!", 0);
    print_inhdr(2, "New Game", error_delay);
    print_inhdr(4, "Resume Game", error_delay);
    print_inhdr(6, "Settings", error_delay);
    print_inhdr(8, "Scoreboard", error_delay);
    print_inhdr(10, "Back (Log out)", error_delay);
    print_all();

    int cursor = 1;
    while (TRUE) {
        print_in(cursor * 2, Y / 2 - 10, "-->", 1, 0);
        print_all();
        int c = getch();

        if (c == KDOWN) {
            cursor += cursor != 5;
        }
        else if (c == KUP) {
            cursor -= cursor != 1;
        }
        else if (c == '\n') {
            break;
        }
        refresh_all();

    }
    clear_all();
    return cursor;
}