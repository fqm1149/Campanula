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
			printf("%4d", map[i][j]);
		}
		printf("\n");
	}
}

//1.expand
//获取内点到边界的最短距离（外点赋值为1，内点2+）
int** GetInDisToBorder(Block** mapL1, int line, int column,int playernum) {
	int** resmap = malloc(line * sizeof(int*));
	for (int i = 0; i < line; i++) {
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
	for (int i = 0; i < line; i++) {
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
	for (int i = 0; i < line; i++) {
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
	for (int i = 0; i < line; i++) {
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
//获取点到某一中心的最短距离
int** GetDisToCenter(Block** mapL1, int line, int column, int playernum,int x,int y) {
	int** resmap = malloc(line * sizeof(int*));
	for (int i = 0; i < line; i++) {
		resmap[i] = malloc(column * sizeof(int));
		memset(resmap[i], 0, column * sizeof(int));
	}
	bool changed = 1;
	resmap[x][y] = 1;
	while (changed) {
		changed = 0;
		for (int i = 0; i < line; i++) for (int j = 0; j < column; j++) {
			if (resmap[i][j] == 0) {
				if (mapL1[i][j].type==PLAIN || mapL1[i][j].owner == playernum) {
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
//返回的是指向path头的地址
path_node* GetPathnode(Vector start, Vector end, Block** mapL1, int line, int column, int playernum) {
	int** map_d2end = GetDisToCenter(mapL1, line, column, playernum, end.x, end.y);
	if (map_d2end[start.x][start.y] == 0) {
		for (int i = 0; i < line; i++)free(map_d2end[i]);
		free(map_d2end);
		return NULL;
	}
	path_node* head = malloc(sizeof(path_node));
	head->previous = NULL;
	head->next = NULL;
	head->loc = start;
	head->remain_cnt = map_d2end[start.x][start.y];
	path_node* ptr = head;
	while (map_d2end[ptr->loc.x][ptr->loc.y] > 1) {
		int i = ptr->loc.x, j = ptr->loc.y;
		ptr->next = malloc(sizeof(path_node));
		if (i > 0) if (map_d2end[i - 1][j] < map_d2end[i][j] && map_d2end[i - 1][j] > 0) ptr->next->loc = (Vector){ i - 1,j };
		if (i < line - 1)if (map_d2end[i + 1][j] < map_d2end[i][j] && map_d2end[i + 1][j] > 0) ptr->next->loc = (Vector){ i + 1,j };
		if (j > 0) if (map_d2end[i][j - 1] < map_d2end[i][j] && map_d2end[i][j - 1] > 0)ptr->next->loc = (Vector){ i,j - 1 };
		if (j < column - 1) if (map_d2end[i][j + 1] < map_d2end[i][j] && map_d2end[i][j + 1] > 0)ptr->next->loc = (Vector){ i,j + 1 };
		ptr->next->remain_cnt = ptr->remain_cnt - 1;
		ptr->next->next = NULL;
		ptr->next->previous = ptr;
		ptr = ptr->next;
	}
	for (int i = 0; i < line; i++) free(map_d2end[i]);
	free(map_d2end);
	return head;
}

void FreePathFromTail(path_node* tail) {
	path_node* ptr;
	while (tail != NULL) {
		ptr = tail->previous;
		free(tail);
		tail = ptr;
	}
}
void FreePathFromHead(path_node* head) {
	path_node* ptr;
	while (head != NULL) {
		ptr = head->next;
		free(head);
		head = ptr;
	}
}

//计算路线权重
int PathWeight(path_node* head, Block** mapL1) {
	return (head->remain_cnt-1) * (mapL1[head->loc.x][head->loc.y].num);
}

//寻找最大值,0:startw,1:endw
Vector GetMaxWeight(WeighBlock** weightmap, int line, int column, int type_of_weight) {
	float maxweight = 0;
	Vector output = { 0,0 };
	if (type_of_weight == 0) {
		for (int i = 0; i < line; i++) {
			for (int j = 0; j < column; j++) {
				if (weightmap[i][j].startw > maxweight) {
					maxweight = weightmap[i][j].startw;
					output.x = i;
					output.y = j;
				}
			}
		}
	}
	if (type_of_weight == 1) {
		for (int i = 0; i < line; i++) {
			for (int j = 0; j < column; j++) {
				if (weightmap[i][j].endw > maxweight) {
					maxweight = weightmap[i][j].endw;
					output.x = i;
					output.y = j;
				}
			}
		}
	}
	return output;
}