#include "header.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#define printf(...) printf(__VA_ARGS__); fflush(stdout)

// extern char[RX][RY];
static Room rooms[FLOOR_NUMBER][ROOM_NUMBER];
static Path paths[FLOOR_NUMBER][PATH_NUMBER];
static Coor inports[FLOOR_NUMBER], outports[FLOOR_NUMBER];
static int psize[FLOOR_NUMBER];
static Coor start_coor;
static Coor rand_points[FLOOR_NUMBER][10000];
static int rands[FLOOR_NUMBER];

static int golds[FLOOR_NUMBER], foods[FLOOR_NUMBER], spells[FLOOR_NUMBER], weapons[FLOOR_NUMBER], monsters[FLOOR_NUMBER];
static Gold gold[FLOOR_NUMBER][1000];
static Food food[FLOOR_NUMBER][1000];
static Spell spell[FLOOR_NUMBER][1000];
static Weapon weapon[FLOOR_NUMBER][1000];
static Monster monster[FLOOR_NUMBER][1000];
static int Deamon_number[4] = {3, 3, 3, 2};
static int Fire_number[4] = {2, 2, 1, 2};
static int Giant_number[4] = {1, 2, 2, 1};
static int Snake_number[4] = {0, 1, 2, 2};
static int Undeed_number[4] = {0, 1, 1, 2};

static int limits[ROOM_NUMBER][4] = {
	{4, 14, 1, 43},
	{18, 28, 1, 43},
	{32, RX - 1, 1, 43},
	{4, 14, 48, 93},
	{18, 28, 48, 93},
	{32, RX - 1, 53, 93},
	{4, 14, 98, RY - 1},
	{18, 28, 98, RY - 1},
	{32, RX - 1, 98, RY - 1}
};

static int inside_room(Coor c, int f) {
	int room_id = -1;
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (rooms[f][i].coor[0].x <= c.x && c.x <= rooms[f][i].coor[1].x && rooms[f][i].coor[0].y <= c.y && c.y <= rooms[f][i].coor[1].y) {
			room_id = i;
			break;
		}
	}

	return room_id;
}

void set_non_treasure_random_point(Coor *r, int floor) {
	int c = 0, x = 0, y = 0;
	do {
		int room = rand() % ROOM_NUMBER;
		while (rooms[floor][room].type == 2) {
			room = rand() % ROOM_NUMBER;
		}
		// printf("rands: %d\n", rands[floor]);
		c = 0;
		x = rooms[floor][room].coor[0].x + 1 + rand() % (rooms[floor][room].coor[1].x - rooms[floor][room].coor[0].x - 2);
		y = rooms[floor][room].coor[0].y + 1 + rand() % (rooms[floor][room].coor[1].y - rooms[floor][room].coor[0].y - 2);
		for (int i = 0; i < rands[floor]; i++) {
			if (x == rand_points[floor][i].x && y == rand_points[floor][i].y) {
				c = 1;
				break;
			}
		}
	} while (c);
	r->x = x;
	r->y = y;
	rand_points[floor][rands[floor]].x = r->x;
	rand_points[floor][rands[floor]].y = r->y;
	rands[floor]++;
}

void set_treasure_random_point(Coor *r) {
	int floor = FLOOR_NUMBER - 1;
	int c = 0, x = 0, y = 0;
	do {
		int room = 0;
		for (int i = 0; i < ROOM_NUMBER; i++) {
			if (rooms[floor][i].type == 2) {
				room = i;
				break;
			}
		}
		// printf("rands: %d\n", rands[floor]);
		c = 0;
		x = rooms[floor][room].coor[0].x + 1 + rand() % (rooms[floor][room].coor[1].x - rooms[floor][room].coor[0].x - 2);
		y = rooms[floor][room].coor[0].y + 1 + rand() % (rooms[floor][room].coor[1].y - rooms[floor][room].coor[0].y - 2);
		for (int i = 0; i < rands[floor]; i++) {
			if (x == rand_points[floor][i].x && y == rand_points[floor][i].y) {
				c = 1;
				break;
			}
		}
	} while (c);
	r->x = x;
	r->y = y;
	rand_points[floor][rands[floor]].x = r->x;
	rand_points[floor][rands[floor]].y = r->y;
	rands[floor]++;
}

