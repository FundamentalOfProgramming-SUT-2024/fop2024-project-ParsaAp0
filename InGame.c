#include "header.h"
#include <stdio.h>
#define printf(...) printf(__VA_ARGS__); fflush(stdout)
#define MAX_VISIBILITY 5

/*
#include <stdio.h>
#define printf(...) printf(__VA_ARGS__), fflush(stdout)
*/



Player player;
char PLAYER_CHAR[10] = "\u1403";

Room grooms[FLOOR_NUMBER][ROOM_NUMBER];
Path gpaths[FLOOR_NUMBER][PATH_NUMBER];
Coor inports[FLOOR_NUMBER], outports[FLOOR_NUMBER];
int psize[FLOOR_NUMBER];

int Visibility_power;
int visibility[FLOOR_NUMBER][X][Y];
int floor_seen[FLOOR_NUMBER];

int golds[FLOOR_NUMBER], foods[FLOOR_NUMBER], spells[FLOOR_NUMBER], weapons[FLOOR_NUMBER];
Gold gold[FLOOR_NUMBER][1000];
Food food[FLOOR_NUMBER][1000];
Spell spell[FLOOR_NUMBER][1000];
Weapon weapon[FLOOR_NUMBER][1000];

int power_boost = 1;
int speed_boost = 1;

/*
	Layer 0: Base of the map: rooms, paths, traps, ...
	Layer 1: Player and enemies
	Layer 2: visibility of the map
	Layer 3: Notifications and messages.

*/

void save_user(User x) {
	User users[1000];
	FILE *f = fopen("players.txt", "r");
	int sz = 0;
	users[0].first_play = calloc(50, sizeof(char));
	users[0].name = calloc(30, sizeof(char));
	users[0].pass = calloc(30, sizeof(char));
	users[0].email = calloc(50, sizeof(char));

	while (f != NULL && fscanf(f, "%*s %*s %s %*s %s %*s %s %*s %d %*s %d %*s %s %*s", users[sz].name, users[sz].email, users[sz].pass, &users[sz].point, &users[sz].gold, users[sz].first_play) == 6) {
		if (strcmp(users[sz].name, x.name) == 0) {
			users[sz].gold = x.gold;
			users[sz].point = x.point;
		}
		sz++;
		users[sz].first_play = calloc(50, sizeof(char));
		users[sz].name = calloc(30, sizeof(char));
		users[sz].pass = calloc(30, sizeof(char));
		users[sz].email = calloc(50, sizeof(char));
	}
	fclose(f);
	f = fopen("players.txt", "w");
	for (int i = 0; i < sz; i++) {
		fprintf(f, "{\n\tname: %s\n\temail: %s\n\tpassword: %s\n\tpoint: %d\n\tgold: %d\n\tfirst_play: %s\n}\n", users[i].name, users[i].email, users[i].pass, users[i].point, users[i].gold, users[i].first_play);
	}
	fclose(f);
}

int inside_room(Coor c, int f) {
	int room_id = -1;
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (grooms[f][i].coor[0].x <= c.x && c.x <= grooms[f][i].coor[1].x && grooms[f][i].coor[0].y <= c.y && c.y <= grooms[f][i].coor[1].y) {
			room_id = i;
			break;
		}
	}

	return room_id;
}

static void reset(int k) {
	for (int i = 0; i < ROOM_NUMBER; i++) {
		free(grooms[k][i].coor);
		grooms[k][i].hidden = 0;
		grooms[k][i].type = 0;
	}
	for (int i = 0; i < psize[k]; i++) {
		free(gpaths[k][i].coor);
		gpaths[k][i].r1 = gpaths[k][i].r2 = gpaths[k][i].size = 0;
	}
	psize[k] = 0;
	golds[k] = foods[k] = spells[k] = weapons[k] = 0;
}

static void reset_all() {
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		reset(k);
	}
}

void decrease_hunger(int amount) {
	player.hunger -= amount;
	if (player.hunger < 0) {
		player.health += player.hunger;
		player.hunger = 0;
	}
}

