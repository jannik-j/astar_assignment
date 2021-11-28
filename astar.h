/*
Defines the functions to execute the A*-algorithm
*/

#include "binary_heap.h"
//#include "priority_queue.h"
#include <math.h>
#include <limits.h>
#include <float.h>

//Performs the astar algorithm on the graph represented by the nodes vector where
//source and goal are the index of the start and the end in this vector.
//The data needed is stored in the vector PathData.
void astar(node *nodes, unsigned source, unsigned goal, AStarData *PathData);

//The haversine function to compute the great circle distance on earth's surface
//between the nodes pointed to by node1 and node2.
double haversine(node *node1, node *node2);

//After the A*-algorithm is executed reconstructs the path from the node at start_index in the nodes vector
//to the one at end_index by retracing it using the parent information in the PathData.
//Saves the path as a csv-file of coordinates at filename.
void make_path(node *nodes, unsigned start_index, unsigned end_index, AStarData *PathData, const char *filename);

//Creates a vector of structs of type AStarData to use during the algorithm. The values in the structs are
//initialized.
AStarData *astar_init(unsigned long num_nodes);