void set_enchant_random_point(Coor *r, int floor) {
	int c = 0, x = 0, y = 0;
	do {
		int room = 0;
		for (int i = 0; i < ROOM_NUMBER; i++) {
			if (rooms[floor][i].type == 1) {
				room = i;
				break;
			}
		}
		c = 0;
		x = rooms[floor][room].coor[0].x + 1 + rand() % (rooms[floor][room].coor[1].x - rooms[floor][room].coor[0].x - 2);
		y = rooms[floor][room].coor[0].y + 1 + rand() % (rooms[floor][room].coor[1].y - rooms[floor][room].coor[0].y - 2);
		for (int i = 0; i < rands[floor]; i++) {
			if (x == rand_points[floor][i].x && y == rand_points[floor][i].y) {
				c = 1;
				break;
			}
		}
	} while (c);
	r->x = x;
	r->y = y;
	rand_points[floor][rands[floor]].x = r->x;
	rand_points[floor][rands[floor]].y = r->y;
	rands[floor]++;
}

void set_normal_random_point(Coor *r, int floor) {
	int c = 0, x = 0, y = 0;
	do {
		int room = rand() % ROOM_NUMBER;
		while (rooms[floor][room].type || rooms[floor][room].hidden) {
			room = rand() % ROOM_NUMBER;
		}
		// printf("rands: %d\n", rands[floor]);
		c = 0;
		x = rooms[floor][room].coor[0].x + 1 + rand() % (rooms[floor][room].coor[1].x - rooms[floor][room].coor[0].x - 2);
		y = rooms[floor][room].coor[0].y + 1 + rand() % (rooms[floor][room].coor[1].y - rooms[floor][room].coor[0].y - 2);
		for (int i = 0; i < rands[floor]; i++) {
			if (x == rand_points[floor][i].x && y == rand_points[floor][i].y) {
				c = 1;
				break;
			}
		}
	} while (c);
	r->x = x;
	r->y = y;
	rand_points[floor][rands[floor]].x = r->x;
	rand_points[floor][rands[floor]].y = r->y;
	rands[floor]++;
}

void set_wide_random_point(Coor *r, int floor) {
	int c = 0, x = 0, y = 0;
	do {
		int room = rand() % ROOM_NUMBER;
		c = 0;
		x = rooms[floor][room].coor[0].x + 1 + rand() % (rooms[floor][room].coor[1].x - rooms[floor][room].coor[0].x - 2);
		y = rooms[floor][room].coor[0].y + 1 + rand() % (rooms[floor][room].coor[1].y - rooms[floor][room].coor[0].y - 2); // -3!
		for (int i = 0; i < rands[floor]; i++) {
			if (x == rand_points[floor][i].x && (y == rand_points[floor][i].y || y + 1 == rand_points[floor][i].y)) {
				c = 1;
				break;
			}
		}
	} while (c);
	r->x = x;
	r->y = y;
	rand_points[floor][rands[floor]].x = r->x;
	rand_points[floor][rands[floor]].y = r->y;
	rands[floor]++;
}

