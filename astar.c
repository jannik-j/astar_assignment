#include "utilities.h"
#include "priority_queue.h"
#include <math.h>
#include <limits.h>
#include <float.h>
#include <time.h>

double dijkstra(node *nodes, unsigned source, unsigned goal, unsigned long num_nodes, unsigned *parent);
void astar(node *nodes, unsigned source, unsigned goal, AStarData *PathData);
double haversine(node *node1, node *node2);
const double R = 6371e3;


int main(int argc, char *argv[]){
    if (argc != 2) ExitError("Please specify the path to the binary file to read as an argument", 10);

    unsigned long num_nodes;
    node *nodes = readBinary(argv[1], &num_nodes);

    //printNodesList(nodes, DEBUG_MAX_PRINT);
    computeValences(nodes, num_nodes, DEBUG_MAX_VALENCE);
    unsigned long start = 240949599UL;
    unsigned long end = 195977239UL;

    unsigned start_index = nodesearch(nodes, start, num_nodes);
    unsigned end_index = nodesearch(nodes, end, num_nodes);

/*
    unsigned *parent = (unsigned *) malloc(num_nodes*sizeof(unsigned));
    memset(parent, UINT_MAX, num_nodes*sizeof(unsigned));
    double distance = dijkstra(nodes, start_index, end_index, num_nodes, parent);
*/
    AStarData *PathData = (AStarData *) malloc(num_nodes*sizeof(AStarData));
    unsigned i;
    for (i=0; i<num_nodes; i++){
        PathData[i].f = DBL_MAX;
        PathData[i].g = DBL_MAX;
        PathData[i].isOpen = 0;
    }

    clock_t start_astar = clock();
    astar(nodes, start_index, end_index, PathData);
    clock_t end_astar = clock();

    printf("Path found with distance %lf in %lf seconds\n", PathData[end_index].g, ((double) end_astar-start_astar)/CLOCKS_PER_SEC);

/*
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
    */

    return(0);
}

double haversine(node *node1, node *node2){
    double lat1 = node1->lat;
    double lon1 = node1->lon;
    double lat2 = node2->lat;
    double lon2 = node2->lon;

    double phi1 = lat1*M_PI/180;
    double phi2 = lat2*M_PI/180;
    double delta_phi = (lat2-lat1)*M_PI/180;
    double delta_lambda = (lon2-lon1)*M_PI/180;

    double a = pow(sin(delta_phi/2), 2) + (cos(phi1)*cos(phi2)*pow(sin(delta_lambda/2), 2));
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return (R*c);
}

void astar(node *nodes, unsigned source, unsigned goal, AStarData *PathData){
    PriorityQueue Open = NULL;

    PathData[source].g = 0.0;
    PathData[source].parent = UINT_MAX;
    PathData[source].f = haversine(nodes+source, nodes+goal);
    add_with_priority(source, &Open, PathData);

    while(!IsEmpty(Open)){
        unsigned i;
        unsigned min_index = extract_min(&Open);
        if (min_index == goal)
            return;
        for(i=0; i<nodes[min_index].nsucc; i++){
            unsigned adj = nodes[min_index].successors[i];
            double g_adj = PathData[min_index].g + haversine(nodes+min_index, nodes+adj);
            if (g_adj < PathData[adj].g){
                PathData[adj].parent = min_index;
                double h_adj;
                if (PathData[min_index].g < DBL_MAX)
                    h_adj = PathData[adj].f - PathData[adj].g;
                else
                    h_adj = haversine(nodes+adj, nodes+goal);
                PathData[adj].f = g_adj + h_adj;
                PathData[adj].g = g_adj;
                if (!PathData[adj].isOpen)
                    add_with_priority(adj, &Open, PathData);
                else
                    requeue_with_priority(adj, &Open, PathData);
            }
        }
        PathData[min_index].isOpen = 0;        
    }   

}

/*
double dijkstra(node *nodes, unsigned source, unsigned goal, unsigned long num_nodes, unsigned *parent){ 
    PriorityQueue Pq = NULL;
    char *expanded = (char *) malloc(num_nodes*sizeof(char));
    memset(expanded, 0, num_nodes*sizeof(char));

    double *dist = (double *) malloc(num_nodes*sizeof(double));
    unsigned i;
    for (i=0; i<num_nodes; i++)
        dist[i] = DBL_MAX;
    
    dist[source] = 0.0;
    add_with_priority_old(source, &Pq, dist);

    while(!IsEmpty(Pq)){
        register unsigned i;
        unsigned min_index = extract_min(&Pq);
        char Is_adj_In_Pq;
        // double max_double = DBL_MAX;
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
                Is_adj_In_Pq = (dist[adj] < DBL_MAX);
                dist[adj] = dist_aux;
                parent[adj] = min_index;
                if(Is_adj_In_Pq)
                    decrease_priority(adj, &Pq, dist);
                else
                    add_with_priority_old(adj, &Pq, dist);
            }
        }
    }
    return -1.0;
}
*/