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
}path_node;

typedef struct WeighBlock {
	float startw;
	float endw;
}WeighBlock;



#endif