void set_random_point(Coor *r, int floor) {
	int c = 0, x = 0, y = 0;
	do {
		int room = rand() % ROOM_NUMBER;
		c = 0;
		x = rooms[floor][room].coor[0].x + 1 + rand() % (rooms[floor][room].coor[1].x - rooms[floor][room].coor[0].x - 2);
		y = rooms[floor][room].coor[0].y + 1 + rand() % (rooms[floor][room].coor[1].y - rooms[floor][room].coor[0].y - 2);
		for (int i = 0; i < rands[floor]; i++) {
			if (x == rand_points[floor][i].x && y == rand_points[floor][i].y) {
				c = 1;
				break;
			}
		}
	} while (c);
	r->x = x;
	r->y = y;
	rand_points[floor][rands[floor]].x = r->x;
	rand_points[floor][rands[floor]].y = r->y;
	rands[floor]++;
}

Coor *create_room(int *lim) { // coor: {x1, x2, y1, y2}
	Coor *res = (Coor *) calloc(2, sizeof(Coor));
	int szx = 8 + rand() % (lim[1] - lim[0] - 7), szy = 8 + rand() % (lim[3] - lim[2] - 7);
	res[0].x = lim[0] + rand() % (lim[1] - lim[0] - szx + 1);
	res[1].x = res[0].x + 5 + rand() % (szx - 5);
	res[0].y = lim[2] + rand() % (lim[3] - lim[2] - szy + 1);
	res[1].y = res[0].y + 5 + rand() % (szy - 5);
	return res;
}

void add_path_char(Path *v, Coor c) {
	if ((*v).size == 0) 
		(*v).coor = (Coor *) malloc(((*v).size + 1) * sizeof(Coor));
		// (*v).coor = (int **) malloc(((*v).size + 1) * sizeof(int *));
	else
		(*v).coor = (Coor *) realloc((*v).coor, ((*v).size + 1) * sizeof(Coor));
		//(*v).coor = (int **) realloc((*v).coor, ((*v).size + 1) * sizeof(int *));
	(*v).coor[(*v).size].x = c.x;
	(*v).coor[(*v).size].y = c.y;

	(*v).size++;
}

void create_vertical_path(int k, int id1, int id2, int tx, int tly, int Try, int dx, int dly, int dry) {
	paths[k][psize[k]].r1 = id1;
	paths[k][psize[k]].r2 = id2;
	int up = tly + rand() % (Try - tly), down = dly + rand() % (dry - dly);
	if (up == down) {
		int ptr = tx;
		while (ptr <= dx) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			co[0].x = ptr;
			co[0].y = up;
			add_path_char(paths[k] + psize[k], co[0]);
			ptr++;
		}
	}
	else {
		int mid = tx + 1 + rand() % (dx - tx - 1), ptr = tx;
		while (ptr <= mid) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			// int *co = (int *) malloc(2 * sizeof(int));
			co[0].x = ptr;
			co[0].y = up;
			add_path_char(paths[k] + psize[k], co[0]);
			ptr++;
		}

		if (up < down) {
			for (int p = up + 1; p < down; p++) {
				Coor *co = (Coor *) malloc(sizeof(Coor));
				// int *co = (int *) malloc(2 * sizeof(int));
				co[0].x = mid;
				co[0].y = p;
				add_path_char(paths[k] + psize[k], co[0]);
			}

		}
		else {
			for (int p = up - 1; p > down; p--) {
			// for (int p = down + 1; p < up; p++) {
				Coor *co = (Coor *) malloc(sizeof(Coor));
				// int *co = (int *) malloc(2 * sizeof(int));
				co[0].x = mid;
				co[0].y = p;
				add_path_char(paths[k] + psize[k], co[0]);
			}
		}
		
		ptr--;
		while (ptr <= dx) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			// int *co = (int *) malloc(2 * sizeof(int));
			co[0].x = ptr;
			co[0].y = down;
			add_path_char(paths[k] + psize[k], co[0]);
			ptr++;
		}
	}
	psize[k]++;
}

