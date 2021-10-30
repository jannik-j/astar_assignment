#include "priority_queue.h"
#include <math.h>
#include <limits.h>
#include <float.h>

void astar(node *nodes, unsigned source, unsigned goal, AStarData *PathData);
double haversine(node *node1, node *node2);
void make_path(node *nodes, unsigned start_index, unsigned end_index, AStarData *PathData, const char *filename);
AStarData *astar_init(unsigned long num_nodes);