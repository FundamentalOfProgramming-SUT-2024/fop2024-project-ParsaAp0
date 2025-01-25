#include "header.h"

typedef struct {
    int coor[2];
    int health;
} Player;

static Room rooms[ROOM_NUMBER];
static Path paths[PATH_NUMBER];
static int psize;
static Coor start_coor;
char lmap[5][X][Y];
/*
    Layer 0: Base of the map: rooms, paths, traps, ...
    Layer 2: Player and enemies
    Layer 1: visibility of the map
    Layer 3: Notifications and messages.

*/

void init_map(FILE *fmap) {
    fscanf(fmap, "Rooms:\n");
    for (int j = 0; j < ROOM_NUMBER; j++) {
        fscanf(fmap, "\t%d %d %d %d\n", &rooms[j].coor[0].x, &rooms[j].coor[0].y, &rooms[j].coor[1].x, &rooms[j].coor[1].y);
    }
    fscanf(fmap, "Paths number: %d\nPaths:\n", &psize);
    for (int j = 0; j < psize; j++) {
        fscanf(fmap, "\tr1: %d, r2: %d, length: %d, coors: ", &paths[j].r1, &paths[j].r2, &paths[j].size);
        for (int k = 0; k < paths[j].size; k++) {
            fscanf(fmap, "%d %d, ", &paths[j].coor[k].x, &paths[j].coor[k].y);
        }
    }
    
    for (int i = 0; i < ROOM_NUMBER; i++) {
        Room *v = rooms + i;
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
        Path *v = paths + i;
        for (int i = 1; i < -1 + (*v).size; i++) {
    		lmap[0][(*v).coor[i].x][(*v).coor[i].y] = '#';
	    }
	    lmap[0][(*v).coor[0].x][(*v).coor[0].y] = '+';
	    lmap[0][(*v).coor[(*v).size - 1].x][(*v).coor[(*v).size - 1].y] = '+';
    }
}

void init_player(Player *p, FILE *fmap) {
	fscanf(fmap, "start coor: %d %d\n", &start_coor.x, &start_coor.y);
    p->health = 100;
}

void run_game(User user, char *map_name) {
    FILE *fmap = fopen(map_name, "r");
    init_map(fmap);

    Player player;
    init_player(&player, fmap);
}