#include <stdlib.h>
#include "utilities.h"

typedef struct QueueElementstructure {
    unsigned index;
    struct QueueElementstructure *next;
    } QueueElement;
typedef QueueElement * PriorityQueue;

typedef struct{
    double f;
    double g;
    unsigned parent;
    char isOpen;
} AStarData;

int IsEmpty( PriorityQueue Pq );
unsigned extract_min(PriorityQueue *Pq);
void add_with_priority(unsigned index, PriorityQueue *Pq, AStarData *PathData);
void requeue_with_priority(unsigned index, PriorityQueue *Pq, AStarData *PathData);