void create_horizontal_path(int k, int id1, int id2, int ly, int ltx, int ldx, int ry, int rtx, int rdx) {
	paths[k][psize[k]].r1 = id1;
	paths[k][psize[k]].r2 = id2;
	int left = ltx + rand() % (ldx - ltx), right = rtx + rand() % (rdx - rtx); 
	// int up = tly + rand() % (Try - tly), down = dly + rand() % (dry - dly);
	if (left == right) {
		int ptr = ly;
		while (ptr <= ry) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			// int *co = (int *) malloc(2 * sizeof(int));
			co[0].x = left;
			co[0].y = ptr;
			add_path_char(paths[k] + psize[k], co[0]);
			ptr++;
		}
	}
	else {
		int mid = ly + 1 + rand() % (ry - ly - 1), ptr = ly;
		while (ptr <= mid) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			// int *co = (int *) malloc(2 * sizeof(int));
			co[0].x = left;
			co[0].y = ptr;
			add_path_char(paths[k] + psize[k], co[0]);
			ptr++;
		}

		if (left < right) {
			for (int p = left + 1; p < right; p++) {
				Coor *co = (Coor *) malloc(sizeof(Coor));
				// int *co = (int *) malloc(2 * sizeof(int));
				co[0].x = p;
				co[0].y = mid;
				add_path_char(paths[k] + psize[k], co[0]);
			}
		}

		else {
			for (int p = left - 1; p > right; p--) {
			// for (int p = right + 1; p < left; p++) {
				Coor *co = (Coor *) malloc(sizeof(Coor));
				// int *co = (int *) malloc(2 * sizeof(int));
				co[0].x = p;
				co[0].y = mid;
				add_path_char(paths[k] + psize[k], co[0]);
			}
		}
		ptr--;
		while (ptr <= ry) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			// int *co = (int *) malloc(2 * sizeof(int));
			co[0].x = right;
			co[0].y = ptr;
			add_path_char(paths[k] + psize[k], co[0]);
			ptr++;
		}
	}
	psize[k]++;
}

void define_rooms_type(int k) {
	int num[ROOM_NUMBER] = {0};
	for (int i = 0; i < psize[k]; i++) {
		num[paths[k][i].r1]++;
		num[paths[k][i].r2]++;
	}
	int b = 0;
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (num[i] == 1) {
			rooms[k][i].hidden = true;
			b = 1;
			break;
		}
	}
	if (b == 0) {
		return;
	}

	if (k == FLOOR_NUMBER - 1) {
		// Last Floor --> Treasure room
		int id = rand() % ROOM_NUMBER;
		rooms[k][id].type = 2;
	}
	int id = rand() % ROOM_NUMBER;
	while (rooms[k][id].type) {
		id = rand() % ROOM_NUMBER;
	}
	rooms[k][id].type = 1; // Enchant room
	id = rand() % ROOM_NUMBER;
	while (rooms[k][id].type) {
		id = rand() % ROOM_NUMBER;
	}
	rooms[k][id].type = 3; // Nightmare room
}

