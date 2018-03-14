/*
        author: Mădălina Șinca,

*/

#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

#include "Profiler_linux.h"
Profiler profiler("OSTree");

#define RED       0
#define BLACK     1
#define TEST_SIZE 11
#define INDENT_SIZE 1000000

typedef struct n{
        int       data;
        bool      color; // UNUSED FIELD
        int       sub_tree_size;
        struct n *left;
        struct n *right;
} node;

// UNUSED, FOR NOW
typedef struct b_tree{
        node x;
        int  size;
} tree;

// RETURNS A NEW NODE WITH THE GIVEN FIELDS
node *new_node(int data, int color, int sub_tree_size, node *left, node *right){
        node *n = (node *)malloc(sizeof(node));
        if (n){
                n->data = data;
                n->color = color; // UNUSED FIELD
                n->sub_tree_size = sub_tree_size;
                n->left = left;
                n->right = right;
        }
        return (n);
}

// RETURNS A NODE WHICH IS THE ROOT OF THE NEWLY BUILT TREE
node *build_tree(int *arr, int start, int end, int *operations){
        if (start > end){
                return NULL;
        }

        // INITIALIZE CURRENT ELEMENT
        int mid = (start + end) / 2;
        node *x = new_node(arr[mid], 0, end - start + 1, NULL, NULL);
        (*operations)++;

        // BUILD RECURSIVELY
        x->left  = build_tree(arr, start, mid - 1, operations);
        (*operations)++;
        x->right = build_tree(arr, mid + 1, end, operations);
        (*operations)++;

        return (x);
}

// RETURNS THE SELECTED ELEMENT FROM THE TREE
node *OS_select(node *x, int to_be_selected, int *operations){
        int size_of_root = 1;
        (*operations) += 4;
        (*operations)++;
        // CHECK WHETHER TREE IS EMPTY
        if (x == NULL){
                return (NULL);
        }
        (*operations)++;
        // GET SIZE OF LEFT SUBTREE
        if (x->left != NULL){
                (*operations)++;
                size_of_root = x->left->sub_tree_size + 1;
        }
        (*operations)++;
        // THIS IS THE NODE, RETURN IT
        if (size_of_root == to_be_selected){
                return (x);
        }
        // IF SMALLER, GO LEFT
        else if (to_be_selected < size_of_root){
                (*operations)++;
                return OS_select(x->left, to_be_selected, operations);
        }
        // IF BIGGER, GO RIGHT
        // (THE ELEMENT SHOULD BE K-TH SMALLEST IN THE RIGHT BRANCH, WHEN K = SELECTION INDEX - SUBTREE SIZE OF ROOT)
        else{
                return OS_select(x->right, to_be_selected - size_of_root, operations);
        }
}

// RETURNS THE MINIMUM ELEMENT ON A CERTAIN BRANCH
node *branch_minimum(node *x, int *operations){
        node *current = x;

        // IF NO LEFT SUBTREE => THIS IS THE MINIMUM NODE
        (*operations)++;
        while (current->left != NULL){
                current = current->left;
                (*operations) += 2;
        }
        // RETURN THE MINIUM
        return (current);
}

// RETURNS THE ROOT OF THE TREE POST-DELETION
node *OS_delete(node *x, node *to_be_deleted, int *operations){
        (*operations)++;

        // ALREADY NO NODE IN TREE
        if (x == NULL){
                return (x);
        }

        // GOING LEFT
        if (to_be_deleted->data < x->data){
                x->left = OS_delete(x->left, to_be_deleted, operations);
                (*operations) += 2;
        }
        // GOING RIGHT
        else if (to_be_deleted->data > x->data){
                x->right = OS_delete(x->right, to_be_deleted, operations);
                (*operations) += 2;
        }
        // FOUND THE NODE TO DELETE
        else {
                node *successor = new_node(0, 0, 0, NULL, NULL);

                // HANDLING A NODE WHICH ONLY HAS ONE CHILD OR NO CHILD AT ALL

                // THE NODE HAS ONLY 1 RIGHT CHILD
                if (x->left == NULL){
                        successor = x->right;
                        (*operations) += 2;
                        free(x);
                        return (successor);
                }

                // THE NODE HAS ONLY 1 LEFT CHILD
                else if (x->right == NULL){
                        successor = x->left;
                        (*operations) += 2;
                        free(x);
                        return (successor);
                }

                // HANDLING NODE WITH 2 CHILDREN
                successor = branch_minimum(x->right, operations);
                x->data = successor->data;
                x->right = OS_delete(x->right, successor, operations);
                (*operations) += 2;
        }

        // RETURN THE MODIFIED TREE
        x->sub_tree_size--;
        (*operations)++;
        return (x);
}

