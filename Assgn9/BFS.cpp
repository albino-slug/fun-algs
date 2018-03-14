// bfs
// use adjency lists

// eval : prettyprint the forrest (cause the graph is not necessarilly connected)
// graph vary E, vary V

#include <cmath>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <queue>

using namespace std;

#include "Profiler_linux.h"
Profiler profiler("BFS");

#define WHITE 0
#define GRAY  1
#define BLACK 2

#define DEMO_VERTICES_SIZE      10
#define DEMO_EDGES_SIZE         10
#define MAX_SIZE                201
#define HUGE_VALUE              2147483645

#define INDENT_SIZE 1000000

typedef struct e {
        int a;
        int b;
} edge;

typedef struct m {
        int        data;
        struct m **child;
        int        nb_of_children;
} node_multi;

edge edges[DEMO_EDGES_SIZE] = {{1, 3}, {1, 7}, {1, 10}, {2, 10}, {2, 3}, {10, 4}, {6, 2}, {6, 8}, {6, 10}, {7, 9}};
list<int> graph[MAX_SIZE];            // adjancy list of all veritices
int color[MAX_SIZE];
int d_time[MAX_SIZE];
queue<int> Q;


void pretty_print_R2(node_multi *n, char *indent, bool is_tail, bool is_x, bool is_null){
        // IF ROOT, INDENT ONLY
        if (is_x){
                printf("    ");
        }

        // ELSE, ADD THE BRANCHES
        if (!is_x){
                printf("%s", indent);
                if (is_tail){
                        printf("└── ");
                }
                else{
                        printf("├── ");
                }
                // IF KNOWN TO BE NULL, PRINT o
                if(is_null){
                        printf(" o \n");
                        return;
                }
        }

        // PRINT THE NODE DATA
        printf("%d\n", n->data);

        if (!is_x){
                if (is_tail){
                        strcat(indent, "     ");
                }
                else{
                        strcat(indent, "|    ");
                }
        }

        for(int i = n->nb_of_children; i > 0; i--){
                // MAKE O COPY OF THE INDENT - I KNOW THIS IS BAD CODING STYLE AND I AM ASHAMED
                char indent2[INDENT_SIZE];
                strcpy(indent2, indent);
                bool is_tail = false;

                if (n->child[i] != NULL){
                        if (n->child[i]->nb_of_children == 0 || i == 1){
                                is_tail = true;
                        }
                        char indent2[INDENT_SIZE];
                        strcpy(indent2, indent);
                        pretty_print_R2(n->child[i], indent2, is_tail, false, false);
                }
                else {
                        char indent2[INDENT_SIZE];
                        strcpy(indent2, indent);
                        pretty_print_R2(n->child[i],  indent2, true, false, true);
                }
        }
}

// ATTEMPTS TO PRETTY-PRINT THE 2ND REPRESENTATION
void print_R2(node_multi *x, int size){
        char indent[7] = "    ";
        pretty_print_R2(x, indent, false, true, false);
}

// CREATES A NEW NODE OF TYPE MULTI
node_multi *init_multi(int data, int nb_of_children){
        node_multi *fresh_node = (node_multi *)malloc(sizeof(node_multi));
        fresh_node->data  = data;

        // AT FIRST, ALL NODES HAVE THE FIELD NB_OF_CHILDREN = 0, BUT MAKE SPACE FOR ENOUGH NODES
        fresh_node->nb_of_children = 0;
        fresh_node->child = (node_multi **)malloc(sizeof(node_multi*) * nb_of_children);

        return (fresh_node);
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
        printf("[edge generator] am creat matricea de adiacenta;\n");

        for (int i = 0; i < E; i++){
                int a = 1 + rand() % V;
                int b = 1 + rand() % V;
                printf("incerc sa creez muchia %d, %d;\n", a, b);
                while (adj_mtrx[a][b]){
                        printf("\t\tmuchia %d, %d exista deja..;\n", a, b);
                        a = 1 + rand() % V;
                        b = 1 + rand() % V;
                }
                printf("[step %d] am creat muchia %d, %d;\n", i+1, a, b);
                e[i].a = a;
                e[i].b = b;
                adj_mtrx[a][b] = 1;
                adj_mtrx[b][a] = 1;
        }
        return e;
}

