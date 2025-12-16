#define _CRT_SECURE_NO_WARNINGS
#define GEN_BOT_DEV
#include <stdlib.h>
#include <generals.h>
#include <stdio.h>
#include <errno.h>
#include <windows.h>
#include <threads.h>
#include <stdbool.h>
#include <campanula.h>

//starting point weight
//0.debug tools
void printIntMap(int** map, int line, int column,char *title) {
	printf("Print map titled %s\n", title);
	for (int i = 0; i < line; i++) {
		for (int j = 0; j < column; j++) {
			printf("%4d", &map[i][j]);
		}
		printf("\n");
	}
}

//1.expand
//获取内点到边界的最短距离（外点赋值为1，内点2+）
int** GetInDisToBorder(Block** mapL1, int line, int column,int playernum) {
	int** resmap = malloc(line * sizeof(int*));
	for (int i = 0; i < column; i++) {
		resmap[i] = malloc(column * sizeof(int));
		memset(resmap[i], 0, column * sizeof(int));
	}
	bool changed = 1;
	while (changed) {
		changed = 0;
		for (int i = 0; i < line; i++) for (int j = 0; j < column; j++) {
			if (resmap[i][j] == 0) {
				if (mapL1[i][j].owner != playernum) {
					resmap[i][j] = 1;
					changed = 1;
				}
				else {
					int tmpmin = line * column + 1;
					if (i > 0) if (resmap[i - 1][j] != 0 && resmap[i - 1][j] < tmpmin) tmpmin = resmap[i - 1][j];
					if (i < line - 1) if (resmap[i + 1][j] != 0 && resmap[i + 1][j] < tmpmin) tmpmin = resmap[i + 1][j];
					if (j > 0) if (resmap[i][j - 1] != 0 && resmap[i][j - 1] < tmpmin) tmpmin = resmap[i][j - 1];
					if (j < column - 1) if (resmap[i][j + 1] != 0 && resmap[i][j + 1] < tmpmin)tmpmin = resmap[i][j + 1];
					if (tmpmin < line * column + 1) {
						resmap[i][j] = tmpmin + 1;
						changed = 1;
					}
				}
			}
		}
	}
	return resmap;
}
//获得距离对手距离（对手赋值为1，内点2+）
int** GetInDisToEnemy(Block** mapL1, int line, int column, int playernum) {
	int** resmap = malloc(line * sizeof(int*));
	for (int i = 0; i < column; i++) {
		resmap[i] = malloc(column * sizeof(int));
		memset(resmap[i], 0, column * sizeof(int));
	}
	bool changed = 1;
	while (changed) {
		changed = 0;
		for (int i = 0; i < line; i++) for (int j = 0; j < column; j++) {
			if (resmap[i][j] == 0) {
				if (mapL1[i][j].owner > 0 && mapL1[i][j].owner != playernum) {
					resmap[i][j] = 1;
					changed = 1;
				}
				else {
					int tmpmin = line * column + 1;
					if (i > 0) if (resmap[i - 1][j] != 0 && resmap[i - 1][j] < tmpmin) tmpmin = resmap[i - 1][j];
					if (i < line - 1) if (resmap[i + 1][j] != 0 && resmap[i + 1][j] < tmpmin) tmpmin = resmap[i + 1][j];
					if (j > 0) if (resmap[i][j - 1] != 0 && resmap[i][j - 1] < tmpmin) tmpmin = resmap[i][j - 1];
					if (j < column - 1) if (resmap[i][j + 1] != 0 && resmap[i][j + 1] < tmpmin)tmpmin = resmap[i][j + 1];
					if (tmpmin < line * column + 1) {
						resmap[i][j] = tmpmin + 1;
						changed = 1;
					}
				}
			}
		}
	}
	return resmap;
}
//获得距离王城距离（王城赋值为1，其余2+）
int** GetInDisToHome(Block** mapL1, int line, int column, int playernum) {
	int** resmap = malloc(line * sizeof(int*));
	for (int i = 0; i < column; i++) {
		resmap[i] = malloc(column * sizeof(int));
		memset(resmap[i], 0, column * sizeof(int));
	}
	bool changed = 1;
	while (changed) {
		changed = 0;
		for (int i = 0; i < line; i++) for (int j = 0; j < column; j++) {
			if (resmap[i][j] == 0) {
				if (mapL1[i][j].owner ==playernum && mapL1[i][j].type == CROWN) {
					resmap[i][j] = 1;
					changed = 1;
				}
				else if(mapL1[i][j].owner==playernum) {
					int tmpmin = line * column + 1;
					if (i > 0) if (resmap[i - 1][j] != 0 && resmap[i - 1][j] < tmpmin) tmpmin = resmap[i - 1][j];
					if (i < line - 1) if (resmap[i + 1][j] != 0 && resmap[i + 1][j] < tmpmin) tmpmin = resmap[i + 1][j];
					if (j > 0) if (resmap[i][j - 1] != 0 && resmap[i][j - 1] < tmpmin) tmpmin = resmap[i][j - 1];
					if (j < column - 1) if (resmap[i][j + 1] != 0 && resmap[i][j + 1] < tmpmin)tmpmin = resmap[i][j + 1];
					if (tmpmin < line * column + 1) {
						resmap[i][j] = tmpmin + 1;
						changed = 1;
					}
				}
			}
		}
	}
	return resmap;
}

