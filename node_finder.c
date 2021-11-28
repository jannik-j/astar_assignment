#include "astar.h"

node* findClosestNode(double lat, double lon, node* nodes, unsigned long num_nodes);

int main(int argc, char* argv[]){
    if (argc != 2)
        ExitError("Please specify the path to the binary file as an argument", 81);
    
    unsigned long num_nodes;
    clock_t start_binary = clock();
    node *nodes = readBinary(argv[1], &num_nodes);
    clock_t end_binary = clock();
    printf("Done in %lf seconds.\n\n", ((double) end_binary-start_binary)/CLOCKS_PER_SEC);

    double lat, lon;
    node* min_node;

    while(1){
        printf("Enter latitude: ");
        scanf("%lf", &lat);
        printf("Enter longitude: ");
        scanf("%lf", &lon);
        printf("Searching node...\n");
        min_node = findClosestNode(lat, lon, nodes, num_nodes);
    }
}

node* findClosestNode(double lat, double lon, node *nodes, unsigned long num_nodes){
    node *temp_node = (node*) malloc(sizeof(node));
    if (temp_node == NULL)
        ExitError("Error while allocating memory for the temporary node.", 82);
    temp_node->lat = lat;
    temp_node->lon = lon;

    unsigned i;
    double min_distance = DBL_MAX, distance;
    node *min_node;
    for (i=0; i<num_nodes; i++){
        distance = haversine(temp_node, nodes+i);
        if (distance<min_distance){
            min_node = nodes+i;
            min_distance = distance;
        }
    }
    printf("Closest node at (%lf, %lf) has ID %lu.\n", min_node->lat, min_node->lon, min_node->id);
    printf("Distance is %lfm\n", haversine(temp_node, min_node));
    return min_node;
}