#include "header.h"

#include <stdio.h>
#define printf(...) printf(__VA_ARGS__), fflush(stdout)

typedef struct {
	Coor coor;
	int health;
} Player;

Player player;

Room grooms[ROOM_NUMBER];
Path gpaths[PATH_NUMBER];
int psize;
Coor start_coor;
extern char lmap[5][X][Y];
extern int lcons[5][X][Y], ldelayed[5][X][Y], att[5][X][Y][10], satt[5][X][Y];
/*
	Layer 0: Base of the map: rooms, paths, traps, ...
	Layer 1: Player and enemies
	Layer 2: visibility of the map
	Layer 3: Notifications and messages.

*/

void reset_all() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < X; j++) {
			for (int k = 0; k < Y; k++) {
				lmap[i][j][k] = 0;
				lcons[i][j][k] = 0;
				ldelayed[i][j][k] = 0;
			}
		}
	}
	for (int i = 0; i < ROOM_NUMBER; i++) {
		free(grooms[i].coor);
	}
	for (int i = 0; i < psize; i++) {
		free(gpaths[i].coor);
		gpaths[i].r1 = gpaths[i].r2 = gpaths[i].size = 0;
	}
	psize = 0;
}

void init_map(FILE *fmap) {
	// Reading rooms and paths.
	fscanf(fmap, "Rooms:");
	for (int j = 0; j < ROOM_NUMBER; j++) {
		grooms[j].coor = (Coor *) calloc(2, sizeof(Coor));
		fscanf(fmap, "%d %d %d %d", &grooms[j].coor[0].x, &grooms[j].coor[0].y, &grooms[j].coor[1].x, &grooms[j].coor[1].y);
	}
	fscanf(fmap, "%*s %*s %d\nPaths:", &psize);
	
	for (int j = 0; j < psize; j++) {
		fscanf(fmap, "%*s %d%*s %*s %d%*s %*s %d%*s %*s", &gpaths[j].r1, &gpaths[j].r2, &gpaths[j].size);
		gpaths[j].coor = (Coor *) calloc(gpaths[j].size, sizeof(Coor));
		for (int k = 0; k < gpaths[j].size; k++) {
			fscanf(fmap, "%d %d", &gpaths[j].coor[k].x, &gpaths[j].coor[k].y);
		}
	}

	// Setting rooms and paths on the layer 0.

	for (int i = 2; i < RX; i++) {
		for (int j = 0; j < RY; j++) {
			lmap[0][i][j] = ' ';
		}
	}
	for (int j = 0; j < RY; j++) {
		lmap[0][2][j] = '_';
		if (j != 0 || j != RY - 1) {
			lmap[0][RX - 1][j] = '_';
		}
	}
	for (int i = 3; i < RX; i++) {
		lmap[0][i][0] = '|';
		lmap[0][i][RY - 1] = '|';
	}

	for (int i = 0; i < ROOM_NUMBER; i++) {
		Room *v = grooms + i;
		for (int xi = v->coor[0].x + 1; xi < v->coor[1].x; xi++) {
			for (int yi = v->coor[0].y + 1; yi < v->coor[1].y; yi++) {
				lmap[0][xi][yi] = '.';
			}
		}
		for (int yi = v->coor[0].y; yi <= v->coor[1].y; yi++) {
			lmap[0][v->coor[0].x][yi] = '_';
			if (yi != v->coor[0].y && yi != v->coor[1].y) {
				lmap[0][v->coor[1].x][yi] = '_';
			}
		}
		for (int xi = v->coor[0].x + 1; xi <= v->coor[1].x; xi++) {
			lmap[0][xi][v->coor[0].y] = '|';
			lmap[0][xi][v->coor[1].y] = '|';
		}
	}
	for (int i = 0; i < psize; i++) {
		Path *v = gpaths + i;
		for (int i = 1; i < -1 + (*v).size; i++) {
			lmap[0][(*v).coor[i].x][(*v).coor[i].y] = '#';
		}
		lmap[0][(*v).coor[0].x][(*v).coor[0].y] = '+';
		lmap[0][(*v).coor[(*v).size - 1].x][(*v).coor[(*v).size - 1].y] = '+';
	}


}

void init_player(Player *p, FILE *fmap) {
	fscanf(fmap, "%*s %*s %d %d", &player.coor.x, &player.coor.y);
	p->health = 100;
	lmap[1][player.coor.x][player.coor.y] = 'P';
}

void mup() {
	Coor nxt = {player.coor.x - 1, player.coor.y};
	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
		return;
	}
	char c = lmap[0][nxt.x][nxt.y];
	if (c == ' ' || c == '_' || c == '|') {
		return;
	}
	lmap[1][player.coor.x][player.coor.y] = 0;
	player.coor = nxt;
	lmap[1][player.coor.x][player.coor.y] = 'P';
}

void mdown() {
	Coor nxt = {player.coor.x + 1, player.coor.y};
	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
		return;
	}
	char c = lmap[0][nxt.x][nxt.y];
	if (c == ' ' || c == '_' || c == '|') {
		return;
	}
	lmap[1][player.coor.x][player.coor.y] = 0;
	player.coor = nxt;
	lmap[1][player.coor.x][player.coor.y] = 'P';

}

void mleft() {
	Coor nxt = {player.coor.x, player.coor.y - 1};
	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
		return;
	}
	char c = lmap[0][nxt.x][nxt.y];
	if (c == ' ' || c == '_' || c == '|') {
		return;
	}
	lmap[1][player.coor.x][player.coor.y] = 0;
	player.coor = nxt;
	lmap[1][player.coor.x][player.coor.y] = 'P';

}

void mright() {
	Coor nxt = {player.coor.x, player.coor.y + 1};
	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
		return;
	}
	char c = lmap[0][nxt.x][nxt.y];
	if (c == ' ' || c == '_' || c == '|') {
		return;
	}
	lmap[1][player.coor.x][player.coor.y] = 0;
	player.coor = nxt;
	lmap[1][player.coor.x][player.coor.y] = 'P';
	
}

void run_game(User user, char *map_name) {
	reset_all();

	FILE *fmap = fopen(map_name, "r");
	init_map(fmap);

	init_player(&player, fmap);

	fclose(fmap);

	ginit_screen();
	
	bool running = true;
	gprint_all();
	while (running) {
		int c = getch();
		if (c == 27) { // Esc
			running = false;
			continue;
		}
		else if (c == KDOWN) {
			mdown();
		}
		else if (c == KUP) {
			mup();
		}
		else if (c == KLEFT) {
			mleft();
		}
		else if (c == KRIGHT) {
			mright();
		}
		else {

		}



		gprint_all();
	}

	save_map(map_name, grooms, gpaths, player.coor, psize);
	gend_screen();
	// */
}