//2.arrival
//获取外点到边界的最短距离（内点赋值为1，外点2+）
int** GetExDisToBorder(Block** mapL1, int line, int column, int playernum) {
	int** resmap = malloc(line * sizeof(int*));
	for (int i = 0; i < column; i++) {
		resmap[i] = malloc(column * sizeof(int));
		memset(resmap[i], 0, column * sizeof(int));
	}
	bool changed = 1;
	while (changed) {
		changed = 0;
		for (int i = 0; i < line; i++) for (int j = 0; j < column; j++) {
			if (resmap[i][j] == 0) {
				if (mapL1[i][j].owner == playernum) {
					resmap[i][j] = 1;
					changed = 1;
				}
				else if(mapL1[i][j].type==PLAIN && mapL1[i][j].owner<=0) {
					int tmpmin = line * column + 1;
					if (i > 0) if (resmap[i - 1][j] != 0 && resmap[i - 1][j] < tmpmin) tmpmin = resmap[i - 1][j];
					if (i < line - 1) if (resmap[i + 1][j] != 0 && resmap[i + 1][j] < tmpmin) tmpmin = resmap[i + 1][j];
					if (j > 0) if (resmap[i][j - 1] != 0 && resmap[i][j - 1] < tmpmin) tmpmin = resmap[i][j - 1];
					if (j < column - 1) if (resmap[i][j + 1] != 0 && resmap[i][j + 1] < tmpmin)tmpmin = resmap[i][j + 1];
					if (tmpmin < line * column + 1) {
						resmap[i][j] = tmpmin + 1;
						changed = 1;
					}
				}
			}
		}
	}
	return resmap;
}
//获取外点到某一中心的最短距离
int** GetExDisToCenter(Block** mapL1, int line, int column, int playernum,int x,int y) {
	int** resmap = malloc(line * sizeof(int*));
	for (int i = 0; i < column; i++) {
		resmap[i] = malloc(column * sizeof(int));
		memset(resmap[i], 0, column * sizeof(int));
	}
	bool changed = 1;
	resmap[x][y] = 1;
	while (changed) {
		changed = 0;
		for (int i = 0; i < line; i++) for (int j = 0; j < column; j++) {
			if (resmap[i][j] == 0) {
				if (mapL1[i][j].type==PLAIN) {
					int tmpmin = line * column + 1;
					if (i > 0) if (resmap[i - 1][j] != 0 && resmap[i - 1][j] < tmpmin) tmpmin = resmap[i - 1][j];
					if (i < line - 1) if (resmap[i + 1][j] != 0 && resmap[i + 1][j] < tmpmin) tmpmin = resmap[i + 1][j];
					if (j > 0) if (resmap[i][j - 1] != 0 && resmap[i][j - 1] < tmpmin) tmpmin = resmap[i][j - 1];
					if (j < column - 1) if (resmap[i][j + 1] != 0 && resmap[i][j + 1] < tmpmin)tmpmin = resmap[i][j + 1];
					if (tmpmin < line * column + 1) {
						resmap[i][j] = tmpmin + 1;
						changed = 1;
					}
				}
			}
		}
	}
	return resmap;
}
