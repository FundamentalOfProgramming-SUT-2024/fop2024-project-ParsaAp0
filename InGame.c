#include "header.h"
#include <stdio.h>
#define printf(...) printf(__VA_ARGS__); fflush(stdout)
#define MAX_VISIBILITY 5

/*
#include <stdio.h>
#define printf(...) printf(__VA_ARGS__), fflush(stdout)
*/

typedef struct {
	Coor coor;
	int floor;
	int health;
} Player;

Player player;

Room grooms[FLOOR_NUMBER][ROOM_NUMBER];
Path gpaths[FLOOR_NUMBER][PATH_NUMBER];
Coor inports[FLOOR_NUMBER], outports[FLOOR_NUMBER];
int psize[FLOOR_NUMBER];
Coor start_coor;
extern char lmap[FLOOR_NUMBER][5][X][Y];
extern int lcons[FLOOR_NUMBER][5][X][Y], ldelayed[FLOOR_NUMBER][5][X][Y], att[FLOOR_NUMBER][5][X][Y][10], satt[FLOOR_NUMBER][5][X][Y];
extern int Visibility_power[FLOOR_NUMBER];
/*
	Layer 0: Base of the map: rooms, paths, traps, ...
	Layer 1: Player and enemies
	Layer 2: visibility of the map
	Layer 3: Notifications and messages.

*/

void reset_all() {
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < X; j++) {
				for (int q = 0; q < Y; q++) {
					lmap[k][i][j][q] = 0;
					lcons[k][i][j][q] = 0;
					ldelayed[k][i][j][q] = 0;
				}
			}
		}
		for (int i = 0; i < ROOM_NUMBER; i++) {
			free(grooms[k][i].coor);
		}
		for (int i = 0; i < psize[k]; i++) {
			free(gpaths[k][i].coor);
			gpaths[k][i].r1 = gpaths[k][i].r2 = gpaths[k][i].size = 0;
		}
		psize[k] = 0;
	}
}

void init_map(FILE *fmap) {
	fscanf(fmap, "%*s %*s %d %d %d", &player.floor, &player.coor.x, &player.coor.y);
	player.health = 100;
	lmap[player.floor][1][player.coor.x][player.coor.y] = 'P';
	att[player.floor][1][player.coor.x][player.coor.y][satt[player.floor][1][player.coor.x][player.coor.y]++] = A_BOLD;
	
	// Reading rooms and paths.
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		fscanf(fmap, "%*s");
		for (int j = 0; j < ROOM_NUMBER; j++) {
			grooms[k][j].coor = (Coor *) calloc(2, sizeof(Coor));
			fscanf(fmap, "%d %d %d %d", &grooms[k][j].coor[0].x, &grooms[k][j].coor[0].y, &grooms[k][j].coor[1].x, &grooms[k][j].coor[1].y);
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

		// Setting rooms and paths on the layer 0.

		for (int i = 2; i < RX; i++) {
			for (int j = 0; j < RY; j++) {
				lmap[k][0][i][j] = ' ';
			}
		}
		for (int j = 0; j < RY; j++) {
			lmap[k][0][2][j] = '_';
			if (j != 0 || j != RY - 1) {
				lmap[k][0][RX - 1][j] = '_';
			}
		}
		for (int i = 3; i < RX; i++) {
			lmap[k][0][i][0] = '|';
			lmap[k][0][i][RY - 1] = '|';
		}

		for (int i = 0; i < ROOM_NUMBER; i++) {
			Room *v = grooms[k] + i;
			for (int xi = v->coor[0].x + 1; xi < v->coor[1].x; xi++) {
				for (int yi = v->coor[0].y + 1; yi < v->coor[1].y; yi++) {
					lmap[k][0][xi][yi] = '.';
				}
			}
			for (int yi = v->coor[0].y; yi <= v->coor[1].y; yi++) {
				lmap[k][0][v->coor[0].x][yi] = '_';
				if (yi != v->coor[0].y && yi != v->coor[1].y) {
					lmap[k][0][v->coor[1].x][yi] = '_';
				}
			}
			for (int xi = v->coor[0].x + 1; xi <= v->coor[1].x; xi++) {
				lmap[k][0][xi][v->coor[0].y] = '|';
				lmap[k][0][xi][v->coor[1].y] = '|';
			}
		}
		for (int i = 0; i < psize[k]; i++) {
			Path *v = gpaths[k] + i;
			for (int i = 1; i < -1 + (*v).size; i++) {
				lmap[k][0][(*v).coor[i].x][(*v).coor[i].y] = '#';
			}
			lmap[k][0][(*v).coor[0].x][(*v).coor[0].y] = '+';
			lmap[k][0][(*v).coor[(*v).size - 1].x][(*v).coor[(*v).size - 1].y] = '+';
		}
		if (k > 0)
			lmap[k][0][inports[k].x][inports[k].y] = '^';
		if (k + 1 < FLOOR_NUMBER)
			lmap[k][0][outports[k].x][outports[k].y] = '<';


		/*
		for (int i = 3; i < RX - 1; i++) {
			for (int j = 1; j < RY - 1; j++) {
				lmap[k][2][i][j] = ' ';
			}
		}*/

		fscanf(fmap, "%*s:");
		for (int i = 3; i < RX - 1; i++) {
			for (int j = 1; j < RY - 1; j++) {
				char c;
				fscanf(fmap, " %c", &c);
				if (c == '0') {
					lmap[k][2][i][j] = 0;
				}
				else {
					lmap[k][2][i][j] = ' ';
				}
			}
		}
	}
}

