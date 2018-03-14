#define RAND_MAX 50000

#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

#include "Profiler_linux.h"
Profiler profiler("ListMerger");

typedef struct node{
  int data;
  struct node *next;
} node;

typedef struct{
  int length;
  node *head;
  node *tail;
} list;

typedef struct{
        int number;
        int list;
} heap_node;

void print_array(int *arr, int size){
	for (int i = 0; i < size; i++){
		printf("%d  ", arr[i]);
	}
        printf("\nlength = %d\n", size);
}

void print_heap(heap_node *arr, int size){
	for (int i = 0; i < size; i++){
		printf("%d (L%d), ", arr[i].number, arr[i].list);
	}
	printf("\n");
}

//frees all the elements of a list
void   free_list(list *list_ptr){
        node *n;
        while (list_ptr->head != NULL){
                n = list_ptr->head;
                list_ptr->head = list_ptr->head->next;
                free(n);
        }
        list_ptr->tail = NULL;
        list_ptr->length = 0;

        free(list_ptr);
}

void free_all(list **LL, int size){
        for (int i = 0; i < size; i++){
                free_list(LL[i]);
        }

        free(LL);
}

// creates a new empty list
list *new_list(){
  list *list_ptr = (list*)malloc(sizeof(list));
  if (list_ptr){
    list_ptr->length = 0;
    list_ptr->head = list_ptr->tail = NULL;
  }
  return list_ptr;
}

// creates a new empty list array
list **new_list_array(int size){
  list **list_array_ptr = (list**)malloc(sizeof(list*) * size);
  return list_array_ptr;
}

//checks if the list is empty
int   is_list_empty(list *list_ptr){
  return ((list_ptr->head == NULL) && (list_ptr->tail == NULL));
}

//creates a new node with a given data
node *new_node(int data){
  node *n = (node*)malloc(sizeof(node));
  if (n){
    n->data = data;
    n->next = NULL;
  }
  return (n);
}

//inserts a given node in the rear of a given list
int   insert_at_end(list *list_ptr, node *n){
  if (list_ptr){
    if (is_list_empty(list_ptr)){
      list_ptr->head = list_ptr->tail = n;
    }
    else{
         list_ptr->tail->next = n;
         list_ptr->tail = n;
    }
    list_ptr->length++;
    return (1);
  }
  return (0);
}

//deletes the node in the front of a given list. returs the deleted node
node  *delete_first(list *list_ptr){
  node *n;
  if (list_ptr->head != NULL){
    n = list_ptr->head;
    list_ptr->head = list_ptr->head->next;
    list_ptr->length--;
    if (list_ptr->head == NULL){
      list_ptr->tail = NULL;
    }
    return (n);
  }
  return (NULL);
}

//deletes the node in the rear of a given list. returs the deleted node
node   *delete_last(list *list_ptr){
  node *iterator, *aux;

  aux = NULL;
  iterator  = list_ptr->head;
  if (iterator != NULL){
    while (iterator != list_ptr->tail){
      aux = iterator;
      iterator = iterator->next;
    }
    if (iterator == list_ptr->head){
      list_ptr->head = list_ptr->tail = NULL;
    }
    else{
      aux->next = NULL;
      list_ptr->tail = aux;
    }
  }
  return (iterator);
}

//prints a given list in a given file
void  print_list(list *list_ptr, int list_number){
  node *iterator;
  node *aux;

  printf("list %d: ", list_number);
  iterator  = list_ptr->head;
  aux = list_ptr->head;
  if (iterator == NULL) {
    printf("this list is empty :(\n");
    return;
  }
  while (iterator != list_ptr->tail){
    printf("%d ",iterator->data);
    iterator = iterator->next;
    aux = iterator;
  }
  printf("%d\n",aux->data);
  printf("length = %d\n\n", list_ptr->length);
}

int random_offset(int n){
        int o;
        int p = rand();

        if (n % 10 == 0 && n > 10){
                o = n / 3;
        }
        if (n % 10 != n){
                o = n % 10 + 1;
        }
        else{
                o = n / 2 - 1;
        }

        //printf("rand = %d\n", -o + p % (4*o - o + 1));
        return (p % (o + 1));
}

list *generate_individual_list(int size){
        int *arr = (int *)malloc(size * sizeof(int));
        list *L  = new_list();

        FillRandomArray(arr, size, 1, 50000, 1, 1);

        for (int i = 0; i < size; i++){
                insert_at_end(L, new_node(arr[i]));
        }
        free(arr);
        return(L);
}

