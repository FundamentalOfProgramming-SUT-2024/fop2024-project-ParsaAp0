#include "header.h"
#include <time.h>
#define printf(...) printf(__VA_ARGS__); fflush(stdout)
#define ROOM_NUMBER 9

char map[X][Y];
int xt[ROOM_NUMBER], yt[ROOM_NUMBER], xd[ROOM_NUMBER], yd[ROOM_NUMBER];
int *coor[ROOM_NUMBER];
int limits[ROOM_NUMBER][4] = {
	{3, 15, 1, 50}, 
	{16, 30, 1, 60}, 
	{31, 43, 1, 70}, 
	{3, 20, 51, 120}, 
	{21, 33, 61, 100}, 
	{34, 43, 71, 120}, 
	{3, 13, 121, 184}, 
	{14, 28, 101, 184}, 
	{29, 43, 121, 184}
};
/*
X = 45
Y = 184
1: x: [3, 15], y: [1, 50]
2: x: [16, 30], y: [1, 60]
3: x: [31, 45], y: [1: 70]
4: x: [3, 20], y: [51, 120]
5: x: [21, 33], y: [61, 100]
6: x: [34, 45], y: [71: 120]
7: x: [3, 13], y: [121, 184]
8: x: [14, 28], y: [101, 184]
9: x: [29, 45], y: [121: 184]
*/

// int *create_room(int id, int uplim, int downlim, int leftlim, int rightlim) {
int *create_room(int *lim) {
	int *res = (int *) calloc(4, sizeof(int)), szx = 6 + rand() % (lim[1] - lim[0] - 6), szy = 6 + rand() % (lim[3] - lim[2] - 6);
	res[0] = lim[0] + rand() % (lim[1] - lim[0] - szx + 1);
	res[1] = res[0] + 5 + rand() % (szx - 5);
	res[2] = lim[2] + rand() % (lim[3] - lim[2] - szy + 1);
	res[3] = res[2] + 5 + rand() % (szy - 5);
	// int res[4] = {0, 0, 0, 0}, szx = 6 + rand() % (downlim - uplim - 6), szy = 6 + rand() % (rightlim - leftlim - 6);
	// res[0] = uplim + rand() % (downlim - uplim - szx + 1);
	// res[1] = res[0] + 5 + rand() % (szx - 5);
	// res[2] = leftlim + rand() % (rightlim - leftlim - szy + 1);
	// res[3] = res[2] + 5 + rand() % (szy - 5);
	return res;
}

void create_map() {
    srand(time(0));
    for (int i = 2; i < X - 2; i++) {
        for (int j = 0; j < Y; j++) {
            map[i][j] = ' ';
        }
    }
	for (int j = 0; j < Y; j++) {
		map[2][j] = '_';
		if (j != 0 || j != Y - 1) {
			map[X - 2][j] = '_';
		}
	}
	for (int i = 3; i < X - 2; i++) {
		map[i][0] = '|';
		map[i][Y - 1] = '|';
	}

    int T = 100;
	for (int i = 0; i < ROOM_NUMBER; i++) {
		coor[i] = create_room(limits[i]);
		int valid_room = 1;
		for (int j = 0; j < i; j++) {
            if ((coor[j][0] < coor[i][0] && coor[i][0] < coor[j][1]) || (coor[i][0] < coor[j][0] && coor[j][0] < coor[i][1])) {
            	if ((coor[j][2] < coor[i][2] && coor[i][2] < coor[j][3]) || (coor[i][2] < coor[j][2] && coor[j][2] < coor[i][3])) {
            	//if ((yt[i] < yt[nm] && yt[nm] < yd[i]) || (yt[nm] < yt[i] && yt[i] < yd[nm])) {
                    valid_room = 0;
                    break;
                }
            }
		}
		if (valid_room == 0) {
			free(coor[i]);
			i--;
			T--;
		}
	}
	printf("T: %d\n", T);
    
    for (int i = 0; i < ROOM_NUMBER; i++) {
		for (int xi = coor[i][0] + 1; xi < coor[i][1]; xi++) {
			for (int yi = coor[i][2] + 1; yi < coor[i][3]; yi++) {
				map[xi][yi] = '.';
			}
		}
		for (int yi = coor[i][2]; yi <= coor[i][3]; yi++) {
			map[coor[i][0]][yi] = '_';
			if (yi != coor[i][2] && yi != coor[i][3]) {
				map[coor[i][1]][yi] = '_';
			}
		}
		for (int xi = coor[i][0] + 1; xi <= coor[i][1]; xi++) {
			map[xi][coor[i][2]] = '|';
			map[xi][coor[i][3]] = '|';
		}
    }


}

int main() {
	create_map();
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
}
