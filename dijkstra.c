#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "vector.h"
#include "dijkstra.h"

int num_stations;
int src;
int dest;

vector* get_safest_shortest(int s, int d, int num_st, int** adj_matrix, station arr[])
{
    num_stations = num_st;
    src = s;
    dest = d;

    // initialising the 3 paths as vectors
    vector* path_1 = init_vector_ptr();
    vector* path_2 = init_vector_ptr();
    vector* path_3 = init_vector_ptr();

    // finding the safest path
    double danger_1 = first_safest(adj_matrix, path_1, arr);
    print_vector(path_1);

    // finding the second safest path
    double danger_2 = second_safest(adj_matrix, path_1, path_2, arr);
    print_vector(path_2);

    // finding the third safest path
    double danger_3 = third_safest(adj_matrix, path_1, path_2, path_3, arr);
    print_vector(path_3);

    // getting the shortest paths if 2 paths have the same danger value
    if(danger_1 == danger_2)
    {
        if(give_shortest_path(path_1, path_2, adj_matrix) == 1)
        {
            int tmp = danger_1;
            danger_1 = danger_2;
            danger_2 = tmp;
        }
    }

    if(danger_2 == danger_3)
    {
        if(give_shortest_path(path_2, path_3, adj_matrix) == 1)
        {
            int tmp = danger_2;
            danger_2 = danger_3;
            danger_3 = tmp;
        }
    }

    // printing the paths along with the danger value
    printf("Path-1 with danger_val = %.2lf is ", danger_1);
    print_vector(path_1);

    printf("Path-2 with danger_val = %.2lf is ", danger_2);
    print_vector(path_2);

    printf("Path-3 with danger_val = %.2lf is ", danger_3);
    print_vector(path_3);

    char ch;
    int path_num;
    printf("Do you want to traverse? (0/1): ");
    scanf(" %c", &ch);

    if(ch == 'Y')
    {
        printf("Which path do you want to go with (1/2/3): ");
        scanf(" %d", &path_num);

        if(path_num == 1)
        {
            //free the malloc'ed memory and return ptr to required path
            delete_vector_ptr(&path_2);
            delete_vector_ptr(&path_3);
            return path_1;
        }
        else if(path_num == 2)
        {
            //free the malloc'ed memory and return ptr to required path
            delete_vector_ptr(&path_1);
            delete_vector_ptr(&path_3);
            return path_2;
        }
        else
        {
            //free the malloc'ed memory and return ptr to required path
            delete_vector_ptr(&path_1);
            delete_vector_ptr(&path_2);
            return path_3;
        }
    }
    else
    {
        // freeing the malloc'ed memory
        delete_vector_ptr(&path_1);
        delete_vector_ptr(&path_2);
        delete_vector_ptr(&path_3);
        return NULL;
    }    

}

// gives the shortest path according to the total distance
int give_shortest_path(vector* S, vector* T, int matrix[][num_stations])
{
    int total_dist_1 = 0;
    int total_dist_2 = 0;

    // 'r' acts as the source and 's' as the destination
    int r,s;

    for(int i = 0 ; i < S->size - 1; i++)
    {
        r = S->arr[i];
        s = S->arr[i+1];

        total_dist_1 += matrix[r][s];
    }

    for(int i = 0 ; i < T->size - 1; i++)
    {
        r = T->arr[i];
        s = T->arr[i+1];

        total_dist_2 += matrix[r][s];
    }

    if(total_dist_2 < total_dist_1)
    {
        // swapping the pointers to the paths if second path is shorter than the first
        vector* tmp = init_vector_ptr();
        *tmp = *S;
        *S = *T;
        *T = *tmp;

        delete_vector_ptr(&tmp);

        return 1;
    }

    return 0;
}

// getting the safest path among all
double first_safest(int matrix[][num_stations], vector* P1, station arr[])
{
    double danger_1 =  dijkstra_safety(matrix, arr, P1);
    printf("%lf(danger_val) ", danger_1);
    return danger_1;
}

// picks out the safest station not yet traversed
int safest_station(double danger_path[], bool traversed[])
{
    double min = INT_MAX;
    int station_with_min_danger;

    for(int i = 0; i < num_stations; i++)
    {
        if(traversed[i] == false && danger_path[i] <= min)
        {
            min = danger_path[i];
            station_with_min_danger = i;
        }
    }

    return station_with_min_danger;
}

