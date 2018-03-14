#include <cmath>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <list>

using namespace std;

#include "Profiler_linux.h"
Profiler profiler("DFS");

#define WHITE 0
#define GRAY  1
#define BLACK 2

#define DEMO_VERTICES_SIZE      10
#define DEMO_EDGES_SIZE         10
#define MAX_SIZE                201

typedef struct e {
        int a;
        int b;
} edge;

edge edges[DEMO_EDGES_SIZE] = {{1, 3}, {1, 7}, {1, 10}, {2, 10}, {2, 3}, {10, 4}, {6, 2}, {6, 8}, {6, 10}, {7, 9}};
list<int> graph[MAX_SIZE];            // adjancy list of all veritices
int color[MAX_SIZE];
int d_time[MAX_SIZE];
int f_time[MAX_SIZE];

list<int> top_sorted;
list<int> top_f_times;

void visit(list<int> G[], int v, int *current_time, int *ops){
        (*current_time)++;
        d_time[v] = *current_time;
        // start processing the vertex v
        color[v] = GRAY;
        (*ops) += 2;
        for (list<int>::iterator it = G[v].begin(); it != G[v].end(); it++){
                (*ops)++;
                if (color[(*it)] == WHITE){
                        (*ops)++;
                        //printf("\t\t..visiting %d\n", *it);
                        visit(G, (*it), current_time, ops);
                }
        }

        color[v] = BLACK;
        (*current_time)++;
        f_time[v] = *current_time;
        (*ops) += 2;
        // finished processing the vertex v
}

void DFS(list<int> G[], int V, int *ops){
        int current_time = 0;

        for (int i = 1; i <= V; i++){

                (*ops)++;
                if (color[i] == WHITE){
                        //printf("visiting %d\n", i);
                        visit(G, i, &current_time, ops);
                }
        }
}

void topological_sort(list<int> G[]){
        int blind_ops = 0;
        printf("\t\t\t\t... running DFS ...\n");
        DFS(G, DEMO_VERTICES_SIZE, &blind_ops); // creates the finishing times array

        // sorting the order of finishing times
        printf("\t\t\t... sorting the vertices ...\n");
        for(int i = 1; i < DEMO_VERTICES_SIZE; i++){
                int max_f_time = 0;
                int vertex = 0;
                for (int j = 1; j < DEMO_VERTICES_SIZE; j++){
                        if (f_time[j] > max_f_time){
                                max_f_time = f_time[j];
                                vertex = j;
                        }
                }
                // put vertex and time in their place in list
                top_sorted.push_back(vertex);
                top_f_times.push_back(max_f_time);
                f_time[vertex] = 0;
        }
        printf("\t\t\t... constructing the list ...\n");
}

void print_list(list<int> L){
        for (list<int>::iterator it = L.begin(); it != L.end(); it++){
                printf("%d, ", *it);
        }
        printf("\n");
}

void clear_graph(){
        for(int i = 0; i < MAX_SIZE; i++){
                graph[i].clear();
        }
}

edge *generate_edges(int E, int V){
        int **adj_mtrx = (int **)malloc(sizeof(int *) * (V + 1));
        edge *e = (edge *)malloc(sizeof(edge) * E);

        for (int i = 0; i <= V; i++){
                adj_mtrx[i] = (int *)malloc(sizeof(int) * (V + 1));
                memset(adj_mtrx[i], 0, (V+1) * (sizeof(int)));
        }

        for (int i = 0; i < E; i++){
                int a = 1 + rand() % V;
                int b = 1 + rand() % V;
                while (adj_mtrx[a][b]){
                        a = 1 + rand() % V;
                        b = 1 + rand() % V;
                }
                e[i].a = a;
                e[i].b = b;
                adj_mtrx[a][b] = 1;
                //printf("(%d)[%d->%d]\n", i+1, a, b);
        }
        return e;
}

int main(int argc, char **argv){

        if (argc == 2 && strcmp(argv[1], "demo") == 0){

                //construct graph
                printf("\t\t\t... constructing the graph ...\n\n");
                for (int i = 0; i < DEMO_EDGES_SIZE; i++){
                        graph[edges[i].a].push_back(edges[i].b);
                }
                //print graph
                printf("\t\t\t... the graph is:\n");
                for (int i = 1; i <= DEMO_VERTICES_SIZE; i++){
                        printf("-> vertex %d is adjacent with: ", i);
                        print_list(graph[i]);
                }

                printf("\t\t\t... topologically sorting the vertices ...\n");
                topological_sort(graph);
                list<int>::iterator it1 = top_sorted.begin();
                list<int>::iterator it2 = top_f_times.begin();
                printf("-> pair is [vertex, finishing time];\n->");
                for (; it1 != top_sorted.end() && it2 != top_f_times.end(); it1++, it2++){
                        printf("[%d, %d], ", *it1, *it2);
                }
                printf("\n");
                clear_graph();
                memset(color, 0, MAX_SIZE + 1);
                memset(d_time, 0, MAX_SIZE + 1);
                memset(f_time, 0, MAX_SIZE + 1);
        }

        if (argc == 2 && strcmp(argv[1], "generate") == 0){
                srand(time(0));
                int E, V, Eops = 0, Vops = 0;
                // VARYING NUMBER OF VERTICES
                V = 100;
                for (E = 1000; E <= 5000; E += 100){
                        //Eops = 0;

                        edge *e = generate_edges(E, V);
                        for (int i = 0; i < E; i++){
                                graph[e[i].a].push_back(e[i].b);
                        }
                        DFS(graph, V, &Eops);
                        clear_graph();
                        memset(color, 0, V + 1);
                        memset(d_time, 0, V + 1);
                        memset(f_time, 0, V + 1);
                        free(e);

                        profiler.countOperation("EdgeCount", E, Eops);
                }

                // VARYING NUMBER OF EDGES
                E = 9000;
                for (V = 100; V <= 200; V +=  10){
                        //Vops = 0;

                        edge *e = generate_edges(E, V);
                        for (int i = 0; i < E; i++){
                                graph[e[i].a].push_back(e[i].b);
                        }
                        DFS(graph, V, &Vops);

                        // for (int i = 1; i <= V; i++){
                        //         printf("color[%d]=%d\n", i, color[i]);
                        // }

                        clear_graph();
                        memset(color, 0, V + 1);
                        memset(d_time, 0, V + 1);
                        memset(f_time, 0, V + 1);
                        free(e);

                        profiler.countOperation("VertexCount", V, Vops);
                }

                //profiler.createGroup("VaryingEdges", "VaryingVertices");
                profiler.showReport();
        }
        return 0;
}
