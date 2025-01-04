#include "header.h"

int main() {
    init_screen();

    int first_time = true;
    while (true) {
        User user;
        int result = start_menu(first_time * head_delay);
        first_time = false;
        if (result == 1 || result == 2) {
            if (result == 1) {
                user = sign_in();
            }
            else {
                user.name = "GUEST MODE";
            }

            while (true) {
                int res = pregame_menu();
                if (res == 1) {
                    // New Game
                }
                else if (res == 2) {
                    // Resume Game
                }
                else if (res == 3) {
                    // Settings
                }
                else if (res == 4) {
                    // Scoreboard
                }
                else {
                    // Back
                }
            }
        }
        else if (result == 3) {
            sign_up();
        }
        else if (result == 4) {
            break;
        }
    }



    end_screen();
    return 0;
}
