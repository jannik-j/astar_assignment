#include <stdlib.h>
#include "utilities.h"

typedef struct QueueElementstructure {
    unsigned index;
    struct QueueElementstructure *left;
    unsigned left_size;
    struct QueueElementstructure *right;
    unsigned right_size;
    struct QueueElementstructure *parent;
    } QueueElement;
typedef QueueElement * PriorityQueue;

typedef struct{
    double f;
    double g;
    unsigned parent;
    char isOpen;
} AStarData;

void restore(QueueElement *q, AStarData *PathData);
QueueElement* find_element(unsigned index, PriorityQueue Pq);
int IsEmpty( PriorityQueue Pq );
unsigned extract_min(PriorityQueue *Pq, AStarData *PathData);
void add_with_priority(unsigned index, PriorityQueue *Pq, AStarData *PathData);
void requeue_with_priority(unsigned index, PriorityQueue *Pq, AStarData *PathData);