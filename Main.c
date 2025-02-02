#include "header.h"

int main() {
	init_screen();

	int first_time = true;
	while (true) {
		User* user;
		int result = start_menu(first_time * head_delay);
		first_time = false;
		if (result == 1 || result == 2) {
			if (result == 1) {
				user = sign_in();
				if (user->name[0] == 0) {
					continue;
				}
			}
			else {
				user = calloc(1, sizeof(User));
				user->name = calloc(30, sizeof(char));
				strcpy(user->name, "GUEST MODE");
				user->first_play = calloc(50, sizeof(char));
				strcpy(user->first_play, "0.0.0,00:00:00");
				user->gold = 0;
				user->point = 0;
			}

			while (true) {
				int res = pregame_menu();
				if (res == 1) {
					// New Game
					char* map_name = make_map();
					end_screen();
					run_game(user, map_name);
					init_screen();
				} 
				else if (res == 2) {
					// Resume Game
					char* map_name = find_last_map();
					if (map_name[0] == 0) {
						nosavedgame_error();
						continue;
					}
					end_screen();
					run_game(user, map_name);
					init_screen();
				}
				else if (res == 3) {
					// Settings
					setting_menu();
				}
				else if (res == 4) {
					// Scoreboard
					scoreboard(*user);
				}
				else {
					// Back
					break;
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
