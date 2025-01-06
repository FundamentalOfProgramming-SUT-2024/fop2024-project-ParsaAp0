#include "header.h"
#include <time.h>
#define printf(...) printf(__VA_ARGS__); fflush(stdout)
#define ROOM_NUMBER 9
#define PATH_NUMBER 12
#define RX 43
#define RY 150


typedef struct {
	int *coor;
	// char vert_wall, horz_wall, inside;
} Room;
typedef struct {
	// int made;
	int r1, r2;
	int **coor, size;
} Path;


char map[RX][RY];
Room rooms[ROOM_NUMBER];
Path paths[PATH_NUMBER];
int psize = 0;

int limits[ROOM_NUMBER][4] = {
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

int *create_room(int *lim) {
	int *res = (int *) calloc(4, sizeof(int)), szx = 6 + rand() % (lim[1] - lim[0] - 5), szy = 6 + rand() % (lim[3] - lim[2] - 5);
	res[0] = lim[0] + rand() % (lim[1] - lim[0] - szx + 1);
	res[1] = res[0] + 5 + rand() % (szx - 5);
	res[2] = lim[2] + rand() % (lim[3] - lim[2] - szy + 1);
	res[3] = res[2] + 5 + rand() % (szy - 5);
	return res;
}

void add_path_char(Path *v, int *c) {
	if ((*v).size == 0)
		(*v).coor = (int **) malloc(((*v).size + 1) * sizeof(int *));
	else
		(*v).coor = (int **) realloc((*v).coor, ((*v).size + 1) * sizeof(int *));
	(*v).coor[(*v).size] = c;
	(*v).size++;
}

void create_vertical_path(int id1, int id2, int tx, int tly, int Try, int dx, int dly, int dry) {
	paths[psize].r1 = id1;
	paths[psize].r2 = id2;
	int up = tly + rand() % (Try - tly), down = dly + rand() % (dry - dly);
	if (up == down) {
		int ptr = tx;
		while (ptr <= dx) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = ptr;
			co[1] = up;
			add_path_char(paths + psize, co);
			ptr++;
		}
	}
	else {
		int mid = (tx + dx) / 2, ptr = tx;
		while (ptr <= mid) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = ptr;
			co[1] = up;
			add_path_char(paths + psize, co);
			ptr++;
		}

		for (int p = up + 1; p < down; p++) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = mid;
			co[1] = p;
			add_path_char(paths + psize, co);
		}

		for (int p = down + 1; p < up; p++) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = mid;
			co[1] = p;
			add_path_char(paths + psize, co);
		}
		ptr--;
		while (ptr <= dx) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = ptr;
			co[1] = down;
			add_path_char(paths + psize, co);
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
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = left;
			co[1] = ptr;
			add_path_char(paths + psize, co);
			ptr++;
		}
	}
	else {
		int mid = (ly + ry) / 2, ptr = ly;
		while (ptr <= mid) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = left;
			co[1] = ptr;
			add_path_char(paths + psize, co);
			ptr++;
		}

		for (int p = left + 1; p < right; p++) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = p;
			co[1] = mid;
			add_path_char(paths + psize, co);
		}

		for (int p = right + 1; p < left; p++) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = p;
			co[1] = mid;
			add_path_char(paths + psize, co);
		}
		ptr--;
		while (ptr <= ry) {
			int *co = (int *) malloc(2 * sizeof(int));
			co[0] = right;
			co[1] = ptr;
			add_path_char(paths + psize, co);
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
				create_vertical_path(id1, id2, r1->coor[1], r1->coor[2] + 1, r1->coor[3] - 1, r2->coor[0], r2->coor[2] + 1, r2->coor[3] - 1);
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
				create_horizontal_path(id1, id2, r1->coor[3], r1->coor[0] + 1, r1->coor[1] - 1, r2->coor[2], r2->coor[0] + 1, r2->coor[1] - 1);
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

void print_room(Room *v) {
	for (int xi = v->coor[0] + 1; xi < v->coor[1]; xi++) {
		for (int yi = v->coor[2] + 1; yi < v->coor[3]; yi++) {
			map[xi][yi] = '.';
		}
	}
	for (int yi = v->coor[2]; yi <= v->coor[3]; yi++) {
		map[v->coor[0]][yi] = '_';
		if (yi != v->coor[2] && yi != v->coor[3]) {
			map[v->coor[1]][yi] = '_';
		}
	}
	for (int xi = v->coor[0] + 1; xi <= v->coor[1]; xi++) {
		map[xi][v->coor[2]] = '|';
		map[xi][v->coor[3]] = '|';
	}
}

void print_path(Path *v) {
	for (int i = 1; i < -1 + (*v).size; i++) {
		map[(*v).coor[i][0]][(*v).coor[i][1]] = '#';
	}
	map[(*v).coor[0][0]][(*v).coor[0][1]] = '+';
	map[(*v).coor[(*v).size - 1][0]][(*v).coor[(*v).size - 1][1]] = '+';
}

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
