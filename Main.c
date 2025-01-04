#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "header.h"
#include <stdio.h>


typedef struct {
    char *name;

} User;

int main() {
    User Parsa = {"PARSA"};
    init_screen();
    
    FILE *players = fopen("players.txt", "r");
    if (players == NULL) {
        sign_up();
    }
    else {
        fclose(players);
    }

    while (true) {
        int result = start_menu();
        if (result == 1) {
            sign_in();
        }
        else if (result == 2) {
            sign_up();
        }
        else if (result == 3) {
            break;
        }
    }



    end_screen();
    return 1;
}