void init_map(FILE *fmap) {
	FILE *fset = fopen("setting.txt", "r");
	int difficulty, player_color;
	if (fset == NULL) {
		difficulty = 1;
		player_color = 0;
	}
	else {
		fscanf(fset, "%*s %d %*s %d", &difficulty, &player_color);
		fclose(fset);
	}


	fscanf(fmap, "%*s %*s %d %d %d", &player.floor, &player.coor.x, &player.coor.y); // Floor, x, y
	fscanf(fmap, "%*s %*s %d %d %d %d", &player.health, &player.hunger, &player.fsize, &player.ssize); // health, hunger, fsize, ssize;
	fscanf(fmap, "%*s %*s");
	for (int i = 0; i < player.fsize; i++) {
		fscanf(fmap, "%d ", player.finventory + i);
	}
	fscanf(fmap, "%*s %*s");
	for (int i = 0; i < player.ssize; i++) {
		fscanf(fmap, "%d ", player.sinventory + i);
	}

	fscanf(fmap, "%*s %d %d %d %d %d\n", &player.mace, &player.dagger, &player.magicw, &player.arrow, &player.sword);
	player.att[player.satt++] = A_BOLD;

	// Colors: "White", "Red", "Green", "Blue", "Magenta", "Cyan", "Yellow"
	int colid[7] = {7, 1, 2, 4, 163, 14, 220};
	player.att[player.satt++] = COLOR_PAIR(colid[player_color]);
	// Reading rooms and paths.
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		fscanf(fmap, "%*s");
		for (int j = 0; j < ROOM_NUMBER; j++) {
			grooms[k][j].coor = (Coor *) calloc(2, sizeof(Coor));
			fscanf(fmap, "%d %d %d %d %d %d", &grooms[k][j].coor[0].x, &grooms[k][j].coor[0].y, &grooms[k][j].coor[1].x, &grooms[k][j].coor[1].y, &grooms[k][j].type, &grooms[k][j].hidden);
		}
		fscanf(fmap, "%*s %*s %d\nPaths:", &psize[k]);
		
		for (int j = 0; j < psize[k]; j++) {
			fscanf(fmap, "%*s %d%*s %*s %d%*s %*s %d%*s %*s", &gpaths[k][j].r1, &gpaths[k][j].r2, &gpaths[k][j].size);
			gpaths[k][j].coor = (Coor *) calloc(gpaths[k][j].size, sizeof(Coor));
			for (int q = 0; q < gpaths[k][j].size; q++) {
				fscanf(fmap, "%d %d", &gpaths[k][j].coor[q].x, &gpaths[k][j].coor[q].y);
			}
		}
		fscanf(fmap, "%*s %*s %d %d", &inports[k].x, &inports[k].y);
		fscanf(fmap, "%*s %*s %d %d", &outports[k].x, &outports[k].y);

		fscanf(fmap, "%*s %*s %d %*s", &golds[k]);
		for (int i = 0; i < golds[k]; i++) {
			fscanf(fmap, "%d %d %d %d", &gold[k][i].type, &gold[k][i].coor.x, &gold[k][i].coor.y, &gold[k][i].value);
		}

		fscanf(fmap, "%*s %*s %d %*s", &foods[k]);
		for (int i = 0; i < foods[k]; i++) {
			fscanf(fmap, "%d %d %d\n", &food[k][i].type, &food[k][i].coor.x, &food[k][i].coor.y);
		}

		fscanf(fmap, "%*s %*s %d %*s", &spells[k]);
		for (int i = 0; i < spells[k]; i++) {
			fscanf(fmap, "%d %d %d\n", &spell[k][i].type, &spell[k][i].coor.x, &spell[k][i].coor.y);
		}
		fscanf(fmap, "%*s %*s %d %*s", &weapons[k]);
		for (int i = 0; i < weapons[k]; i++) {
			fscanf(fmap, "%d %d %d %d\n", &weapon[k][i].type, &weapon[k][i].number, &weapon[k][i].coor.x, &weapon[k][i].coor.y);
		}

		fscanf(fmap, "%*s:");
		for (int i = 3; i < RX - 1; i++) {
			for (int j = 1; j < RY - 1; j++) {
				char c;
				fscanf(fmap, " %c", &c);
				visibility[k][i][j] = c - '0';
			}
		}
	}
}