void create_map(int k) {
	for (int i = 0; i < ROOM_NUMBER; i++) {
		rooms[k][i].coor = create_room(limits[i]);
	}
	int doorp[ROOM_NUMBER] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int doorr[ROOM_NUMBER] = {2, 3, 2, 3, 4, 3, 2, 3, 2};
	int need[ROOM_NUMBER] = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			int id1 = 3 * i + j, id2 = id1 + 1;
			Room *r1 = rooms[k] + id1, *r2 = rooms[k] + id2;
			if (doorp[id1] == 0) {
				need[id1] = 10000 * (1 << (doorr[id1] - 1)) / ((1 << doorr[id1]) - 1);
			}
			else {
				need[id1] = 10000 / 3;
			}
			if (doorp[id2] == 0) {
				need[id2] = 10000 * (1 << (doorr[id2] - 1)) / ((1 << doorr[id2]) - 1);
			}
			else {
				need[id2] = 10000 / 3;
			}
			int ncol = 10000 - (10000 - need[id1]) * (10000 - need[id2]) / 10000;
			if (rand() % 10001 <= ncol) {
				create_vertical_path(k, id1, id2, r1->coor[1].x, r1->coor[0].y + 1, r1->coor[1].y - 1, r2->coor[0].x, r2->coor[0].y + 1, r2->coor[1].y - 1);
				doorp[id1]++;
				doorp[id2]++;
			}
			doorr[id1]--;
			doorr[id2]--;
		}
	}
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			int id1 = 3 * j + i, id2 = id1 + 3;
			Room *r1 = rooms[k] + id1, *r2 = rooms[k] + id2;
			if (doorp[id1] == 0) {
				need[id1] = 10000 * (1 << (doorr[id1] - 1)) / ((1 << doorr[id1]) - 1);
			}
			else {
				need[id1] = 10000 / 3;
			}
			if (doorp[id2] == 0) {
				need[id2] = 10000 * (1 << (doorr[id2] - 1)) / ((1 << doorr[id2]) - 1);
			}
			else {
				need[id2] = 10000 / 3;
			}
			int ncol = 10000 - (10000 - need[id1]) * (10000 - need[id2]) / 10000;
			if (10000 - rand() % 10001 <= ncol) {
				create_horizontal_path(k, id1, id2, r1->coor[1].y, r1->coor[0].x + 1, r1->coor[1].x - 1, r2->coor[0].y, r2->coor[0].x + 1, r2->coor[1].x - 1);
				doorp[id1]++;
				doorp[id2]++;
			}
			doorr[id1]--;
			doorr[id2]--;
		}
	}
}

void dfs(int id, int seen[], int k) {
	seen[id] = 1;
	for (int i = 0; i < psize[k]; i++) {
		if (paths[k][i].r1 == id && seen[paths[k][i].r2] == 0)
			dfs(paths[k][i].r2, seen, k);
		if (paths[k][i].r2 == id && seen[paths[k][i].r1] == 0)
			dfs(paths[k][i].r1, seen, k);
	}
}

int check_map(int k) {
	int num[ROOM_NUMBER] = {0};
	for (int i = 0; i < psize[k]; i++) {
		num[paths[k][i].r1]++;
		num[paths[k][i].r2]++;
	}
	int b = 0;
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (num[i] == 1) {
			b = 1;
		}
	}
	if (b == 0) {
		return 0;
	}
	int seen[ROOM_NUMBER] = {0};
	dfs(0, seen, k);
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (seen[i] == 0)
			return 0;
	}
	return 1;
}

static void reset(int k) {
	for (int i = 0; i < ROOM_NUMBER; i++) {
		free(rooms[k][i].coor);
		rooms[k][i].hidden = 0;
		rooms[k][i].type = 0;
	}
	for (int i = 0; i < psize[k]; i++) {
		free(paths[k][i].coor);
		paths[k][i].r1 = paths[k][i].r2 = paths[k][i].size = 0;
	}
	psize[k] = 0;
	for (int i = 0; i < rands[k]; i++) {
		rand_points[k][i].x = rand_points[k][i].y = 0;
	}
	rands[k] = 0;
	golds[k] = foods[k] = spells[k] = weapons[k] = monsters[k] = 0;
}

static void reset_all() {
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		reset(k);
	}
}

