#include "header.h"

int main() {
    // User Parsa = {"PARSA"};
    init_screen();

    while (true) {
        User user;
        int result = start_menu();
        if (result == 1 || result == 2) {
            if (result == 1) {
                user = sign_in();
            }
            else {
                user.name = "GUEST MODE";
            }

            int res = pregame_menu();
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