// ATTEMPTS TO PRINT THE TREE IN A PRETTY FASHION
void pretty_print(node *n, char *indent, bool is_tail, bool is_x, bool is_null){

        // IF ROOT, INDENT ONLY
        if (is_x){
                printf("    ");
        }

        // ELSE, ADD THE BRANCHES
        if (!is_x){
                printf("%s", indent);
                if (is_tail){
                        printf(" └── ");
                }
                else{
                        printf(" ├── ");
                }

                // IF KNOWN TO BE NULL, PRINT o
                if(is_null){
                        printf(" o \n");
                        return;
                }
        }

        // PRINT THE NODE DATA
        printf("%d, %d\n", n->data, n->sub_tree_size);

        if (!is_x){
                if (is_tail){
                        strcat(indent, "      ");
                }
                else{
                        strcat(indent, " |    ");
                }
        }

        // MAKE O COPY OF THE INDENT - I KNOW THIS IS BAD CODING STYLE AND I AM ASHAMED
        char indent2[INDENT_SIZE];
        strcpy(indent2, indent);

        // RECURSIVE CALLS ON THE RIGHT-HAND SIDE
        if (n->right != NULL){
                pretty_print(n->right, indent, false, false, false);
        }
        else {
                pretty_print(n->right,  indent, true, false, true);
        }

        // RECURSIVE CALLS ON THE LEFT-HAND SIDE
        if (n->left != NULL){
                pretty_print(n->left,  indent2, true, false, false);
        }
        else {
                pretty_print(n->left,  indent2, true, false, true);
        }
}

// TESTS BUILDING, SELECTION AND DELETION FEATURES
void test_tree_features(int size, int sel1, int sel2, int sel3){
        node *to_delete = new_node(0, 0, 0, NULL, NULL);
        bool is_null = false;
        bool is_tail = false;
        char indent[70] = "     ";
        char indent2[70] = "     ";
        char indent3[70] = "     ";
        char indent4[70] = "     ";
        int  blind_ops = 0;

        if (size < 1){
                return;
        }

        // CONSTRUCT ARRAY
        int *arr = (int *)malloc(sizeof(int) * size);
        FillRandomArray(arr, size, 1, size, 1, 1);

        // A NEW TREE IS BUILT
        node *x = build_tree(arr, 0, size - 1, &blind_ops);

        // SETTING SOME PARAMETERS-TO-BE, MIGHT BE USELESS
        if (x == NULL){
                is_null = true;
        }
        if (x->sub_tree_size == 1){
                is_tail = true;
        }
        // PRINT THE TREE
        printf("... constructing tree ...\nThe newly generated tree is:\n\n");
        pretty_print(x, indent, is_tail, true, is_null);

        // SELECTION / DELETION DEMO
        node *sel_1, *sel_2, *sel_3;
        if (sel1 > 0  && sel1 <= size){
                printf("\n... selecting element %d ...\n", sel1);
                sel_1 = OS_select(x, sel1, &blind_ops);

                if (sel_1 == NULL){
                        printf("Element not found;\n");
                }
                else {
                        printf("The k-th smallest element is [%d, %d], when k = %d;\n", sel_1->data, sel_1->sub_tree_size, sel1);
                        to_delete = sel_1;
                        printf("... deleting element %d ...\n the tree after deletion is:\n\n", to_delete->data);
                        OS_delete(x, to_delete, &blind_ops);
                        pretty_print(x, indent2, false, true, false);
                }
        }
        else if(sel1 > size) {
                printf("\nSorry, the element %d that you wanted to select does not exist;\n", sel1);
        }

        if (sel2 > 0  && sel2 <= size){
                printf("\n... selecting element %d ...\n", sel2);
                sel_2 = OS_select(x, sel2, &blind_ops);

                if (sel_2 == NULL){
                        printf("Element not found;\n");
                }
                else {
                        printf("The k-th smallest element is [%d, %d], when k = %d;\n", sel_2->data, sel_2->sub_tree_size, sel2);
                        to_delete = sel_2;
                        printf("... deleting element %d ...\n the tree after deletion is:\n\n", to_delete->data);
                        OS_delete(x, to_delete, &blind_ops);
                        pretty_print(x, indent3, false, true, false);
                }
        }
        else if(sel2 > size) {
                printf("\nSorry, the elements %d, &d, &d that you wanted to select does not exist;\n", sel2);
        }

        if (sel3 > 0  && sel3 <= size){
                printf("\n... selecting element %d ...\n", sel3);
                sel_3 = OS_select(x, sel3, &blind_ops);

                if (sel_3 == NULL){
                        printf("Element not found;\n");
                }
                else {
                        printf("The k-th smallest element is [%d, %d], when k = %d;\n", sel_3->data, sel_3->sub_tree_size, sel3);
                        to_delete = sel_3;
                        printf("... deleting element %d ...\nthe tree after deletion is:\n\n", to_delete->data);
                        OS_delete(x, to_delete, &blind_ops);
                        pretty_print(x, indent4, false, true, false);
                }
        }
        else if(sel1 > size) {
                printf("\nSorry, the elements %d, &d, &d that you wanted to select does not exist;\n", sel3);
        }

        // EVERYONE IS FREE
        if (sel_1 != x && sel_2 != x && sel_3 != x){
                free(x);
        }
        free(sel_1);
        free(sel_2);
        free(sel_3);
        free(arr);
}

