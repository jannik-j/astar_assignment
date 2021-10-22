#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned long id; // Node identification
    char *name;
    double lat, lon; // Node position
    unsigned short nsucc; // Number of node successors; i. e. length of successors
    unsigned long *successors;
} node;

void printNodesList(node *first, int length){
    int i;
    for (i=0; i<length; i++){
        printf("=== Node %d ===\n", i+1);
        printf("ID: %lu\n", first[i].id);
        printf("Latitude: %lf\n", first[i].lat);
        printf("Longitude: %lf\n", first[i].lon);
    }
}

int main(){
    size_t buffer_size = 32;
    size_t num_chars;
    char *buffer, *field;
    node *nodes;

    if ((buffer = (char*) malloc(buffer_size*sizeof(char))) == NULL){
        printf("Unable to allocate memory");
        exit(1);
    }

    FILE *fp = fopen("data/test_reading.txt", "r");
    if (!fp){
        printf("Unable to open file.");
        exit(1);
    }

    int num_lines = 0;
    while (!feof(fp)){
        num_chars = getline(&buffer, &buffer_size, fp);
        num_lines++;
    }

    if ((nodes = (node*) malloc(num_lines*sizeof(node))) == NULL){
        printf("Unable to allocate memory for the nodes.");
        exit(1);
    }

    rewind(fp);
    int i = 0;
    while(!feof(fp)){
        num_chars = getline(&buffer, &buffer_size, fp);
        field = strsep(&buffer, "|");
        if (strcmp(field, "node")){
            printf("No Node\n");
        }
        else{
            field = strsep(&buffer, "|");
            sscanf(field, "%lu", &(nodes[i].id));
            for (int j = 3; j<=9 ; j++){
                field = strsep(&buffer, "|");
            }
            field = strsep(&buffer, "|");
            sscanf(field, "%lf", &(nodes[i].lat));
            field = strsep(&buffer, "|");
            sscanf(field, "%lf", &(nodes[i].lon));
        }        
        i++;
    }
    printNodesList(nodes, num_lines);
}