void save_map(char* name) {
	FILE *fmap = fopen(name, "w");
	fprintf(fmap, "start coor: %d %d %d\n", player.floor, player.coor.x, player.coor.y);
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		fprintf(fmap, "Rooms:\n");
		for (int j = 0; j < ROOM_NUMBER; j++) {
			fprintf(fmap, "\t%d %d %d %d\n", grooms[k][j].coor[0].x, grooms[k][j].coor[0].y, grooms[k][j].coor[1].x, grooms[k][j].coor[1].y);
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
		fprintf(fmap, "Visibility:\n");
		for (int i = 3; i < RX - 1; i++) {
			for (int j = 1; j < RY - 1; j++) {
				if (lmap[k][2][i][j] == 0) {
					fprintf(fmap, "0");
				}
				else {
					fprintf(fmap, "1");
				}
			}
			fprintf(fmap, "\n");
		}
	}
	
	fclose(fmap);
}

void gmove(int dx, int dy) {
	Coor nxt = {player.coor.x + dx, player.coor.y + dy};
	if (nxt.x < 0 || nxt.x >= X || nxt.y < 0 || nxt.y >= Y) {
		return;
	}
	char c = lmap[player.floor][0][nxt.x][nxt.y];
	if (c == ' ' || c == '_' || c == '|') {
		return;
	}
	for (int i = 0; i < satt[player.floor][1][player.coor.x][player.coor.y]; i++) {
		att[player.floor][1][nxt.x][nxt.y][i] = att[player.floor][1][player.coor.x][player.coor.y][i];
	}
	satt[player.floor][1][nxt.x][nxt.y] = satt[player.floor][1][player.coor.x][player.coor.y];
	satt[player.floor][1][player.coor.x][player.coor.y] = 0;
	lmap[player.floor][1][player.coor.x][player.coor.y] = 0;
	player.coor = nxt;
	lmap[player.floor][1][player.coor.x][player.coor.y] = 'P';
}


void check_visibility() {
	
	for (int i = 0; i < ROOM_NUMBER; i++) {
		int a1 = grooms[player.floor][i].coor[0].x, a2 = grooms[player.floor][i].coor[1].x;
		if (grooms[player.floor][i].coor[0].x <= player.coor.x && player.coor.x <= grooms[player.floor][i].coor[1].x && grooms[player.floor][i].coor[0].y <= player.coor.y && player.coor.y <= grooms[player.floor][i].coor[1].y) {
			for (int xi = a1; xi <= a2; xi++) {
				for (int yi = grooms[player.floor][i].coor[0].y; yi <= grooms[player.floor][i].coor[1].y; yi++) {
					lmap[player.floor][2][xi][yi] = 0;
				}
			}
		}
	}

	for (int i = 0; i < psize[player.floor]; i++) {
		for (int j = 0; j < gpaths[player.floor][i].size; j++) {
			if (player.coor.x == gpaths[player.floor][i].coor[j].x && player.coor.y == gpaths[player.floor][i].coor[j].y) {
				for (int k = 0; k <= MAX_VISIBILITY; k++) {
					if (j - k >= 0) {
						lmap[player.floor][2][gpaths[player.floor][i].coor[j - k].x][gpaths[player.floor][i].coor[j - k].y] = 0;
					}
					if (j + k < gpaths[player.floor][i].size) {
						lmap[player.floor][2][gpaths[player.floor][i].coor[j + k].x][gpaths[player.floor][i].coor[j + k].y] = 0;
					}
				}
			}
		}
	}
}

void run_game(User user, char *map_name) {

	FILE *fmap = fopen(map_name, "r");
	init_map(fmap);

	fclose(fmap);
	ginit_screen();
	
	
	bool running = true;
	check_visibility();
	gprint_all(player.floor);
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
		else if (c == 'm') {
			Visibility_power[player.floor] = !Visibility_power[player.floor];
		}
		else if (c == ']' && player.floor + 1 < FLOOR_NUMBER && player.coor.x == outports[player.floor].x && player.coor.y == outports[player.floor].y) {
			lmap[player.floor][0][player.coor.x][player.coor.y] = '^' ;
			player.floor++;
			player.coor.x = inports[player.floor].x;
			player.coor.y = inports[player.floor].y;
			lmap[player.floor][1][player.coor.x][player.coor.y] = 'P';
			att[player.floor][1][player.coor.x][player.coor.y][satt[player.floor][1][player.coor.x][player.coor.y]++] = A_BOLD;
		}
		else if (c == '[' && player.floor > 0 && player.coor.x == inports[player.floor].x && player.coor.y == inports[player.floor].y) {
			player.floor--;
			player.coor.x = outports[player.floor].x;
			player.coor.y = outports[player.floor].y;
			lmap[player.floor][1][player.coor.x][player.coor.y] = 'P';
		}

		check_visibility();
		gprint_all(player.floor);
	}

	save_map(map_name);
	gend_screen();
	reset_all();
	// */
}