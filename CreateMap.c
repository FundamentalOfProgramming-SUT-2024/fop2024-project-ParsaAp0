#include "header.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#define printf(...) printf(__VA_ARGS__); fflush(stdout)

// extern char[RX][RY];
static Room rooms[ROOM_NUMBER];
static Path paths[PATH_NUMBER];
static int psize = 0;
static Coor start_coor;

static int limits[ROOM_NUMBER][4] = {
	{3, 14, 1, 48},
	{18, 28, 1, 48},
	{32, RX - 1, 1, 48},
	{3, 14, 53, 98},
	{18, 28, 53, 98},
	{32, RX - 1, 53, 98},
	{3, 14, 103, RY - 1},
	{18, 28, 103, RY - 1},
	{32, RX - 1, 103, RY - 1}
};
/*
X = 45
Y = 150
1: x: [3, 15], y: [1, 50]
2: x: [16, 30], y: [1, 50]
3: x: [31, 43], y: [1: 50]
4: x: [3, 15], y: [51, 100]
5: x: [16, 30], y: [51, 100]
6: x: [31, 43], y: [51: 100]
7: x: [3, 15], y: [101, 150]
8: x: [16, 30], y: [101, 150]
9: x: [31, 43], y: [101: 150]
*/

Coor *create_room(int *lim) { // coor: {x1, x2, y1, y2}
	Coor *res = (Coor *) calloc(2, sizeof(Coor));
	int szx = 6 + rand() % (lim[1] - lim[0] - 5), szy = 6 + rand() % (lim[3] - lim[2] - 5);
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

void create_vertical_path(int id1, int id2, int tx, int tly, int Try, int dx, int dly, int dry) {
	paths[psize].r1 = id1;
	paths[psize].r2 = id2;
	int up = tly + rand() % (Try - tly), down = dly + rand() % (dry - dly);
	if (up == down) {
		int ptr = tx;
		while (ptr <= dx) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			co[0].x = ptr;
			co[0].y = up;
			add_path_char(paths + psize, co[0]);
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
			add_path_char(paths + psize, co[0]);
			ptr++;
		}

		if (up < down) {
			for (int p = up + 1; p < down; p++) {
				Coor *co = (Coor *) malloc(sizeof(Coor));
				// int *co = (int *) malloc(2 * sizeof(int));
				co[0].x = mid;
				co[0].y = p;
				add_path_char(paths + psize, co[0]);
			}

		}
		else {
			for (int p = up - 1; p > down; p--) {
			// for (int p = down + 1; p < up; p++) {
				Coor *co = (Coor *) malloc(sizeof(Coor));
				// int *co = (int *) malloc(2 * sizeof(int));
				co[0].x = mid;
				co[0].y = p;
				add_path_char(paths + psize, co[0]);
			}
		}
		
		ptr--;
		while (ptr <= dx) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			// int *co = (int *) malloc(2 * sizeof(int));
			co[0].x = ptr;
			co[0].y = down;
			add_path_char(paths + psize, co[0]);
			ptr++;
		}
	}
	psize++;
}

void create_horizontal_path(int id1, int id2, int ly, int ltx, int ldx, int ry, int rtx, int rdx) {
	paths[psize].r1 = id1;
	paths[psize].r2 = id2;
	int left = ltx + rand() % (ldx - ltx), right = rtx + rand() % (rdx - rtx); 
	// int up = tly + rand() % (Try - tly), down = dly + rand() % (dry - dly);
	if (left == right) {
		int ptr = ly;
		while (ptr <= ry) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			// int *co = (int *) malloc(2 * sizeof(int));
			co[0].x = left;
			co[0].y = ptr;
			add_path_char(paths + psize, co[0]);
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
			add_path_char(paths + psize, co[0]);
			ptr++;
		}

		if (left < right) {
			for (int p = left + 1; p < right; p++) {
				Coor *co = (Coor *) malloc(sizeof(Coor));
				// int *co = (int *) malloc(2 * sizeof(int));
				co[0].x = p;
				co[0].y = mid;
				add_path_char(paths + psize, co[0]);
			}
		}

		else {
			for (int p = left - 1; p > right; p--) {
			// for (int p = right + 1; p < left; p++) {
				Coor *co = (Coor *) malloc(sizeof(Coor));
				// int *co = (int *) malloc(2 * sizeof(int));
				co[0].x = p;
				co[0].y = mid;
				add_path_char(paths + psize, co[0]);
			}
		}
		ptr--;
		while (ptr <= ry) {
			Coor *co = (Coor *) malloc(sizeof(Coor));
			// int *co = (int *) malloc(2 * sizeof(int));
			co[0].x = right;
			co[0].y = ptr;
			add_path_char(paths + psize, co[0]);
			ptr++;
		}
	}
	psize++;
}

void create_map() {

	for (int i = 0; i < ROOM_NUMBER; i++) {
		rooms[i].coor = create_room(limits[i]);
	}
	int doorp[ROOM_NUMBER] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int doorr[ROOM_NUMBER] = {2, 3, 2, 3, 4, 3, 2, 3, 2};
	int need[ROOM_NUMBER] = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			int id1 = 3 * i + j, id2 = id1 + 1;
			Room *r1 = rooms + id1, *r2 = rooms + id2;
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
				create_vertical_path(id1, id2, r1->coor[1].x, r1->coor[0].y + 1, r1->coor[1].y - 1, r2->coor[0].x, r2->coor[0].y + 1, r2->coor[1].y - 1);
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
			Room *r1 = rooms + id1, *r2 = rooms + id2;
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
				create_horizontal_path(id1, id2, r1->coor[1].y, r1->coor[0].x + 1, r1->coor[1].x - 1, r2->coor[0].y, r2->coor[0].x + 1, r2->coor[1].x - 1);
				doorp[id1]++;
				doorp[id2]++;
			}
			doorr[id1]--;
			doorr[id2]--;
		}
	}

}