void BFS(list<int> *G, int start_vertex, int V, int *ops){
        //node_multi **arr_multi  = (node_multi **)malloc(sizeof(node_multi *) * V);
        printf("\n\t\t\t... entering BFS ...\n");
        for(int v = 1; v <= V; v++){
                color[v] = WHITE;
                d_time[v] = HUGE_VALUE;
                (*ops)++;
        }
        printf("have set all colors and times;\n");

        color[start_vertex] = GRAY;
        d_time[start_vertex] = 0;
        (*ops)++;
        Q.push(start_vertex);
        //node_multi  *n = init_multi(0, V);
        printf("just enqueued the %d;\n", start_vertex);
        while (!Q.empty()){
                int u = Q.front();
                Q.pop();
                printf("dequeued %d;\n", u);
                printf("handling all white children of %d;\n", u);
                for (list<int>::iterator v = G[u].begin(); v != G[u].end(); v++){
                        if (color[*v] == WHITE){
                                printf("\t\t\thandling %d\n", *v);
                                color[*v] = GRAY;
                                d_time[*v] = d_time[u] + 1;
                                (*ops)++;
                                Q.push(*v);
                        }
                }
                color[u] = BLACK;
                (*ops)++;
        }
        //return n;
}

int main(int argc, char **argv){
        // DEMO
        if (argc == 2 && strcmp(argv[1], "demo") == 0){
                int blind_ops = 0;
                //construct graph
                printf("\t\t\t... constructing the graph ...\n\n");
                for (int i = 0; i < DEMO_EDGES_SIZE; i++){
                        graph[edges[i].a].push_back(edges[i].b);
                        graph[edges[i].b].push_back(edges[i].a);
                }
                //print graph
                printf("\t\t\t... the graph is:\n");
                for (int i = 1; i <= DEMO_VERTICES_SIZE; i++){
                        printf("-> vertex %d is adjacent with: ", i);
                        print_list(graph[i]);
                }

                BFS(graph, 1, 10, &blind_ops);
                // HERE PRINT RESULT OF BFS
                //print_R2(arr, DEMO_VERTICES_SIZE);
                clear_graph();
                memset(color, 0, MAX_SIZE + 1);
                memset(d_time, 0, MAX_SIZE + 1);
        }

        if (argc == 2 && strcmp(argv[1], "generate") == 0){
                srand(time(0));
                printf("ce se intampla aici?\n");
                int E, V, Eops = 0, Vops = 0;
                //VARYING NUMBER OF VERTICES
                V = 200;
                for (E = 1000; E <= 5000; E += 100){

                        edge *e = generate_edges(E, V);
                        for (int i = 0; i < E; i++){
                                graph[e[i].a].push_back(e[i].b);
                                graph[e[i].b].push_back(e[i].a);
                        }

                        BFS(graph, 1, V, &Eops);

                        clear_graph();
                        memset(color, 0, MAX_SIZE + 1);
                        memset(d_time, 0, MAX_SIZE + 1);
                        free(e);

                        profiler.countOperation("EdgeCount", E, Eops);
                }

                // VARYING NUMBER OF EDGES
                E = 4000;
                for (V = 100; V <= 200; V +=  10){

                        edge *e = generate_edges(E, V);
                        for (int i = 0; i < E; i++){
                                graph[e[i].a].push_back(e[i].b);
                                graph[e[i].b].push_back(e[i].a);
                        }

                        BFS(graph, 1, V, &Vops);

                        clear_graph();
                        memset(color, 0, MAX_SIZE + 1);
                        memset(d_time, 0, MAX_SIZE + 1);
                        free(e);

                        profiler.countOperation("VertexCount", V, Vops);
                }

                profiler.showReport();
        }

        return (0);
}