void save_map(char* name) {
	FILE *fmap = fopen(name, "w");
	fprintf(fmap, "Player coor: %d %d %d\n", player.floor, player.coor.x, player.coor.y); // Floor, x, y
	fprintf(fmap, "Player State: %d %d %d %d\n", player.health, player.hunger, player.fsize, player.ssize); // health, hunger, fsize, ssize;
	fprintf(fmap, "Player finv: ");
	for (int i = 0; i < player.fsize; i++) {
		fprintf(fmap, "%d ", player.finventory[i]);
	}
	fprintf(fmap, "\n");
	fprintf(fmap, "Player sinv: ");
	for (int i = 0; i < player.ssize; i++) {
		fprintf(fmap, "%d ", player.sinventory[i]);
	}
	fprintf(fmap, "Weapons: %d %d %d %d %d\n", player.mace, player.dagger, player.magicw, player.arrow, player.sword);
	fprintf(fmap, "\n");
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		fprintf(fmap, "Rooms:\n");
		for (int j = 0; j < ROOM_NUMBER; j++) {
			fprintf(fmap, "\t%d %d %d %d %d %d\n", grooms[k][j].coor[0].x, grooms[k][j].coor[0].y, grooms[k][j].coor[1].x, grooms[k][j].coor[1].y, grooms[k][j].type, grooms[k][j].hidden);
		}
		fprintf(fmap, "Paths number: %d\nPaths:\n", psize[k]);
		for (int j = 0; j < psize[k]; j++) {
			fprintf(fmap, "\tr1: %d, r2: %d, length: %d, coors: ", gpaths[k][j].r1, gpaths[k][j].r2, gpaths[k][j].size);
			for (int q = 0; q < gpaths[k][j].size; q++) {
				fprintf(fmap, "%d %d ", gpaths[k][j].coor[q].x, gpaths[k][j].coor[q].y);
			}
			fprintf(fmap, "\n");
		}
		fprintf(fmap, "Inport coor: %d %d\n", inports[k].x, inports[k].y);
		fprintf(fmap, "Outport coor: %d %d\n", outports[k].x, outports[k].y);
		fprintf(fmap, "Golds number: %d\nGolds:\n", golds[k]);
		for (int i = 0; i < golds[k]; i++) {
			fprintf(fmap, "%d %d %d %d\n", gold[k][i].type, gold[k][i].coor.x, gold[k][i].coor.y, gold[k][i].value);
		}
		fprintf(fmap, "Foods number: %d\nFoods:\n", foods[k]);
		for (int i = 0; i < foods[k]; i++) {
			fprintf(fmap, "%d %d %d\n", food[k][i].type, food[k][i].coor.x, food[k][i].coor.y);
		}
		fprintf(fmap, "Spells number: %d\nSpells:\n", spells[k]);
		for (int i = 0; i < spells[k]; i++) {
			fprintf(fmap, "%d %d %d\n", spell[k][i].type, spell[k][i].coor.x, spell[k][i].coor.y);
		}
		fprintf(fmap, "Weapons number: %d\nWeapons:\n", weapons[k]);
		for (int i = 0; i < weapons[k]; i++) {
			fprintf(fmap, "%d %d %d %d\n", weapon[k][i].type, weapon[k][i].number ,weapon[k][i].coor.x, weapon[k][i].coor.y);
		}
		fprintf(fmap, "Visibility:\n");
		for (int i = 3; i < RX - 1; i++) {
			for (int j = 1; j < RY - 1; j++) {
				fprintf(fmap, "%d", visibility[k][i][j]);
			}
			fprintf(fmap, "\n");
		}
	}
	
	fclose(fmap);
}