static void save_map(char* name) {
	FILE *fmap = fopen(name, "w");

	fprintf(fmap, "Player coor: 0 %d %d\n", start_coor.x, start_coor.y); // Floor, x, y
	fprintf(fmap, "Player State: %d %d 0 0\n", MAX_HEALTH, MAX_HUNGER); // health, hunger, fsize, ssize;
	fprintf(fmap, "Player finv:\n");
	fprintf(fmap, "Player sinv:\n");
	fprintf(fmap, "Weapons: 1 0 0 0 0\n");
	
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		fprintf(fmap, "Rooms:\n");
		for (int j = 0; j < ROOM_NUMBER; j++) {
			fprintf(fmap, "\t%d %d %d %d %d %d\n", rooms[k][j].coor[0].x, rooms[k][j].coor[0].y, rooms[k][j].coor[1].x, rooms[k][j].coor[1].y, rooms[k][j].type, rooms[k][j].hidden);
		}
		fprintf(fmap, "Paths number: %d\nPaths:\n", psize[k]);
		for (int j = 0; j < psize[k]; j++) {
			fprintf(fmap, "\tr1: %d, r2: %d, length: %d, coors: ", paths[k][j].r1, paths[k][j].r2, paths[k][j].size);
			for (int q = 0; q < paths[k][j].size; q++) {
				fprintf(fmap, "%d %d ", paths[k][j].coor[q].x, paths[k][j].coor[q].y);
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
		fprintf(fmap, "Monters number: %d\nMonsters:\n", monsters[k]);
		for (int i = 0; i < monsters[k]; i++) {
			fprintf(fmap, "%d %d %d %d %d %d\n", monster[k][i].type, monster[k][i].health, monster[k][i].able, monster[k][i].awareness, monster[k][i].coor.x, monster[k][i].coor.y);
		}
		fprintf(fmap, "Visibility:\n");
		for (int i = 3; i < RX - 1; i++) {
			for (int j = 1; j < RY - 1; j++) {
				fprintf(fmap, "0");
			}
			fprintf(fmap, "\n");
		}
	}
	
	fclose(fmap);
}

void create_stuff(int f) {
	// Gold
	if (f != 3) {
		int sum_gold = 1000 + f * 250, num = 9;
		while (num--) {
			set_non_treasure_random_point(&gold[f][golds[f]].coor, f);
			gold[f][golds[f]].type = 0;
			
			int room_id = inside_room(gold[f][golds[f]].coor, f);
			if (rooms[f][room_id].type == 3) {
				gold[f][golds[f]].value = 0;
				num++;
				golds[f]++;
				continue;
			}
			if (num) {
				gold[f][golds[f]].value = sum_gold / num / 2 + rand() % (sum_gold / num);
				if (gold[f][golds[f]].value > sum_gold) {
					gold[f][golds[f]].value = sum_gold / 2;
				}
			}
			else {
				gold[f][golds[f]].value = sum_gold;
			}

			sum_gold -= gold[f][golds[f]].value;
			golds[f]++;
		}
	}
	else {
		int sum_gold = 1500, num = 7;
		while (num--) {
			set_non_treasure_random_point(&gold[f][golds[f]].coor, f);
			gold[f][golds[f]].type = 0;

			int room_id = inside_room(gold[f][golds[f]].coor, f);
			if (rooms[f][room_id].type == 3) {
				gold[f][golds[f]].value = 0;
				num++;
				golds[f]++;
				continue;
			}
			if (num) {
				gold[f][golds[f]].value = sum_gold / num / 2 + rand() % (sum_gold / num);
				if (gold[f][golds[f]].value > sum_gold) {
					gold[f][golds[f]].value = sum_gold / 2;
				}
			}
			else {
				gold[f][golds[f]].value = sum_gold;
			}

			sum_gold -= gold[f][golds[f]].value;
			golds[f]++;
		}
		sum_gold = 1000, num = 5;
		while (num--) {
			set_treasure_random_point(&gold[f][golds[f]].coor);
			gold[f][golds[f]].type = 0;

			int room_id = inside_room(gold[f][golds[f]].coor, f);
			if (rooms[f][room_id].type == 3) {
				gold[f][golds[f]].value = 0;
				num++;
				golds[f]++;
				continue;
			}
			if (num) {
				gold[f][golds[f]].value = sum_gold / num / 2 + rand() % (sum_gold / num);
				if (gold[f][golds[f]].value > sum_gold) {
					gold[f][golds[f]].value = sum_gold / 2;
				}
			}
			else {
				gold[f][golds[f]].value = sum_gold;
			}
			sum_gold -= gold[f][golds[f]].value;
			golds[f]++;
		}
	}

	// Black Gold
	int sum_gold = 1000 + f * 250, num = 2;
	while (num--) {
		set_non_treasure_random_point(&gold[f][golds[f]].coor, f);
		gold[f][golds[f]].type = 1;

		int room_id = inside_room(gold[f][golds[f]].coor, f);
		if (rooms[f][room_id].type == 3) {
			gold[f][golds[f]].value = 0;
			num++;
			golds[f]++;
			continue;
		}
		if (num) {
			gold[f][golds[f]].value = sum_gold / num / 2 + rand() % (sum_gold / num);
			if (gold[f][golds[f]].value > sum_gold) {
				gold[f][golds[f]].value = sum_gold / 2;
			}
		}
		else {
			gold[f][golds[f]].value = sum_gold;
		}
		sum_gold -= gold[f][golds[f]].value;
		golds[f]++;
	}

	// Normal Food
	num = 8;
	while (num--) {
		set_random_point(&food[f][foods[f]].coor, f);
		food[f][foods[f]].type = 0;
		foods[f]++;
	}

	// Supreme Food
	num = 4;
	while (num--) {
		set_random_point(&food[f][foods[f]].coor, f);
		food[f][foods[f]].type = 1;
		foods[f]++;
	}

	// Magic Food
	num = 3;
	while (num--) {
		set_random_point(&food[f][foods[f]].coor, f);
		food[f][foods[f]].type = 2;
		foods[f]++;
	}
	
	// Rotten Food
	num = 3;
	while (num--) {
		set_random_point(&food[f][foods[f]].coor, f);
		food[f][foods[f]].type = 3;
		foods[f]++;
	}

	// Health Spell
	num = 3;
	while (num--) {
		set_random_point(&spell[f][spells[f]].coor, f);
		spell[f][spells[f]].type = 0;
		spells[f]++;
	}
	set_enchant_random_point(&spell[f][spells[f]].coor, f);
	spell[f][spells[f]].type = 0;
	spells[f]++;

	// Damage Spell
	num = 3;
	while (num--) {
		set_random_point(&spell[f][spells[f]].coor, f);
		spell[f][spells[f]].type = 1;
		spells[f]++;
	}
	set_enchant_random_point(&spell[f][spells[f]].coor, f);
	spell[f][spells[f]].type = 1;
	spells[f]++;

	// Speed Spell
	num = 3;
	while (num--) {
		set_random_point(&spell[f][spells[f]].coor, f);
		spell[f][spells[f]].type = 2;
		spells[f]++;
	}
	set_enchant_random_point(&spell[f][spells[f]].coor, f);
	spell[f][spells[f]].type = 2;
	spells[f]++;

	// Weapons
	// Mace: Already have one
	// Dagger
	num = 2;
	while (num--) {
		set_random_point(&weapon[f][weapons[f]].coor, f);
		weapon[f][weapons[f]].type = 1;
		weapon[f][weapons[f]].number = 10;
		weapons[f]++;
	}

	// Magic wand
	num = 1;
	while (num--) {
		set_random_point(&weapon[f][weapons[f]].coor, f);
		weapon[f][weapons[f]].type = 2;
		weapon[f][weapons[f]].number = 8;
		weapons[f]++;
	}

	// Arrow
	num = 2;
	while (num--) {
		set_random_point(&weapon[f][weapons[f]].coor, f);
		weapon[f][weapons[f]].type = 3;
		weapon[f][weapons[f]].number = 20;
		weapons[f]++;
	}

	// Sword
	if (f == 1) {
		set_random_point(&weapon[f][weapons[f]].coor, f);
		weapon[f][weapons[f]].type = 4;
		weapon[f][weapons[f]].number = 1;
		weapons[f]++;
	}

	// Monsters
	// Deamon
	for (int i = 0; i < Deamon_number[f]; i++) {
		set_random_point(&monster[f][monsters[f]].coor, f);
		monster[f][monsters[f]].type = 0;
		monster[f][monsters[f]].health = 5;
		monster[f][monsters[f]].able = true;
		monster[f][monsters[f]].awareness = 0;
		monsters[f]++;
	}

	// Fire Breathing Monster
	for (int i = 0; i < Fire_number[f]; i++) {
		set_random_point(&monster[f][monsters[f]].coor, f);
		monster[f][monsters[f]].type = 1;
		monster[f][monsters[f]].health = 10;
		monster[f][monsters[f]].able = true;
		monster[f][monsters[f]].awareness = 0;
		monsters[f]++;
	}

	// Giant
	for (int i = 0; i < Giant_number[f]; i++) {
		set_random_point(&monster[f][monsters[f]].coor, f);
		monster[f][monsters[f]].type = 2;
		monster[f][monsters[f]].health = 15;
		monster[f][monsters[f]].able = true;
		monster[f][monsters[f]].awareness = 0;
		monsters[f]++;
	}

	// Snake
	for (int i = 0; i < Snake_number[f]; i++) {
		set_random_point(&monster[f][monsters[f]].coor, f);
		monster[f][monsters[f]].type = 3;
		monster[f][monsters[f]].health = 20;
		monster[f][monsters[f]].able = true;
		monster[f][monsters[f]].awareness = 0;
		monsters[f]++;
	}

	// Undeed
	for (int i = 0; i < Undeed_number[f]; i++) {
		set_random_point(&monster[f][monsters[f]].coor, f);
		monster[f][monsters[f]].type = 4;
		monster[f][monsters[f]].health = 30;
		monster[f][monsters[f]].able = true;
		monster[f][monsters[f]].awareness = 0;
		monsters[f]++;
	}
}

char* make_map() {
	// char name[15] = "map";
	// int id = 1;
	// while (true) {
	// 	strcpy(name, "map");
	// 	sprintf(name + 3, "%d", id);
	// 	strcat(name, ".txt");

	// 	if (access(name, F_OK) == 0) {
	// 		id++;
	// 		continue;
	// 	}
	// 	break;
	// }
	char name[15] = "map.txt";
	srand(time(0) % 12491419);
	for (int k = 0; k < FLOOR_NUMBER; k++) {
		create_map(k);
		while (check_map(k) == 0) {
			reset(k);
			create_map(k);
		}

		define_rooms_type(k);
		create_stuff(k);
		if (k > 0)
			set_normal_random_point(inports + k, k);
		if (k + 1 < FLOOR_NUMBER) {
			set_normal_random_point(outports + k, k);
			while (k > 0 && inside_room(inports[k], k) == inside_room(outports[k], k)) {
				rands[k]--;
				set_normal_random_point(outports + k, k);
			}
		}
	}
	set_normal_random_point(&start_coor, 0);
	while (inside_room(start_coor, 0) == inside_room(outports[0], 0)) {
		rands[0]--;
		set_normal_random_point(&start_coor, 0);
	}

	save_map(name);
	reset_all();
	
	char *result = calloc(15, sizeof(char));
	strcpy(result, name);


	// printf("Pitty!");
	// exit(1);
	return result;
}

char* find_last_map() {
	return "map.txt";
	
	char name[15] = "map";
	int id = 0;

	while (true) {
		strcpy(name, "map");
		sprintf(name + 3, "%d", id + 1);
		strcat(name, ".txt");

		if (access(name, F_OK) == 0) {
			id++;
			continue;
		}
		break;
	}
	if (id == 0) {
		name[0] = 0;
	}
	else {
		strcpy(name, "map");
		sprintf(name + 3, "%d", id);
		strcat(name, ".txt");
	}

	char *result = calloc(15, sizeof(char));
	strcpy(result, name);
	return result;
}
