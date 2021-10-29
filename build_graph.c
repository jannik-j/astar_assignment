#include "utilities.h"

int main(int argc, char *argv[]){

    if (argc != 2) ExitError("Please specify the path to the .csv-file to read as an argument", 10);

    FILE *fp;
    size_t buffer_size = 0;
    size_t num_chars;
    char *buffer, *field = NULL;
    node *nodes;
    unsigned i;
    unsigned long num_nodes;
    int j;

    /* Open the file */
    printf("Opening file %s\n\n", argv[1]);
    fp = fopen(argv[1], "r");
    if (!fp) ExitError("Unable to open file", 11);
    /* Count number of nodes and allocate memory for the nodes vector */
    for (i=0; i<3; i++)
        getline(&buffer, &buffer_size, fp); // Discard the first three lines

    num_nodes = 0U;
    while (!feof(fp)){
        if (num_nodes%1000000 == 0)
            printf("Reading line %lu...\n", num_nodes);
        num_chars = getline(&buffer, &buffer_size, fp);
        // printf("%s", buffer);
        if (strncmp(buffer, "node", strlen("node")))
            break;
        else
            num_nodes++;
    }

    printf("Found %lu lines.\n\n", num_nodes);

    if ((nodes = (node*) malloc(num_nodes*sizeof(node))) == NULL)
        ExitError("Unable to allocate memory for the nodes", 12);

    /* Read the information line by line and save it in the struct */
    rewind(fp);
    for (i=0; i<3; i++)
        getline(&buffer, &buffer_size, fp); // Discard the first three lines

    i = 0U;
    while(!feof(fp) && (i<num_nodes)){
        if (i%1000000 == 0)
            printf("Extracting info of node %d...\n", i);
        num_chars = getline(&buffer, &buffer_size, fp);
        readNodeLine(buffer, nodes, i);
        nodes[i].successors = NULL;
        nodes[i].name = NULL;
        nodes[i].nsucc = 0U;
        i++;
    }
    printf("Got info of all nodes. \n\n");

    /* Read the information of the ways */
    int oneway = 0;
    int count = 1;
    int found;
    unsigned long prev_id, next_id;
    unsigned prev_pos, next_pos;
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
        prev_id = 0UL;
        prev_pos = NOTFOUND;
        while (field != NULL){
            if (!strcmp(field, ""))
                ExitError("Empty field while reading way", 13);
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
    //printf("\n=== Information of the first %d nodes in the list ===\n", DEBUG_MAX_PRINT);
    //printNodesList(nodes, DEBUG_MAX_PRINT);
    computeValences(nodes, num_nodes, DEBUG_MAX_VALENCE);
    writeBinary(nodes, num_nodes, argv[1]);
    fclose(fp);
}