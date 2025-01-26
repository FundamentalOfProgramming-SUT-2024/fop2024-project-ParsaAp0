#include "header.h"
#define MAX_VISIBILITY 5

/*
#include <stdio.h>
#define printf(...) printf(__VA_ARGS__), fflush(stdout)
*/

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

	/*
	for (int i = 3; i < RX - 1; i++) {
		for (int j = 1; j < RY - 1; j++) {
			lmap[2][i][j] = ' ';
		}
	}*/

	fscanf(fmap, "%*s %*s %d %d", &player.coor.x, &player.coor.y);
	player.health = 100;
	lmap[1][player.coor.x][player.coor.y] = 'P';

	fscanf(fmap, "%*s:");
	for (int i = 3; i < RX - 1; i++) {
		for (int j = 1; j < RY - 1; j++) {
			char c;
			fscanf(fmap, " %c", &c);
			if (c == '0') {
				lmap[2][i][j] = 0;
			}
			else {
				lmap[2][i][j] = ' ';
			}
		}
	}
}

void save_map(char* name) {
	FILE *fmap = fopen(name, "w");
	fprintf(fmap, "Rooms:\n");
	for (int j = 0; j < ROOM_NUMBER; j++) {
		fprintf(fmap, "\t%d %d %d %d\n", grooms[j].coor[0].x, grooms[j].coor[0].y, grooms[j].coor[1].x, grooms[j].coor[1].y);
	}
	fprintf(fmap, "Paths number: %d\nPaths:\n", psize);
	for (int j = 0; j < psize; j++) {
		fprintf(fmap, "\tr1: %d, r2: %d, length: %d, coors: ", gpaths[j].r1, gpaths[j].r2, gpaths[j].size);
		for (int k = 0; k < gpaths[j].size; k++) {
			fprintf(fmap, "%d %d ", gpaths[j].coor[k].x, gpaths[j].coor[k].y);
		}
		fprintf(fmap, "\n");
	}
	fprintf(fmap, "start coor: %d %d\n", player.coor.x, player.coor.y);
	fprintf(fmap, "Visibility:\n");
	for (int i = 3; i < RX - 1; i++) {
		for (int j = 1; j < RY - 1; j++) {
			if (lmap[2][i][j] == 0) {
				fprintf(fmap, "0");
			}
			else {
				fprintf(fmap, "1");
			}
		}
		fprintf(fmap, "\n");
	}
	
	fclose(fmap);
}

void gmove(int dx, int dy) {
	Coor nxt = {player.coor.x + dx, player.coor.y + dy};
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

// void mup() {
// 	Coor nxt = {player.coor.x - 1, player.coor.y};
// 	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
// 		return;
// 	}
// 	char c = lmap[0][nxt.x][nxt.y];
// 	if (c == ' ' || c == '_' || c == '|') {
// 		return;
// 	}
// 	lmap[1][player.coor.x][player.coor.y] = 0;
// 	player.coor = nxt;
// 	lmap[1][player.coor.x][player.coor.y] = 'P';
// }

// void mdown() {
// 	Coor nxt = {player.coor.x + 1, player.coor.y};
// 	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
// 		return;
// 	}
// 	char c = lmap[0][nxt.x][nxt.y];
// 	if (c == ' ' || c == '_' || c == '|') {
// 		return;
// 	}
// 	lmap[1][player.coor.x][player.coor.y] = 0;
// 	player.coor = nxt;
// 	lmap[1][player.coor.x][player.coor.y] = 'P';

// }

// void mleft() {
// 	Coor nxt = {player.coor.x, player.coor.y - 1};
// 	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
// 		return;
// 	}
// 	char c = lmap[0][nxt.x][nxt.y];
// 	if (c == ' ' || c == '_' || c == '|') {
// 		return;
// 	}
// 	lmap[1][player.coor.x][player.coor.y] = 0;
// 	player.coor = nxt;
// 	lmap[1][player.coor.x][player.coor.y] = 'P';

// }

// void mright() {
// 	Coor nxt = {player.coor.x, player.coor.y + 1};
// 	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
// 		return;
// 	}
// 	char c = lmap[0][nxt.x][nxt.y];
// 	if (c == ' ' || c == '_' || c == '|') {
// 		return;
// 	}
// 	lmap[1][player.coor.x][player.coor.y] = 0;
// 	player.coor = nxt;
// 	lmap[1][player.coor.x][player.coor.y] = 'P';
// }

void check_visibility() {
	int mx = MAX_VISIBILITY;
	
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (grooms[i].coor[0].x <= player.coor.x && player.coor.x <= grooms[i].coor[1].x && grooms[i].coor[0].y <= player.coor.y && player.coor.y <= grooms[i].coor[1].y) {
			for (int xi = grooms[i].coor[0].x; xi <= grooms[i].coor[1].x; xi++) {
				for (int yi = grooms[i].coor[0].y; yi <= grooms[i].coor[1].y; yi++) {
					lmap[2][xi][yi] = 0;
				}
			}
		}
	}

	for (int i = 0; i < psize; i++) {
		for (int j = 0; j < gpaths[i].size; j++) {
			if (player.coor.x == gpaths[i].coor[j].x && player.coor.y == gpaths[i].coor[j].y) {
				for (int k = 0; k <= MAX_VISIBILITY; k++) {
					if (j - k >= 0) {
						lmap[2][gpaths[i].coor[j - k].x][gpaths[i].coor[j - k].y] = 0;
					}
					if (j + k < gpaths[i].size) {
						lmap[2][gpaths[i].coor[j + k].x][gpaths[i].coor[j + k].y] = 0;
					}
				}
			}
		}
	}
}

void run_game(User user, char *map_name) {
	reset_all();

	FILE *fmap = fopen(map_name, "r");
	init_map(fmap);

	fclose(fmap);

	ginit_screen();
	
	bool running = true;
	check_visibility();
	gprint_all();
	while (running) {
		int c = getch();
		if (c == 27) { // Esc
			running = false;
			continue;
		}
		else if (c == '1') { // down left
			gmove(1, -1);
		}
		else if (c == '2') { // down
			gmove(1, 0);
		}
		else if (c == '3') { // down right
			gmove(1, 1);
		}
		else if (c == '4') { // left
			gmove(0, -1);
		}
		else if (c == '6') { // right
			gmove(0, 1);
		}
		else if (c == '7') { // up left
			gmove(-1, -1);
		}
		else if (c == '8') { // up
			gmove(-1, 0);
		}
		else if (c == '9') { // up right
			gmove(-1, 1);
		}
		else {

		}




		check_visibility();
		gprint_all();
	}

	save_map(map_name);
	gend_screen();
	// */
}