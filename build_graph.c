#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_MAX_PRINT 5
#define NOTFOUND 25000000

typedef struct {
    unsigned long id; // Node identification
    char *name;
    double lat, lon; // Node position
    unsigned short nsucc; // Number of node successors; i. e. length of successors
    unsigned long *successors;
} node;

void readNodeLine(char *buffer, node *nodes, int i);
void printNodesList(node *first, int length);
unsigned nodesearch(node *first, unsigned long id, unsigned length);

int main(){
    size_t buffer_size = 0;
    size_t num_chars;
    char *buffer, *field = NULL;
    node *nodes;
    unsigned i, num_nodes;
    int j;

    FILE *fp = fopen("data/cataluna.csv", "r");
    if (!fp){
        printf("Unable to open file.");
        exit(1);
    }

    // Count number of nodes and allocate memory for the nodes vector
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

    printf("Found %d lines.\n", num_nodes);

    if ((nodes = (node*) malloc(num_nodes*sizeof(node))) == NULL){
        printf("Unable to allocate memory for the nodes.");
        exit(1);
    }

    // Read the information line by line and save it in the struct
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
        nodes[i].nsucc = 0;
        i++;
    }
    printNodesList(nodes, DEBUG_MAX_PRINT);

    // Read the information of the ways
    int oneway = 0;
    int count = 1;
    unsigned long next_id;
    unsigned int prev_pos, next_pos;
    num_chars = getline(&buffer, &buffer_size, fp);
    while(!strncmp(buffer, "way", 3)){
        if (count%100000 == 0)
            printf("Reading way %d\n", count);
        for (j=1; j<=8; j++){
            field = strsep(&buffer, "|");
        }
        if (!strcmp(field, "oneway"))
            oneway = 1;

        field = strsep(&buffer, "|");
        field = strsep(&buffer, "|");
        prev_pos = 0;
        while (field != NULL){
            sscanf(field, "%lu", &next_id);       
            field = strsep(&buffer, "|");
            next_pos = nodesearch(nodes, next_id, num_nodes);
            if (next_pos == 0) continue;
            if (prev_pos != 0){
                nodes[prev_pos].successors = realloc(nodes[prev_pos].successors, (nodes[prev_pos].nsucc+1)*sizeof(unsigned long));
                nodes[prev_pos].successors[nodes[prev_pos].nsucc] = next_pos;
                nodes[prev_pos].nsucc++;
                // TODO Check if successor already there and do it the other way around if oneway
            }
        }
        // DEBUG Skip to the end of the line for the next getline to work correctly
        while(field != NULL)
            field = strsep(&buffer, "|");
        num_chars = getline(&buffer, &buffer_size, fp);
        count++; 
    }
    //printf("Search Test: Node with id %d is at position %u", 416116776, nodesearch(nodes, 416116776, num_nodes));

    fclose(fp);
}

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

void printNodesList(node *first, int length){
    int i;
    for (i=0; i<length; i++){
        printf("=== Node %d ===\n", i+1);
        printf("ID: %lu\n", first[i].id);
        printf("Latitude: %lf\n", first[i].lat);
        printf("Longitude: %lf\n", first[i].lon);
    }
}

unsigned nodesearch(node *nodes, unsigned long id, unsigned length){
    unsigned first = 0;
    unsigned last = length - 1;
    unsigned middle = (first+last)/2;

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