#include <cmath>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

#include "Profiler_linux.h"
Profiler profiler("MultiWayTree");

#define INDENT_SIZE 1000000

typedef struct b {
        int       data;
        struct b *left;
        struct b *right;
} node_bin;

typedef struct m {
        int        data;
        struct m **child;
        int        nb_of_children;
} node_multi;

// ATTEMPTS TO PRINT THE TREE IN A PRETTY FASHION
void pretty_print_R3(node_bin *n, char *indent, bool is_tail, bool is_x, bool is_null){
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
                        strcat(indent, "    ");
                }
                else{
                        strcat(indent, "|   ");
                }
        }

        // MAKE O COPY OF THE INDENT - I KNOW THIS IS BAD CODING STYLE AND I AM ASHAMED
        char indent2[INDENT_SIZE];
        strcpy(indent2, indent);

        // RECURSIVE CALLS ON THE RIGHT-HAND SIDE
        if (n->right != NULL){
                pretty_print_R3(n->right, indent, false, false, false);
        }
        else {
                pretty_print_R3(n->right,  indent, true, false, true);
        }

        // RECURSIVE CALLS ON THE LEFT-HAND SIDE
        if (n->left != NULL){
                pretty_print_R3(n->left,  indent2, true, false, false);
        }
        else {
                pretty_print_R3(n->left,  indent2, true, false, true);
        }
}

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

void print_R1(int *R1, int size){
        printf("node:    ");
        for(int i = 1; i < size; i++){
                if (R1[i] == -1){
                        printf(" %d    ", i);
                }
                else {
                        printf("%d  ", i);
                }
        }
        printf("\nparent:  ");
        for(int i = 1; i < size; i++){
                if (R1[i] == -1){
                        printf("root  ");
                }
                else{
                        printf("%d  ", R1[i]);
                }
        }
        printf("\n\n");
}

// ATTEMPTS TO PRETTY-PRINT THE 2ND REPRESENTATION
void print_R2(node_multi *x, int size){
        char indent[7] = "    ";
        pretty_print_R2(x, indent, false, true, false);
}

// ATTEMPTS TO PRETTY-PRINT THE 3RD REPRESENTATION
void print_R3(node_bin *x, int size){
        char indent[7] = "    ";
        pretty_print_R3(x, indent, false, true, false);
}

// SETS A NODE AS THE CHILD OF ANOTHER
void set_child(node_multi *x, int index, node_multi *child){
        x->child[index] = child;
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

// TRANSFORMS THE TREE REPRESENTATION FROM VECTOR TO MULTIWAY
node_multi *T1(int *R1, int size){
        node_multi **arr_multi  = (node_multi **)malloc(sizeof(node_multi *) * size);
        node_multi  *n = init_multi(0, size);      // TO BE RETURNED AS THE ROOT OF THE NEWLY CREATED MULTIWAY TREE

        // ITERATION 1 CREATES ALL THE NODES
        for(int i = 1; i < size; i++){
                arr_multi[i] = init_multi(i, size);
        }

        // ITERATION 2 CREATES ALL THE PARENT-CHILD CONNECTIONS
        for(int i = 1; i < size; i++){
                if (R1[i] != -1){
                        // INCREMENT THE NUMBER OF CHILDREN, THEN SET THE CHILD ON THE NEW POSITION
                        arr_multi[R1[i]]->nb_of_children++;
                        set_child(arr_multi[R1[i]], arr_multi[R1[i]]->nb_of_children, arr_multi[i]);
                }
                else{
                        n = arr_multi[i];
                }
        }
        return (n);
}

// CREATES A NEW NODE OF TYPE BINARY
node_bin *init_bin(int data, node_bin *left, node_bin *right){
        node_bin *fresh_node = (node_bin *)malloc(sizeof(node_bin));

        fresh_node->data  = data;
        fresh_node->left  = left;
        fresh_node->right = right;

        return (fresh_node);
}

// creates a linked list made up of the children of a node - UNUSED (FOR NOW)
node_bin *create_children_list(int nb_of_children, node_multi **ch){
        node_bin *head    = init_bin(0, NULL, NULL);
        node_bin *walker  = init_bin(0, NULL, NULL);
        int i = 1;
        head->data = ch[i]->data;
        walker = head;
        i++;
        while(ch[i] != NULL){
                node_bin *aux = init_bin(ch[i]->data, NULL, NULL);
                walker->right = aux;
                walker = aux;
                i++;
        }
        return (head);
}

// SECOND TRANSFORMATION
node_bin *T2(node_multi *x, bool is_root){
        node_bin *current = init_bin(x->data, NULL, NULL);
        node_bin *aux = (node_bin*)malloc(sizeof(node_bin));

        if (x->nb_of_children == 0){
                return(current);
        }

        // adding the first sibling
        current->left = T2(x->child[1], false);

        // adding the rest of the siblings
        aux = current->left;
        for (int i = 2; i <= x->nb_of_children; i++){
                aux->right = T2(x->child[i], false);
                aux = aux->right;
        }

        return (current);
}

int main(int argc, char **argv){

        // CREATE FIRST REPRESENTATION
        printf("... creating the tree in R1 ...\n\n");
        int size = argc;
        int *R1 = (int *)malloc(sizeof(int) * size);

        for (int i = 1; i < size; i++){
                R1[i] = atoi(argv[i]);
        }
        printf("... R1 (vector representation) is:\n");
        // DISPLAY FIRST REPRESENTATION
        print_R1(R1, size);

        // TRANSFORM FROM R1 IN R2 BY T1
        printf("\n... applying T1 ...\n\n");
        // DISPLAY R2
        printf("... R2 (multi-way representation) is:\n");
        node_multi *R2 = T1(R1, size);
        print_R2(R2, size);

        // TRANSFORM FROM R2 IN R3 BY T2
        printf("\n... applying T2 ...\n\n");
        // DISPLAY R3
        printf("... R3 (left child - right sibling representation) is:\n");
        print_R3(T2(R2, size), size);

        return (0);
}
