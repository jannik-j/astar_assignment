#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_MAX_PRINT 5
#define NOTFOUND 25000000
#define DEBUG_MAX_VALENCE 9

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

int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Please specify the path to the .csv-file to read as an argument\n");
        exit(1);
    }

    size_t buffer_size = 0;
    size_t num_chars;
    char *buffer, *field = NULL;
    node *nodes;
    unsigned i, num_nodes;
    int j;

    /* Open the file */
    FILE *fp = fopen(argv[1], "r");
    printf("Opening file %s\n\n", argv[1]);
    if (!fp){
        printf("Unable to open file.");
        exit(1);
    }

    /* Count number of nodes and allocate memory for the nodes vector */
    for (i=0; i<3; i++)
        getline(&buffer, &buffer_size, fp); // Discard the first three lines

    num_nodes = 0;
    while (!feof(fp)){
        if (num_nodes%1000000 == 0)
            printf("Reading line %d...\n", num_nodes);
        num_chars = getline(&buffer, &buffer_size, fp);
        // printf("%s", buffer);
        if (strncmp(buffer, "node", strlen("node")))
            break;
        else
            num_nodes++;
    }

    printf("Found %d lines.\n\n", num_nodes);

    if ((nodes = (node*) malloc(num_nodes*sizeof(node))) == NULL){
        printf("Unable to allocate memory for the nodes.");
        exit(1);
    }

    /* Read the information line by line and save it in the struct */
    rewind(fp);
    for (i=0; i<3; i++)
        getline(&buffer, &buffer_size, fp); // Discard the first three lines

    i = 0;
    while(!feof(fp) && (i<num_nodes)){
        if (i%1000000 == 0)
            printf("Extracting info of node %d...\n", i);
        num_chars = getline(&buffer, &buffer_size, fp);
        readNodeLine(buffer, nodes, i);
        nodes[i].successors = NULL;
        nodes[i].name = NULL;
        nodes[i].nsucc = 0;
        i++;
    }
    printf("Got info of all nodes. \n\n");

    /* Read the information of the ways */
    int oneway = 0;
    int count = 1;
    int found;
    unsigned long prev_id, next_id;
    unsigned long prev_pos, next_pos;
    num_chars = getline(&buffer, &buffer_size, fp);
    while(!strncmp(buffer, "way", 3)){
        if (count%100000 == 0)
            printf("Reading way %d\n", count);
        for (j=1; j<=8; j++){
            field = strsep(&buffer, "|");
        }
        if (!strcmp(field, "oneway")){
            oneway = 1;
            //printf("Oneway flag found\n");
        }

        field = strsep(&buffer, "|");
        field = strsep(&buffer, "|");
        prev_id = 0;
        prev_pos = NOTFOUND;
        while (field != NULL){
            if (!strcmp(field, ""))
                printf("Field is empty\n");
            sscanf(field, "%lu", &next_id);       
            field = strsep(&buffer, "|");
            //printf("Searching for node with id %lu\n", next_id);
            next_pos = nodesearch(nodes, next_id, num_nodes);
            if (next_pos == NOTFOUND){
                //printf("Node %lu not found\n", next_id);
                continue;
            }
            //printf("Found at position %u\n", next_pos);
            if (prev_pos != NOTFOUND){
                // Search, if the node at next_pos is already a successor of the node at prev_pos
                found = searchInSuccessors(nodes, prev_pos, next_id);
                if (!found){
                    //printf("Connecting nodes at positions %u and %u\n", prev_pos, next_pos);                    
                    linkNodes(nodes, prev_pos, next_pos);
                }
                // Do it the other way around if not oneway
                if (!oneway){
                    //printf("Flag oneway was not found \n");
                    found = searchInSuccessors(nodes, next_pos, prev_id);
                    if (!found){
                        //printf("Connecting nodes at positions %u and %u\n", next_pos, prev_pos);                
                        linkNodes(nodes, next_pos, prev_pos);
                    }
                }
            }
            prev_pos = next_pos;
            prev_id = next_id;
        }
        // DEBUG Skip to the end of the line for the next getline to work correctly
        // while(field != NULL)
        //     field = strsep(&buffer, "|");
        oneway = 0;
        num_chars = getline(&buffer, &buffer_size, fp);
        count++;
    }
    printf("\n=== Information of the first %d nodes in the list ===\n", DEBUG_MAX_PRINT);
    printNodesList(nodes, DEBUG_MAX_PRINT);
    computeValences(nodes, num_nodes, DEBUG_MAX_VALENCE);

    fclose(fp);
}

