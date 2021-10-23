#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOTFOUND 25000000
#define DEBUG_MAX_VALENCE 9
#define DEBUG_MAX_PRINT 5

typedef struct {
    unsigned long id; // Node identification
    char *name;
    double lat, lon; // Node position
    unsigned short nsucc; // Number of node successors; i. e. length of successors
    unsigned long *successors;
} node;

void readNodeLine(char *buffer, node *nodes, int i);
void printNodesList(node *nodes, int length);
unsigned long nodesearch(node *first, unsigned long id, unsigned length);
int searchInSuccessors(node *first, unsigned long prev_pos, unsigned long next_id);
void linkNodes(node *nodes, unsigned long prev_pos, unsigned long next_pos);
void computeValences(node *nodes, unsigned length, int max_valence);
void ExitError(const char *miss, int errcode);
void writeBinary(node *nodes, unsigned num_nodes, const char *filename);
node *readBinary(const char* filename, unsigned *num_nodes_arg);