void dfs(int id, int seen[]) {
	seen[id] = 1;
	for (int i = 0; i < psize; i++) {
		if (paths[i].r1 == id && seen[paths[i].r2] == 0)
			dfs(paths[i].r2, seen);
		if (paths[i].r2 == id && seen[paths[i].r1] == 0)
			dfs(paths[i].r1, seen);
	}
}

int check_map() {
	int seen[ROOM_NUMBER] = {0};
	dfs(0, seen);
	for (int i = 0; i < ROOM_NUMBER; i++) {
		if (seen[i] == 0)
			return 0;
	}
	return 1;
}

void set_start_point() {
	int room = rand() % 9;
	start_coor.x = rooms[room].coor[0].x + 1 + rand() % (rooms[room].coor[1].x - rooms[room].coor[0].x - 2);
	start_coor.y = rooms[room].coor[0].y + 1 + rand() % (rooms[room].coor[1].y - rooms[room].coor[0].y - 2);
}

static void reset_all() {
	for (int i = 0; i < ROOM_NUMBER; i++) {
		free(rooms[i].coor);
	}
	for (int i = 0; i < psize; i++) {
		free(paths[i].coor);
		paths[i].r1 = paths[i].r2 = paths[i].size = 0;
	}
	psize = 0;
}

static void save_map(char* name, Room* rooms, Path* paths, Coor start_coor, int psize) {
	FILE *fmap = fopen(name, "w");
	fprintf(fmap, "Rooms:\n");
	for (int j = 0; j < ROOM_NUMBER; j++) {
		fprintf(fmap, "\t%d %d %d %d\n", rooms[j].coor[0].x, rooms[j].coor[0].y, rooms[j].coor[1].x, rooms[j].coor[1].y);
	}
	fprintf(fmap, "Paths number: %d\nPaths:\n", psize);
	for (int j = 0; j < psize; j++) {
		fprintf(fmap, "\tr1: %d, r2: %d, length: %d, coors: ", paths[j].r1, paths[j].r2, paths[j].size);
		for (int k = 0; k < paths[j].size; k++) {
			fprintf(fmap, "%d %d ", paths[j].coor[k].x, paths[j].coor[k].y);
		}
		fprintf(fmap, "\n");
	}
	fprintf(fmap, "start coor: %d %d\n", start_coor.x, start_coor.y);
	fprintf(fmap, "Visibility:\n");
	for (int i = 3; i < RX - 1; i++) {
		for (int j = 1; j < RY - 1; j++) {
			fprintf(fmap, "1");
		}
		fprintf(fmap, "\n");
	}
	
	fclose(fmap);
}

char* make_map() {
	reset_all();
	char name[15] = "map";
	int id = 1;
	while (true) {
		strcpy(name, "map");
		sprintf(name + 3, "%d", id);
		strcat(name, ".txt");

		if (access(name, F_OK) == 0) {
			id++;
			continue;
		}
		break;
	}

	srand(time(0) % 12491419 * id);
	create_map();
	while (check_map() == 0) {
		for (int i = 0; i < psize; i++) {
			// for (int j = 0; j < paths[i].size; j++)
			// 	free(paths[i].coor[j]);
			free(paths[i].coor);
			paths[i].size = 0;
		}
		psize = 0;
		for (int i = 0; i < ROOM_NUMBER; i++)
			free(rooms[i].coor);
		create_map();
	}
	set_start_point();

	save_map(name, rooms, paths, start_coor, psize);
	
	
	char *result = calloc(15, sizeof(char));
	strcpy(result, name);
	return result;
}


char* find_last_map() {
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
/*
void print_in_map() {
	for (int i = 2; i < RX; i++) {
	for (int j = 0; j < RY; j++) {
			map[i][j] = ' ';
		}
	}
	for (int j = 0; j < RY; j++) {
		map[2][j] = '_';
		if (j != 0 || j != RY - 1) {
			map[RX - 1][j] = '_';
		}
	}
	for (int i = 3; i < RX; i++) {
		map[i][0] = '|';
		map[i][RY - 1] = '|';
	}
	for (int i = 0; i < ROOM_NUMBER; i++) {
		print_room(rooms + i);
	}
	for (int i = 0; i < psize; i++) {
		print_path(paths + i);
	}
}

int main() {
	srand(time(0));
	create_map();
	while (check_map() == 0) {
		for (int i = 0; i < psize; i++) {
			for (int j = 0; j < paths[i].size; j++)
				free(paths[i].coor[j]);
			free(paths[i].coor);
			paths[i].size = 0;
		}
		psize = 0;
		for (int i = 0; i < ROOM_NUMBER; i++)
			free(rooms[i].coor);
		create_map();
	}
	print_in_map();
	for (int i = 0; i < RX; i++) {
		for (int j = 0; j < RY; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}
*/