// Extract the info from the line in buffer and save it to the struct at position i in nodes.
void readNodeLine(char *buffer, node *nodes, int i){
    char *field;
    int j;
    field = strsep(&buffer, "|");
    if (strcmp(field, "node")){
        printf("No Node\n");
        exit(1);
    }
    else{
        field = strsep(&buffer, "|");
        sscanf(field, "%lu", &(nodes[i].id));
        for (j = 3; j<=9 ; j++){
            field = strsep(&buffer, "|");
        }
        field = strsep(&buffer, "|");
        sscanf(field, "%lf", &(nodes[i].lat));
        field = strsep(&buffer, "|");
        sscanf(field, "%lf", &(nodes[i].lon));
    }  
}

// Print the info of the first length structs.
void printNodesList(node *nodes, int length){
    int i, j;
    for (i=0; i<length; i++){
        printf("= Node %d =\n", i+1);
        printf("ID: %lu\n", nodes[i].id);
        printf("Latitude: %lf\n", nodes[i].lat);
        printf("Longitude: %lf\n", nodes[i].lon);
        printf("Number of successors: %u\n", nodes[i].nsucc);
        printf("IDs of successors: ");
        for (j=0; j<nodes[i].nsucc; j++)
            printf("%lu, ", nodes[nodes[i].successors[j]].id);
        printf("\n");
    }
}

// Search the position of the node with id id in the array of nodes.
// If it is not found, NOTFOUND is returned.
unsigned long nodesearch(node *nodes, unsigned long id, unsigned length){
    unsigned long first = 0;
    unsigned long last = length - 1;
    unsigned long middle = (first+last)/2;

    while (first <= last) {
        if (nodes[middle].id < id)
            first = middle + 1;
        else if (nodes[middle].id == id) {
            return(middle);
        }
        else
            last = middle - 1;
        middle = (first + last)/2;
        }

    return(NOTFOUND);
}

// Search, if the node with next_id is already in the succesors of the node at prev_pos in nodes.
// If yes, 1 is returned, otherwise 0.
int searchInSuccessors(node *nodes, unsigned long prev_pos, unsigned long next_id){
    int k;
    unsigned long *succs;
    succs = nodes[prev_pos].successors;
    for (k=0; k<nodes[prev_pos].nsucc; k++){
        if (nodes[succs[k]].id == next_id){
            return (1);
        }
    }
    return(0);
}

// Adds the position next_pos to the list of successors of the node at prev_pos and increments
// nodes[prev_pos].nsucc by 1.
void linkNodes(node *nodes, unsigned long prev_pos, unsigned long next_pos){
    nodes[prev_pos].successors = (unsigned long*) realloc(nodes[prev_pos].successors, (nodes[prev_pos].nsucc+1)*sizeof(unsigned long));
    if (nodes[prev_pos].successors == NULL){
        printf("Reallocation failed while linking nodes");
        exit(1);
    }
    nodes[prev_pos].successors[nodes[prev_pos].nsucc] = next_pos;
    nodes[prev_pos].nsucc++;
}

// Computes and displays the distribution of valences of the nodes.
void computeValences(node *nodes, unsigned length, int max_valence){
    int i;
    unsigned *valences;
    unsigned too_large = 0;
    if ((valences = (unsigned*) malloc((max_valence+1)*sizeof(unsigned))) == NULL){
        printf("Allocation of memory while computing valences failed\n");
        exit(1);
    }
    memset(valences, 0, (max_valence+1)*sizeof(unsigned));

    for (i=0; i<length; i++){
        if (nodes[i].nsucc <= max_valence)
            valences[nodes[i].nsucc]++;
        else
            too_large++;
    }

    printf("\n=== Valences of nodes found ===\n");
    for (i=0; i<=max_valence; i++){
        printf("Nodes with valence %d:\t%u\n", i, valences[i]);
    }
    printf("Nodes with valence higher than the max of %d: %u\n", max_valence, too_large);
}