list **generate_multiple_lists(int how_many, int total_size){
        list **LL = new_list_array(how_many);
        int remaining_size = total_size;
        srand(time(0));

        for(int i = 0; i < how_many - 1; i++){
                int current_size = 0;
                while(current_size == 0 || remaining_size - current_size == 0){
                        int offset = random_offset(total_size);
                        if (i % 2 == 0){
                                current_size = floor(total_size / how_many) + offset;
                        }
                        else{
                                current_size = floor(total_size / how_many) - offset;
                        }
                }
                //LL[i] = generate_individual_list(current_size);
                LL[i] = generate_individual_list(floor(total_size / how_many));
                //remaining_size -= current_size;
                remaining_size -= floor(total_size / how_many);
                //print_list(LL[i], i);
        }
        LL[how_many - 1] = generate_individual_list(remaining_size);
        //print_list(LL[how_many - 1], how_many - 1);
        return (LL);
}

// returns the parent of the node
int getParent(int node){
	return (floor((node - 1) / 2));
}

// returns the left child of node
int getLeftChild(int node){
	return (2 * node + 1);
}

// returns the right child of node
int getRightChild(int node){
	return (2 * node + 2);
}


void siftDown(heap_node *arr, int start, int end, int *a, int *c){
        int aux;
	int child;
	int root = start;				                                // starting from the root / min element

	while(getLeftChild(root) <= end){		                                // while not at the end of heap
		child = getLeftChild(root);
		aux   = root;
                (*c)++;
		if (arr[aux].number > arr[child].number){                               // checks left child
			aux = child;
		}
                (*c)++;
		if (((child + 1) <= end) && (arr[aux].number > arr[child + 1].number)){
			aux = child + 1;
		}
		if (aux == root){							// left and right children are bigger than root
			return;								// exit procedure
		}
		else{
			swap(arr[root], arr[aux]);
                        (*a) += 3;              			                // put smallest child on first position
			root = aux;
		}
	}
}

// creates the heap structure inside the array using the bottom-up approach
void heapify(heap_node *arr, int size, int *a, int *c){
	int start = getParent(size - 1);

	while (start >= 0){
		siftDown(arr, start, size - 1, a, c);
		start--;
	}
}

int merge_lists(int how_many, int total_size, string caseScenario){
        int k = how_many;
        int a = 0;
        int c = 0;
        list **LL = generate_multiple_lists(how_many, total_size);
        heap_node *heap = (heap_node *)malloc(how_many * sizeof(heap_node));
        int *final_array = (int *)malloc(total_size * sizeof(int));

        for (int i = 0; i < how_many; i++){
                heap_node aux;
                aux.number = delete_first(LL[i])->data;
                aux.list = i;
                heap[i] = aux;
                a++;
        }
        heapify(heap, how_many, &a, &c);
        //print_heap(heap, how_many);

        for(int i = 0; i < total_size; i++){
                final_array[i] = heap[0].number;
                a++;
                if (is_list_empty(LL[heap[0].list])){
                        swap(heap[0], heap[how_many - 1]);
                        a += 3;
                        how_many--;
                }
                else{
                        heap[0].number = delete_first(LL[heap[0].list])->data;
                        a++;
                }
                siftDown(heap, 0, how_many - 1, &a, &c);
        }
        //print_array(final_array, total_size);

        //profiler.countOperation(caseScenario.c_str(), total_size, a + c);
        free(final_array);
        free(heap);
        free_all(LL, k);

        // if (IsSorted(final_array, total_size) == true){
        //         printf("it's sorted alright!\n");
        // }
        // else{
        //         printf("not sorted :( try again..\n)");
        // }

        return (a + c);
}

void tester(int k, int n){
        list **LL = generate_multiple_lists(k, n);

        for(int i = 0; i < k; i++){
                print_list(LL[i], i);
        }

        free_all(LL, k);
}

int main(int argc, char **argv){
        if(strcmp(argv[1], "test") == 0){
                tester(atoi(argv[2]), atoi(argv[3]));
                return (0);
        }
        else if (strcmp(argv[1], "case1") == 0){
                int k1 = 5, k2 = 10, k3 = 100;
                for (int n = 100; n <= 10000; n += 100){
                        int r1 = merge_lists(k1, n, "k1");
                        int r2 = merge_lists(k2, n, "k2");
                        int r3 = merge_lists(k3, n, "k3");

                        profiler.countOperation("k1", n, r1);
                        profiler.countOperation("k2", n, r2);
                        profiler.countOperation("k3", n, r3);
                }
                profiler.createGroup("VariableTotalSize", "k1", "k2", "k3");
        }
        else if (strcmp(argv[1], "case2") == 0){
                int n = 10000;
                for (int k = 10; k <= 500; k +=10){
                        int r = merge_lists(k, n, "k");
                        profiler.countOperation("operations", k, r);
                }
                //profiler.createGroup("2.VariablePartialSize", "k");
        }
        //merge_lists(atoi(argv[1]), atoi(argv[2]));
        //for(int k)

        profiler.showReport();
        return(0);
}
