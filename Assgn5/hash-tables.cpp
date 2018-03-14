#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

#include "Profiler_linux.h"
Profiler profiler("HashTables");

#define C1 10
#define C2 5
#define HASH_SIZE 10007
#define EMPTY 0

int   *hash_table;
int   *random_array;
float *fill_factor;

void init_filling_factor(float *fill_factor){
        fill_factor[0] = 0.8;
        fill_factor[1] = 0.85;
        fill_factor[2] = 0.9;
        fill_factor[3] = 0.95;
        fill_factor[4] = 0.99;
}

int compute_key(int element, int try_number){
        return (element % HASH_SIZE + try_number * C1 + try_number * try_number * C2) % HASH_SIZE;
}

int insert_element(int element){
        int i = 0;
        do{
                int j = compute_key(element, i);
                if (hash_table[j] == EMPTY){
                        hash_table[j] = element;
                        return j;
                }
                else{
                        i++;
                }
        }while(i != HASH_SIZE);
        printf("could not insert element %d in hash\n", element);
        return -1;
}

int search_element(int element, int *op, int *op2){
        int i = 0;
        do {
                (*op)++;
                (*op2)++;
                int j = compute_key(element, i);
                if (hash_table[j] == element){
                        //(*op)++;
                        return j;
                }
                else if (hash_table[j] == EMPTY){
                        //(*op)++;
                        //printf("could not find element %d in hash\n", element);
                        return -1;
                }
                else{
                        i++;
                }
        } while(i != HASH_SIZE);
        //printf("could not find element %d in hash\n", element);
        return -1;
}

void fill_hash(float ff){
        random_array   = (int *)malloc(sizeof(int) * 2 * ff * HASH_SIZE);
        FillRandomArray(random_array, 2 * ff * HASH_SIZE, 1, 100000, 1, 0);

        for(int i = 0; i < ff * HASH_SIZE; i++){
                insert_element(random_array[i]);
        }
}

void search_existing(float ff, int *op0, int *max_per_elem){
        for (int i = 0; i < ff * HASH_SIZE; i += ff * HASH_SIZE / 1500){
                int per_elem = 0;
                search_element(random_array[i], op0, &per_elem);
                if (per_elem > (*max_per_elem)){
                        (*max_per_elem) = per_elem;
                }
                //(*op)++;
        }
}

void search_not_existing(float ff, int *op1, int *max_per_elem){
        for (int i = ff * HASH_SIZE; i < 2 * ff * HASH_SIZE; i += ff * HASH_SIZE / 1500){
                int per_elem = 0;
                search_element(random_array[i], op1, &per_elem);
                if (per_elem > (*max_per_elem)){
                        (*max_per_elem) = per_elem;
                }
                //(*op)++;
        }
}

void test_hash(){
        int a = 0;
        printf("\ninserting elements: 1, 3, 5, 8, 13..\n\n");
        insert_element(1);
        insert_element(3);
        insert_element(5);
        insert_element(8);
        insert_element(13);

        printf("searching for elements: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15..\n\n");
        for (int i = 1; i < 16; i++){
                if (int j = search_element(i, &a, &a) != -1){
                        printf("found element %d on position %d;\n", i, j);
                }
                else{
                        printf("did not find element %d;\n", i);
                }
        }
        free(hash_table);
}

int main(int argc, char **argv){
        hash_table     = (int *)malloc(sizeof(int) * HASH_SIZE);
        fill_factor    = (float *)malloc(sizeof(float) * 5);
        memset(hash_table, EMPTY, sizeof(int) * HASH_SIZE);

        int   nb_repetitions  = 5;
        int   fill_factor_nb  = 5;

        init_filling_factor(fill_factor);

        if (argc > 1 && strcmp(argv[1], "test") == 0){
                test_hash();
                hash_table     = (int *)malloc(sizeof(int) * HASH_SIZE);
                memset(hash_table, EMPTY, sizeof(int) * HASH_SIZE);
        }

        int ops_for_found;
        int ops_for_not_found;
        int max_ops_for_found;
        int max_ops_for_not_found;
        int total_ops_for_found;
        int total_ops_for_not_found;

        printf("\nf.f.  |  avg.eff.(found) |  max.eff.(found) |  avg. eff.(!found) |  max. eff.(!found)\n");
        printf("______|__________________|__________________|____________________|___________________\n");

        for (int i = 0; i < fill_factor_nb; i++){

                total_ops_for_found = 0;
                total_ops_for_not_found = 0;
                max_ops_for_found = 0;
                max_ops_for_not_found = 0;

                for (int j = 0; j < nb_repetitions; j++){

                        ops_for_found = 0;
                        ops_for_not_found = 0;

                        fill_hash(fill_factor[i]);

                        search_existing(fill_factor[i], &ops_for_found, &max_ops_for_found);
                        //printf("ops = %d\n", ops_for_found);
                        total_ops_for_found += ops_for_found;

                        search_not_existing(fill_factor[i], &ops_for_not_found, &max_ops_for_not_found);
                        //printf("ops2 = %d\n", ops_for_not_found);
                        total_ops_for_not_found += ops_for_not_found;
                        memset(hash_table, EMPTY, sizeof(int) * HASH_SIZE);
                }
                printf("%.2f  |     ",      fill_factor[i]);
                printf("%.3f        |       ", total_ops_for_found / (fill_factor[i] * HASH_SIZE));

                if (max_ops_for_found > 100){
                        printf("%d        |       ", max_ops_for_found);
                }
                else{
                        printf("%d         |       ", max_ops_for_found);
                }

                if ((float)(total_ops_for_not_found) / (fill_factor[i] * HASH_SIZE) < 10){
                        printf("%.3f        |       ", total_ops_for_not_found / (fill_factor[i] * HASH_SIZE));
                }
                else{
                        printf("%.3f       |       ", total_ops_for_not_found / (fill_factor[i] * HASH_SIZE));
                }

                printf("%d\n",               max_ops_for_not_found);
                printf("______|__________________|__________________|____________________|___________________\n");
        }

        free(hash_table);
        free(fill_factor);
        free(random_array);
        return (0);
}