// RETURNS THE NEXT RANDOM ELEMENT FROM THE TREE
int next_random_index(int size){
        return(rand() % size + 1);
}

int main(int argc, char **argv){
        // TESTING OF THE PROCEUDRES
        if (argc > 1 && strcmp(argv[1], "test") == 0){
                int size = 0;
                if (argc > 2){
                        size = atoi(argv[2]);
                }
                else{
                        printf("Please specify the size of the tree;\n");
                }

                int *to_select = (int *)malloc(sizeof(int) * 4);
                memset(to_select, 0, 4);
                if (argc >= 4){
                        for (int i = 0; i < argc - 3; i++){
                                to_select[i] = atoi(argv[i + 3]);
                        }
                }
                test_tree_features(size, to_select[0], to_select[1], to_select[2]);
                free(to_select);
        }

        // SIMULATION / COMPUTING EFFORT
        else if (argc > 1 && strcmp(argv[1], "generate") == 0){
                int ops_build;
                int ops_select, ops_delete;
                srand(time(NULL));

                // SIZE OF TREE GOES FROM 100 TO 10.000
                for (int tree_size = 10; tree_size <= 10000; tree_size += 10){
                        ops_build  = 0;
                        ops_select = 0;
                        ops_delete = 0;

                        // FOR EVERY SIZE, 5 REPETITIONS ARE PERFORMED
                        for (int repetitions = 0; repetitions < 5; repetitions++){

                                //CREATING AN ARRAY FROM WHICH THE TREE IS BUILT
                                int *arr = (int *)malloc(sizeof(int) * (tree_size + 5));
                                FillRandomArray(arr, tree_size, 1, tree_size, 1, 1);
                                // X IS THE ROOT OF THE NEWLY BUILT TREE
                                node *x = build_tree(arr, 0, tree_size - 1, &ops_build);
                                free(arr);

                                int count_used = 0;
                                // DELETING ALL NODES ONE BY ONE, IN A RANDOM ORDER
                                for (int step = 1; step <= tree_size; step++){

                                        // TRY TO SELECT A NEW RANDOM NODE
                                        int next = next_random_index(tree_size - count_used);
                                        node *selected = OS_select(x, next, &ops_select);

                                        // IF A NODE HAS BEEN FOUND, DELETE IT
                                        if (selected != NULL /*&& selected->data != 0*/){ // <-- extra condition for handling the double free error
                                                count_used++;
                                                OS_delete(x, selected, &ops_delete);
                                        }
                                        // else if (selected->data == 0){
                                        //         printf("Avoid deleting a ZERO\n");
                                        // }
                                        else {
                                                printf("Could not find element at index %d\n", next);
                                        }
                                }
                        }
                        profiler.countOperation("BuildTree", tree_size, ops_build);
                        profiler.countOperation("OSselect",  tree_size, ops_select);
                        profiler.countOperation("OSdelete",  tree_size, ops_delete);
                }
                profiler.createGroup("ComputationalEffort", "BuildTree", "OSselect", "OSdelete");
                profiler.showReport();
        }
        // PRINT HELP
        else{
                // LEARN HOW TO USE YOUR OWN PROG.
                printf("\nPlease specify whether you want to TEST or RUN this program;\n");
        }

        return (0);
}
