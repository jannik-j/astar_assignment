#include "utilities.h"
#include "priority_queue.h"
#include <math.h>
#include <limits.h>
#include <float.h>

double dijkstra(node *nodes, unsigned source, unsigned goal, unsigned num_nodes, unsigned *parent);
double haversine(node node1, node node2);
const double R = 6371e3;

int main(int argc, char *argv[]){
    if (argc != 2) ExitError("Please specify the path to the binary file to read as an argument", 10);

    unsigned num_nodes;
    node *nodes = readBinary(argv[1], &num_nodes);

    //printNodesList(nodes, DEBUG_MAX_PRINT);
    computeValences(nodes, num_nodes, DEBUG_MAX_VALENCE);

    unsigned long start = 240949599;
    unsigned long end = 195977239;

    unsigned start_index = nodesearch(nodes, start, num_nodes);
    unsigned end_index = nodesearch(nodes, end, num_nodes);

    unsigned *parent = (unsigned *) malloc(num_nodes*sizeof(unsigned));
    memset(parent, UINT_MAX, num_nodes*sizeof(unsigned));
    double distance = dijkstra(nodes, start_index, end_index, num_nodes, parent);

    printf("Found way from id %lu to id %lu with length %lf\n", start, end, distance);
    unsigned index;
    int path_count = 0;
    for (index=end_index; index != start_index; index = parent[index])
        path_count++;
    path_count++;

    unsigned *path = (unsigned *) malloc(path_count*sizeof(unsigned));
    int i = 0;
    node current;
    for (index=end_index; index != UINT_MAX; index = parent[index]){
        path[i] = index;
        i++;
    }

    char filename[257];
    char basename[50];
    sprintf(basename, "_%lu_%lu.csv", start, end);
    strcpy(filename, argv[1]);
    strcpy(strrchr(filename, '.'), basename);
    FILE *fout = fopen(filename, "w");
    fprintf(fout, "lon,lat\n");

    for (i=path_count-1; i>=0; i--){
        current = nodes[path[i]];
        //printf("ID: %lu \t Lat: %lf \t Lon: %lf\n", current.id, current.lat, current.lon);
        fprintf(fout, "%lf,%lf\n", current.lon, current.lat);
    }

    return(0);
}

double haversine(node node1, node node2){
    double lat1 = node1.lat;
    double lon1 = node1.lon;
    double lat2 = node2.lat;
    double lon2 = node2.lon;

    double phi1 = lat1*M_PI/180;
    double phi2 = lat2*M_PI/180;
    double delta_phi = (lat2-lat1)*M_PI/180;
    double delta_lambda = (lon2-lon1)*M_PI/180;

    double a = pow(sin(delta_phi/2), 2) + (cos(phi1)*cos(phi2)*pow(sin(delta_lambda/2), 2));
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return (R*c);
}

double dijkstra(node *nodes, unsigned source, unsigned goal, unsigned num_nodes, unsigned *parent){ 
    PriorityQueue Pq = NULL;
    char *expanded = (char *) malloc(num_nodes*sizeof(char));
    memset(expanded, 0, num_nodes*sizeof(char));

    double *dist = (double *) malloc(num_nodes*sizeof(double));
    unsigned i;
    for (i=0; i<num_nodes; i++)
        dist[i] = DBL_MAX;
    
    dist[source] = 0.0;
    add_with_priority(source, &Pq, dist);

    while(!IsEmpty(Pq)){
        register unsigned i;
        unsigned min_index = extract_min(&Pq);
        expanded[min_index] = 1;
        if (min_index == goal)
            return dist[min_index];
        for(i=0; i < nodes[min_index].nsucc; i++){
            unsigned adj = nodes[min_index].successors[i];
            //printf("Looking at successor %u\n", adj);
            if(expanded[adj])
                continue;
            double dist_aux = dist[min_index] + haversine(nodes[min_index], nodes[adj]);
            //printf("%lf\n", dist_aux);
            if(dist[adj] > dist_aux){
                char Is_adj_In_Pq = dist[adj] < 0;
                dist[adj] = dist_aux;
                parent[adj] = min_index;
                if(Is_adj_In_Pq)
                    decrease_priority(adj, &Pq, dist);
                else
                    add_with_priority(adj, &Pq, dist);
            }
        }
    }
    return -1.0;
}