void check_visibility() {
	if (Visibility_power == -1) Visibility_power = 0;
	int i = inside_room(player.coor, player.floor);
	if (i != -1) {
		if (grooms[player.floor][i].type == 3) {
			// This is a nightmare room
			if (Visibility_power == 0) {
				Visibility_power = -1;
			}
		}
		else {
			for (int xi = grooms[player.floor][i].coor[0].x; xi <= grooms[player.floor][i].coor[1].x; xi++) {
				for (int yi = grooms[player.floor][i].coor[0].y; yi <= grooms[player.floor][i].coor[1].y; yi++) {
					visibility[player.floor][xi][yi] = 1;
				}
			}
		}
	}
	
	for (int i = 0; i < psize[player.floor]; i++) {
		for (int j = 0; j < gpaths[player.floor][i].size; j++) {
			if (player.coor.x == gpaths[player.floor][i].coor[j].x && player.coor.y == gpaths[player.floor][i].coor[j].y) {
				for (int k = 0; k <= MAX_VISIBILITY; k++) {
					if (j - k >= 0) {
						visibility[player.floor][gpaths[player.floor][i].coor[j - k].x][gpaths[player.floor][i].coor[j - k].y] = 1;
					}
					if (j + k < gpaths[player.floor][i].size) {
						visibility[player.floor][gpaths[player.floor][i].coor[j + k].x][gpaths[player.floor][i].coor[j + k].y] = 1;
					}
				}
			}
		}
	}
}

int gmove(int dx, int dy) {
	int res = 0;
	Coor nxt = {player.coor.x + dx, player.coor.y + dy};
	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
		return 0;
	}
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (grooms[player.floor][i].coor[0].x < nxt.x && nxt.x < grooms[player.floor][i].coor[1].x && grooms[player.floor][i].coor[0].y < nxt.y && nxt.y < grooms[player.floor][i].coor[1].y) {
			check_visibility();
			player.coor = nxt;
			res = 1;
			break;
		}
	}
	for (int i = 0; !res && i < psize[player.floor]; i++) {
		for (int j = 0; j < gpaths[player.floor][i].size; j++) {
			if (nxt.x == gpaths[player.floor][i].coor[j].x && nxt.y == gpaths[player.floor][i].coor[j].y) {
				player.coor = nxt;
				check_visibility();
				res = 1;
				break;
			}
		}
	}
	if (!res) {
		return 0;
	}

	decrease_hunger(1);
	return 1;
}

void check_loot() {
	int room_id = inside_room(player.coor, player.floor);
	
	// Gold (Normal and black)
	for (int i = 0; i < golds[player.floor]; i++) {
		if (player.coor.x == gold[player.floor][i].coor.x && player.coor.y == gold[player.floor][i].coor.y) {
			player.gold += gold[player.floor][i].value;
			player.point += gold[player.floor][i].value;
			memcpy(gold[player.floor] + i, gold[player.floor] + golds[player.floor] - 1, sizeof(Gold));
			golds[player.floor]--;
			break;
		}
	}

	// Foods
	for (int i = 0; i < foods[player.floor]; i++) {
		if (player.coor.x == food[player.floor][i].coor.x && player.coor.y == food[player.floor][i].coor.y) {
			if (grooms[player.floor][room_id].type == 3) {
				memcpy(food[player.floor] + i, food[player.floor] + foods[player.floor] - 1, sizeof(Food));
				foods[player.floor]--;
				break;
			}
			if (player.fsize < 5) {
				player.finventory[player.fsize++] = food[player.floor][i].type;
				memcpy(food[player.floor] + i, food[player.floor] + foods[player.floor] - 1, sizeof(Food));
				foods[player.floor]--;
			}
			else {
				food_inventory_full_massege();
			}
			break;
		}
	}
	// Spells

	for (int i = 0; i < spells[player.floor]; i++) {
		if (player.coor.x == spell[player.floor][i].coor.x && player.coor.y == spell[player.floor][i].coor.y) {
			if (grooms[player.floor][room_id].type == 3) {
				memcpy(spell[player.floor] + i, spell[player.floor] + spells[player.floor] - 1, sizeof(Spell));
				spells[player.floor]--;
				break;
			}
			if (player.ssize < 5) {
				player.sinventory[player.ssize++] = spell[player.floor][i].type;
				memcpy(spell[player.floor] + i, spell[player.floor] + spells[player.floor] - 1, sizeof(Spell));
				spells[player.floor]--;
			}
			else {
				spell_inventory_full_massege();
			}
			break;
		}
	}

	// Weapons
	// for (int i = 0; i < spells[player.floor]; i++) {
	// 	if (player.coor.x == spell[player.floor][i].coor.x && player.coor.y == spell[player.floor][i].coor.y) {
	// 		if (grooms[player.floor][room_id].type == 3) {
	// 			memcpy(spell[player.floor] + i, spell[player.floor] + spells[player.floor] - 1, sizeof(Spell));
	// 			spells[player.floor]--;
	// 			break;
	// 		}
	// 		if (player.ssize < 5) {
	// 			player.sinventory[player.ssize++] = spell[player.floor][i].type;
	// 			memcpy(spell[player.floor] + i, spell[player.floor] + spells[player.floor] - 1, sizeof(Spell));
	// 			spells[player.floor]--;
	// 		}
	// 		else {
	// 			spell_inventory_full_massege();
	// 		}
	// 		break;
	// 	}
	// }
}

