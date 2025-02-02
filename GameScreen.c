#include "header.h"

extern Player player;
extern char PLAYER_CHAR[4];

extern Room grooms[FLOOR_NUMBER][ROOM_NUMBER];
extern Path gpaths[FLOOR_NUMBER][PATH_NUMBER];
extern Coor inports[FLOOR_NUMBER], outports[FLOOR_NUMBER];
extern int psize[FLOOR_NUMBER];
extern int Visibility_power;
extern int visibility[FLOOR_NUMBER][X][Y];
extern int floor_seen[FLOOR_NUMBER];


extern int golds[FLOOR_NUMBER], foods[FLOOR_NUMBER], spells[FLOOR_NUMBER];
extern Gold gold[FLOOR_NUMBER][1000];
extern Food food[FLOOR_NUMBER][1000];
extern Spell spell[FLOOR_NUMBER][1000];

void gend_screen() {
	echo();
	clear();
	endwin();
}

void ginit_screen() {
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	cbreak();
	// init_pair(1, COLOR_BLACK, COLOR_WHITE);
}

void gprint_all() {
	// 1: Border
	// 2: Rooms
	// 3: Paths
	// 4: In & Out ports
	// 5: Loots
	// 6: Player, Enemies
	// 7: Visibility

	// Border
	attron(COLOR_PAIR(CID_MAP));
	
	for (int j = 0; j < RY; j++) {
		mvprintw(2, j, HWALL_CHAR);
		if (j != 0 || j != RY - 1) {
			mvprintw(RX - 1, j, HWALL_CHAR);
		}
	}
	for (int i = 3; i < RX; i++) {
		mvprintw(i, 0, VWALL_CHAR);
		mvprintw(i, RY - 1, VWALL_CHAR);
	}
	attroff(COLOR_PAIR(CID_MAP));

	// ROOMS
	int f = player.floor;
	for (int i = 0; i < ROOM_NUMBER; i++) {
		Room *v = grooms[f] + i;
		switch (v->type) {
			case 0:
				attron(COLOR_PAIR(CID_MAP));
				break;
			case 1:
				attron(COLOR_PAIR(93));
				break;
			case 2:
				attron(COLOR_PAIR(220));
				break;
			case 3:
				attron(COLOR_PAIR(88));
				break;
		}
		for (int xi = v->coor[0].x + 1; xi < v->coor[1].x; xi++) {
			for (int yi = v->coor[0].y + 1; yi < v->coor[1].y; yi++) {
				mvprintw(xi, yi, INROOM_CHAR);
			}
		}
		for (int yi = v->coor[0].y; yi <= v->coor[1].y; yi++) {
			mvprintw(v->coor[0].x, yi, HWALL_CHAR);
			if (yi != v->coor[0].y && yi != v->coor[1].y) {
				mvprintw(v->coor[1].x, yi, HWALL_CHAR);
			}
		}
		for (int xi = v->coor[0].x + 1; xi <= v->coor[1].x; xi++) {
			mvprintw(xi, v->coor[0].y, VWALL_CHAR);
			mvprintw(xi, v->coor[1].y, VWALL_CHAR);
		}
		switch (v->type) {
			case 0:
				attroff(COLOR_PAIR(CID_MAP));
				break;
			case 1:
				attroff(COLOR_PAIR(93));
				break;
			case 2:
				attroff(COLOR_PAIR(220));
				break;
			case 3:
				attroff(COLOR_PAIR(88));
				break;
		}
	}

	// Paths
	for (int i = 0; i < psize[f]; i++) {
		attron(COLOR_PAIR(CID_MAP));
		Path *v = gpaths[f] + i;
		for (int i = 1; i < -1 + (*v).size; i++) {
			mvprintw((*v).coor[i].x, (*v).coor[i].y, PATH_CHAR);
			// mvaddch((*v).coor[i].x, (*v).coor[i].y, '#');
		}
		attroff(COLOR_PAIR(CID_MAP));

		if (grooms[f][(*v).r2].hidden == 0 || Visibility_power) {
			switch (grooms[f][(*v).r1].type) {
				case 0:
					attron(COLOR_PAIR(CID_MAP));
					break;
				case 1:
					attron(COLOR_PAIR(93));
					break;
				case 2:
					attron(COLOR_PAIR(220));
					break;
				case 3:
					attron(COLOR_PAIR(88));
					break;
			}
			mvprintw((*v).coor[0].x, (*v).coor[0].y, DOOR_CHAR);
			switch (grooms[f][(*v).r1].type) {
				case 0:
					attroff(COLOR_PAIR(CID_MAP));
					break;
				case 1:
					attroff(COLOR_PAIR(93));
					break;
				case 2:
					attroff(COLOR_PAIR(220));
					break;
				case 3:
					attroff(COLOR_PAIR(88));
					break;
			}
		}
		if (grooms[f][(*v).r1].hidden == 0 || Visibility_power) {
			switch (grooms[f][(*v).r2].type) {
				case 0:
					attron(COLOR_PAIR(CID_MAP));
					break;
				case 1:
					attron(COLOR_PAIR(93));
					break;
				case 2:
					attron(COLOR_PAIR(220));
					break;
				case 3:
					attron(COLOR_PAIR(88));
					break;
			}
			mvprintw((*v).coor[(*v).size - 1].x, (*v).coor[(*v).size - 1].y, DOOR_CHAR);
			switch (grooms[f][(*v).r2].type) {
				case 0:
					attroff(COLOR_PAIR(CID_MAP));
					break;
				case 1:
					attroff(COLOR_PAIR(93));
					break;
				case 2:
					attroff(COLOR_PAIR(220));
					break;
				case 3:
					attroff(COLOR_PAIR(88));
					break;
			}
		}
	}

	// In & Out ports
	attron(COLOR_PAIR(CID_MAP));
	if (f > 0) {
		mvprintw(inports[f].x, inports[f].y, SPORT_CHAR);
	}
	if (f + 1 < FLOOR_NUMBER) {
		if (floor_seen[f + 1] == 1)
			mvprintw(outports[f].x, outports[f].y, SPORT_CHAR);
		else
			mvprintw(outports[f].x, outports[f].y, PORT_CHAR);
	}
	attroff(COLOR_PAIR(CID_MAP));

	// Loots
	int gcp[2] = {220, 94};
	for (int i = 0; i < golds[f]; i++) {
		attron(COLOR_PAIR(gcp[gold[f][i].type]));
		mvprintw(gold[f][i].coor.x, gold[f][i].coor.y, GOLD_CHAR);
		attroff(COLOR_PAIR(gcp[gold[f][i].type]));
	}

	int fcp[4] = {46, 160, 93, 46};
	for (int i = 0; i < foods[f]; i++) {
		attron(COLOR_PAIR(fcp[food[f][i].type]));
		mvprintw(food[f][i].coor.x, food[f][i].coor.y, FOOD_CHAR);
		attroff(COLOR_PAIR(fcp[food[f][i].type]));
	}
	int scp[3] = {220, 1, 93};
	for (int i = 0; i < spells[f]; i++) {
		attron(COLOR_PAIR(scp[spell[f][i].type]));
		mvprintw(spell[f][i].coor.x, spell[f][i].coor.y, SPELL_CHAR);
		attroff(COLOR_PAIR(scp[spell[f][i].type]));
	}

	// Player, Enemies
	for (int i = 0; i < player.satt; i++) {
		attron(player.att[i]);
	}
	mvprintw(player.coor.x, player.coor.y, PLAYER_CHAR);
	for (int i = 0; i < player.satt; i++) {
		attroff(player.att[i]);
	}

	// Visibility
	int nightmare_room_id = 0;
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (grooms[f][i].type == 3) {
			nightmare_room_id = i;
			break;
		}
	}
	if (Visibility_power != 1) {
		for (int i = 3; i < RX - 1; i++) {
			for (int j = 1; j < RY - 1; j++) {
				if (Visibility_power == -1) {
					if (!(grooms[f][nightmare_room_id].coor[0].x <= i && i <= grooms[f][nightmare_room_id].coor[1].x && grooms[f][nightmare_room_id].coor[0].y <= j && j <= grooms[f][nightmare_room_id].coor[1].y) || (abs(player.coor.x - i) > 2 || abs(player.coor.y - j) > 2)) {
						mvprintw(i, j, " ");
					}
				}
				else if (visibility[f][i][j] == 0) {					
					mvprintw(i, j, " ");
				}
				if (visibility[f][i][j] == 0 && Visibility_power != -1) {
				}
			}
		}
	}

	mvprintw(3, RY + 2, "Food inventory:");
	for (int i = 0; i < player.fsize; i++) {
		attron(COLOR_PAIR(fcp[player.finventory[i]]));
		mvprintw(5, RY + 2 + i * 3, FOOD_CHAR);
		attroff(COLOR_PAIR(fcp[player.finventory[i]]));
	}
	mvprintw(7, RY + 2, "Spell inventory:");
	for (int i = 0; i < player.ssize; i++) {
		attron(COLOR_PAIR(scp[player.sinventory[i]]));
		mvprintw(9, RY + 2 + i * 3, SPELL_CHAR);
		attroff(COLOR_PAIR(scp[player.sinventory[i]]));
	}

	attron(A_BOLD);
	mvprintw(X - 1, 5, "Hunger: %3d\t\tHealth: %3d\t\tGold: %5d\t\tPoint: %5d\n", player.hunger, player.health, player.gold, player.point);
	attroff(A_BOLD);

	mvprintw(X - 1, Y - 2, AMONGUS_CHAR);
}

void food_inventory_empty_massege() {

}

void food_inventory_full_massege() {

}

void spell_inventory_empty_massege() {

}

void spell_inventory_full_massege() {

}