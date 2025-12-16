#ifndef CAMPANULA_H
#define CAMPANULA_H

typedef struct Vector {
	int x;
	int y;
}Vector;

typedef struct path_node {
	Vector loc;
	struct path_node* next;
	struct path_node* previous;
	//倒数第几步即显示为几
	int remain_cnt;
}path_node;

typedef struct WeighBlock {
	float startw;
	float endw;
}WeighBlock;
//系数宏
#define EXPAND_WEIGHT 1


#endif
