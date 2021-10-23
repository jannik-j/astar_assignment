#include "utilities.h"

int main(int argc, char *argv[]){
    if (argc != 2) ExitError("Please specify the path to the binary file to read as an argument", 10);

    unsigned num_nodes;
    node *nodes = readBinary(argv[1], &num_nodes);

    printNodesList(nodes, DEBUG_MAX_PRINT);
    computeValences(nodes, num_nodes, DEBUG_MAX_VALENCE);

    return(0);
}