// dijkstra implementation using the danger value as the cost
double dijkstra_safety(int graph[num_stations][num_stations], station arr[], vector* P)
{
    double danger_val_path[num_stations];
    bool traversed[num_stations];
    int parent[num_stations];

    for(int i = 0; i < num_stations; i++)
    {
        danger_val_path[i] = INT_MAX;
        traversed[i] = false;
        parent[i] = -1;
    }

    danger_val_path[src] = 0;

    for(int i = 0; i < num_stations - 1; i++)
    {
        int u = safest_station(danger_val_path, traversed);
        traversed[u] = true;

        if(danger_val_path[u] != INT_MAX)
        {
            for(int j = 0; j < num_stations; j++)
            {
                if(traversed[j] == false && graph[u][j] > 0 && danger_val_path[u] + arr[j].dangerValue < danger_val_path[j])
                {
                    parent[j] = u;
                    danger_val_path[j] = danger_val_path[u] + arr[j].dangerValue;
                }
            }
        }
    }

    // getting the path using the parent array
    get_path(parent, P, dest);

    return danger_val_path[dest];
}

// gets the path using parent array
void get_path(int parent[], vector* P, int tmp_src)
{
    if(parent[tmp_src] == -1)
    {
        pushback(P, tmp_src);
        return;
    }
    get_path(parent, P, parent[tmp_src]);
    pushback(P, tmp_src);
}

// We use Yen's Algorithm to find the next shortest paths
double second_safest(int matrix[][num_stations], vector* P1, vector* P2, station arr[])
{
    // creating an array of vectors of size being the number of edges in P1
    vector* all_paths[P1->size - 1];

    // keeps track of the danger value of each path
    double total_danger_val[P1->size -1];

    for(int i = 0; i < P1->size - 1; i++)
    {
        int r = P1->arr[i];
        int s = P1->arr[i+1];

        all_paths[i] = init_vector_ptr();

        // removing the edge from the matrix
        int tmp = matrix[r][s];
        matrix[r][s] = 0;

        // finding the shortest path in this modified graph
        double danger_of_dest = dijkstra_safety(matrix, arr, all_paths[i]);

        matrix[r][s] = tmp;

        total_danger_val[i] = danger_of_dest; 
    }    

    double min = INT_MAX;
    int index = 0;

    // getting the path with the least danger value
    for(int i = 0; i < P1->size - 1; i++)
    {
        if(total_danger_val[i] < min)
        {
            index = i;
            min = total_danger_val[i];
        }
    }

    printf("%lf(danger_val) ", min);

    // Putting the safest path achieved in P2
    for(int i = 0; i < all_paths[index]->size; i++)
    {
        pushback(P2, all_paths[index]->arr[i]);
    }

    // freeing the memory allocated for each path
    for(int i = 0; i < P1->size - 1; i++)
    {
        delete_vector_ptr(&all_paths[i]);
    }

    return min;
}

double third_safest(int matrix[][num_stations], vector* P1, vector* P2, vector* P3, station arr[])
{
    // creating an array of vectors of size being the number of edges in P2
    vector* all_paths[P2->size - 1];

    // keeps track of the danger value of each path
    double total_danger_val[P2->size -1];

    for(int i = 0; i < P2->size - 1; i++)
    {
        int r = P2->arr[i];
        int s = P2->arr[i+1];

        all_paths[i] = init_vector_ptr();

        // removing the edge from the matrix
        int tmp = matrix[r][s];
        matrix[r][s] = 0;

        // finding the shortest path in this modified graph
        double danger_of_dest = dijkstra_safety(matrix, arr, all_paths[i]);

        // to remove the cases in which the 1st and 3rd paths are identical
        if(comp(P1, all_paths[i]) == 1)
        {
            danger_of_dest = INT_MAX;
        }
        
        matrix[r][s] = tmp;

        total_danger_val[i] = danger_of_dest;
    }    

    double min = INT_MAX;
    int index = 0;

    // getting the path with the least danger value
    for(int i = 0; i < P2->size - 1; i++)
    {
        if(total_danger_val[i] < min)
        {
            index = i;
            min = total_danger_val[i];
        }
    }

    printf("%lf(danger_val) ", min);

    // Putting the safest path achieved in P3
    for(int i = 0; i < all_paths[index]->size; i++)
    {
        pushback(P3, all_paths[index]->arr[i]);
    }

    // freeing the memory allocated for each path
    for(int i = 0; i < P2->size - 1; i++)
    {
        delete_vector_ptr(&all_paths[i]);
    }

    return min;
}

int comp(vector* P1, vector* P3)
{
    if(P1->size != P3->size)
    {
        return 0;
    }
    else
    {
        for(int i = 0; i < P1->size; i++)
        {
            if(P1->arr[i] != P3->arr[i])
            {
                return 0;
            }
        }
    }
    return 1;
}