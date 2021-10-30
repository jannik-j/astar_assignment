#include "priority_queue.h"

int IsEmpty(PriorityQueue Pq){
    return ( Pq == NULL );
}

unsigned extract_min(PriorityQueue *Pq){
    PriorityQueue first = *Pq;
    unsigned index = first->index;
    //printf("Removed index %u from the queue\n", index);
    *Pq = (*Pq)->next;
    free(first);
    return index;
}


void add_with_priority(unsigned index, PriorityQueue *Open, AStarData *PathData){
    QueueElement *aux = (QueueElement *) malloc(sizeof(QueueElement));
    if(aux == NULL)
        exit(66);

    aux->index = index;
    double costv = PathData[index].f;
    PathData[index].isOpen = 1;

    if (*Open == NULL || !(costv > PathData[(*Open)->index].f)) {
        aux->next = *Open;
        *Open = aux;
        return;
    }
    register QueueElement * q;
    for(q = *Open; q->next && PathData[q->next->index].f < costv; q = q->next );
    aux->next = q->next; q->next = aux;
    return;
}

void add_with_priority_old(unsigned index, PriorityQueue *Pq, double *dist){ 
    QueueElement *aux = (QueueElement *) malloc(sizeof(QueueElement));
    if(aux == NULL)
        exit(66);

    aux->index = index;
    double costv = dist[index];
    if (*Pq == NULL || !(costv > dist[(*Pq)->index])) {
        aux->next = *Pq;
        *Pq = aux;
        return;
    }
    register QueueElement * q;
    for(q = *Pq; q->next && dist[q->next->index] < costv; q = q->next );
    aux->next = q->next; q->next = aux;
    return;
}

void requeue_with_priority(unsigned index, PriorityQueue *Open, AStarData *PathData){
    if((*Open)->index == index)
        return;

    double costv = PathData[index].f;
    if(!(costv > PathData[(*Open)->index].f)){
        register QueueElement *prepv;
        for(prepv = *Open; prepv->next->index != index; prepv = prepv->next);
        QueueElement *swap = *Open;
        *Open=prepv->next; prepv->next=prepv->next->next; (*Open)->next=swap;
        return;
    }

    register QueueElement *q, *prepv;
    for(q = *Open; PathData[q->next->index].f < costv; q = q->next );
    if(q->next->index == index)
        return;
    for(prepv = q->next; prepv->next->index != index; prepv = prepv->next);
    QueueElement *pv = prepv->next;
    prepv->next = pv->next; pv->next = q->next; q->next = pv;
    return;
}

void decrease_priority(unsigned index, PriorityQueue *Pq, double *dist){
    if((*Pq)->index == index)
        return;

    double costv = dist[index];
    if(!(costv > dist[(*Pq)->index])){
        register QueueElement *prepv;
        for(prepv = *Pq; prepv->next->index != index; prepv = prepv->next);
        QueueElement *swap = *Pq;
        *Pq=prepv->next; prepv->next=prepv->next->next; (*Pq)->next=swap;
        return;
    }

    register QueueElement *q, *prepv;
    for(q = *Pq; dist[q->next->index] < costv; q = q->next );
    if(q->next->index == index)
        return;
    for(prepv = q->next; prepv->next->index != index; prepv = prepv->next);
    QueueElement *pv = prepv->next;
    prepv->next = pv->next; pv->next = q->next; q->next = pv;
    return;
}