void use_food(int id) {
	int type = player.finventory[id];
	switch (type) {
		case 0:
			player.hunger += MAX_HUNGER / 3;
			player.health += MAX_HEALTH / 5;
			break;
		case 1:
			player.hunger += MAX_HUNGER / 2;
			player.health += MAX_HEALTH / 4;
			break;
		case 2:
			player.hunger += MAX_HUNGER / 3;
			player.health += MAX_HEALTH / 5;
			break;
		case 3:
			player.hunger += MAX_HUNGER / 5;
			player.health -= MAX_HEALTH / 6;
			break;
	}
	if (player.hunger > MAX_HUNGER) {
		player.hunger = MAX_HUNGER;
	}
	if (player.health > MAX_HEALTH) {
		player.health = MAX_HEALTH;
	}

	for (int i = id + 1; i < player.fsize; i++) {
		player.finventory[i - 1] = player.finventory[i];
	}
	player.fsize--;
}

void food_inventory_menu() {
	bool running = true;
	check_visibility();
	gprint_all();
	int cursor = 0;
	if (player.fsize == 0) {
		food_inventory_empty_massege();
		return;
	}

	mvprintw(6, RY + 2 + cursor * 3, SPORT_CHAR);
	gprint_all();
	while (running && player.fsize > 0) {
		int c = getch();
		if (c == 27 || c == 'e') { // Esc
			running = false;
			continue;
		}
		else if (c == KRIGHT) {
			cursor = (cursor + 1) % player.fsize;
		}
		else if (c == KLEFT) {
			cursor = (cursor - 1 + player.fsize) % player.fsize;
		}
		else if (c == '\n') {
			use_food(cursor);
			cursor = 0;
		}

		clear();
		mvprintw(6, RY + 2 + cursor * 3, SPORT_CHAR);
		gprint_all();
	}
	clear();
}

void use_spell(int id) {
	int type = player.sinventory[id];
	// switch (type) {
	// 	case 0:
	// 		break;
	// 	case 1:
	// 		break;
	// 	case 2:
	// 		break;
	// }
	if (player.hunger > MAX_HUNGER) {
		player.hunger = MAX_HUNGER;
	}
	if (player.health > MAX_HEALTH) {
		player.health = MAX_HEALTH;
	}

	for (int i = id + 1; i < player.ssize; i++) {
		player.sinventory[i - 1] = player.sinventory[i];
	}
	player.ssize--;
}

