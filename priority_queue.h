#include <stdlib.h>

typedef struct QueueElementstructure {
    unsigned index;
    struct QueueElementstructure *next;
    } QueueElement;
typedef QueueElement * PriorityQueue;

int IsEmpty( PriorityQueue Pq );
unsigned extract_min(PriorityQueue *Pq);
void add_with_priority(unsigned index, PriorityQueue *Pq, double *dist);
void decrease_priority(unsigned index, PriorityQueue *Pq, double *dist);