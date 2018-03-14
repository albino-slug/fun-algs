#include <cmath>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>

#define max_nodes 10000
#define max_edges 40000

using namespace std;

#include "Profiler_linux.h"
Profiler profiler("Kruskal");

typedef struct s{
        int data;
        int rank;
        struct s *parent;
} node;

typedef struct e{
        int a;
        int b;
        int weight;
} edge;

bool representative(node *x){
        return (x == x->parent);
}

edge *make_edge(int a, int b){
        edge *e = (edge *)malloc(sizeof(edge));
        e->a = a;
        e->b = b;
        e->weight = rand() % 101;
        return (e);
}

node *make_set(int data, int *om){
        node *x = (node*)malloc(sizeof(node));
        x->data = data;
        x->rank = 0;
        x->parent = x;
        (*om)++;
        return (x);
}

node *find_set(node *x, int *of){
        (*of)++;
        if (!representative(x)){
                x->parent = find_set(x->parent, of);
                (*of)++;
        }
        return (x->parent);
}

node *link(node *x, node *y, int *ou){
        (*ou)++;
        if (x->rank > y->rank){
                y->parent = x;
                (*ou)++;
                return (x);
        }
        else{
                x->parent = y;
                (*ou) += 2;
                if (x->rank == y->rank){
                        y->rank = y->rank + 1;
                        (*ou)++;
                }
                return (y);
        }
}

node *unite(node *x, node *y, int *ou, int *of){
        return (link(find_set(x, of), find_set(y, of), ou));
}

void Kruskal(edge *answer, edge *e, int nodes_count, int edges_count, int *ou, int *om, int *of){
        node **nodes = (node **)malloc(sizeof(node*) * nodes_count);
        int answer_index = 0;

        // create all the nodes
        for(int i = 0; i < nodes_count; i++){
                nodes[i] = make_set(i, om);
        }

        // sort the edges by weight (insertion sort)
        for (int i = 1; i < edges_count; i++){
                edge aux = e[i];
                int k = i - 1;
                while ((aux.weight < e[k].weight) && (k >= 0)){
                        e[k+1] = e[k];
                        k--;
                }
                e[k + 1] = aux;
        }

        // add the edges, one by one (if they dont create cycles)
        for(int i = 0; i < edges_count; i++){
                if (find_set(nodes[e[i].a], of) != find_set(nodes[e[i].b], of)){
                        answer[answer_index] = e[i];
                        answer_index++;
                        unite(nodes[e[i].a], nodes[e[i].b], ou, of);
                }
        }
}

int get_random_node(int max){
        return (rand() % max);
}

int main(int argc, char **argv){
        srand(time(0));

        if (argc == 3 && strcmp(argv[1], "demo") == 0){
                int blind_o = 0;
                int size  = atoi(argv[2]);
                printf("... creating %d sets; the sets are:\n  ", size);
                node **set = (node **)malloc(sizeof(node *) * size);
                for (int i = 0; i < size; i++){
                        set[i] = make_set(i, &blind_o);
                        printf("%d, ", i);
                }
                printf("\n\n... creating unions of 2 nodes; the unions and their sets are:\n  ");
                node **unions = (node **)malloc(sizeof(node *) * (size / 2));
                for (int i = 0; i < size / 2; i++){
                        unions[i] = unite(set[i], set[size - i - 1], &blind_o, &blind_o);
                        printf("[%d, %d], the set is %d;\n  ", i, size - i - 1, find_set(unions[i], &blind_o)->data);
                        free(set[i]);
                        free(set[size - i - 1]);
                }
                printf("\n... freeing everything ...\n");
        }

        if (argc == 2 && strcmp(argv[1], "generate")== 0){
                for (int n = 100; n < 10000; n += 100){
                        int om = 0, ou = 0, of = 0;
                        node sets[max_nodes];
                        edge edges[max_edges];
                        edge answer[max_edges];
                        int  **edge_check = (int **)malloc(sizeof(int *) * n);
                        for (int i = 0; i < n; i++){
                                edge_check[i] = (int *)calloc(n, sizeof(int));
                        }

                        // iterate once, making sure the graph is connected by making a chain of all the nodes
                        sets[0] = *make_set(0, &om);
                        for (int i = 1; i < n; i++){
                                sets[i] = *make_set(i, &om);
                                unite(&sets[i - 1], &sets[i], &ou, &of);
                                edges[i - 1] = *make_edge(i - 1, n - i - 1);
                                edge_check[i - 1][i] = edge_check[i][i - 1] = 1;
                        }

                        // creating the rest of the random edges with random weights
                        int a = 0, b = 0;
                        for (int i = n; i < (n * 4); i++){
                                while (a == b || edge_check[a][b]){
                                        a = get_random_node(n);
                                        b = get_random_node(n);
                                }
                                unite(&sets[a], &sets[b], &ou, &of);
                                edges[i] = *make_edge(a, b);
                                edge_check[a][b] = edge_check[b][a] = 1;
                        }

                        // creating the minimum spanning tree
                        Kruskal(answer, edges, n, n * 4, &ou, &om, &of);

                        for (int i = 0; i < n; i++){
                                free(edge_check[i]);
                        }
                        free(edge_check);

                        profiler.countOperation("MakeSet", n, om);
                        profiler.countOperation("Union",   n, ou);
                        profiler.countOperation("FindSet", n, of);
                }
        }
        profiler.createGroup("ComputationalEffort", "MakeSet", "Union", "FindSet");
        profiler.showReport();
        return (0);
}