void spell_inventory_menu() {
	bool running = true;
	check_visibility();
	gprint_all();
	int cursor = 0;
	if (player.ssize == 0) {
		spell_inventory_empty_massege();
		return;
	}

	mvprintw(10, RY + 2 + cursor * 3, SPORT_CHAR);
	gprint_all();
	while (running && player.ssize > 0) {
		int c = getch();
		if (c == 27 || c == 'r') { // Esc
			running = false;
			continue;
		}
		else if (c == KRIGHT) {
			cursor = (cursor + 1) % player.ssize;
		}
		else if (c == KLEFT) {
			cursor = (cursor - 1 + player.ssize) % player.ssize;
		}
		else if (c == '\n') {
			use_spell(cursor);
			cursor = 0;
		}

		clear();
		mvprintw(10, RY + 2 + cursor * 3, SPORT_CHAR);
		gprint_all();
	}
	clear();
}

void run_game(User *user, char *map_name) {
	player.gold = user->gold;
	player.point = user->point;

	FILE *fmap = fopen(map_name, "r");
	init_map(fmap);

	fclose(fmap);
	ginit_screen();
	
	
	bool running = true, f_c = 0, g_c = 0;
	floor_seen[player.floor] = 1;
	check_visibility();
	gprint_all();
	while (running) {
		int c = getch();
		clear();
		if (c == 27) { // Esc
			running = false;
			continue;
		}
		else if (c == '1') { // down left
			strcpy(PLAYER_CHAR, "\u1405");
			while (gmove(1, -1) && f_c);
			// gmove(1, -1);
		}
		else if (c == '2') { // down
			strcpy(PLAYER_CHAR, "\u1401");
			while (gmove(1, 0) && f_c);
			// gmove(1, 0);
		}
		else if (c == '3') { // down right
			strcpy(PLAYER_CHAR, "\u140A");
			while (gmove(1, 1) && f_c);
			// gmove(1, 1);
		}
		else if (c == '4') { // left
			strcpy(PLAYER_CHAR, "\u140A");
			while (gmove(0, -1) && f_c);
			// gmove(0, -1);
		}
		else if (c == '6') { // right
			strcpy(PLAYER_CHAR, "\u1405");
			while (gmove(0, 1) && f_c);
			// gmove(0, 1);
		}
		else if (c == '7') { // up left
			strcpy(PLAYER_CHAR, "\u1405");
			while (gmove(-1, -1) && f_c);
			// gmove(-1, -1);
		}
		else if (c == '8') { // up
			strcpy(PLAYER_CHAR, "\u1403");
			while (gmove(-1, 0) && f_c);
			// gmove(-1, 0);
		}
		else if (c == '9') { // up right
			strcpy(PLAYER_CHAR, "\u140A");
			while (gmove(-1, 1) && f_c);
			// gmove(-1, 1);
		}
		else if (c == 'm') {
			if (Visibility_power == 1)
				Visibility_power = 0;
			else {
				Visibility_power = 1;
			}
		}
		else if (c == KRIGHT && player.floor + 1 < FLOOR_NUMBER && player.coor.x == outports[player.floor].x && player.coor.y == outports[player.floor].y) {
			player.floor++;
			player.coor.x = inports[player.floor].x;
			player.coor.y = inports[player.floor].y;
			floor_seen[player.floor] = 1;
		}
		else if (c == KLEFT && player.floor > 0 && player.coor.x == inports[player.floor].x && player.coor.y == inports[player.floor].y) {
			player.floor--;
			player.coor.x = outports[player.floor].x;
			player.coor.y = outports[player.floor].y;
			floor_seen[player.floor] = 1;
		}
		else if (c == 'e') {
			food_inventory_menu();
		}
		else if (c == 'r') {
			spell_inventory_menu();
		}

		check_visibility();
		if (g_c == 0) {
			check_loot();
		}
		gprint_all();

		f_c = (!f_c) && (c == 'f');
		g_c = (!f_c) && (c == 'g');
	}

	save_map(map_name);
	gend_screen();
	reset_all();


	user->gold = player.gold;
	user->point = player.point;
	save_user(*user);
}