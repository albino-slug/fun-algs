#define MAX_SIZE 50000
#define RAND_MAX 50000

#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "Profiler_linux.h"
Profiler profiler("QuickSort");

//int qa = 0;
//int qc = 0;

// prints array, useful in debugging
void print(int *arr, int size){
	for (int i = 0; i < size; i++){
		printf("%d  ", arr[i]);
	}
	printf("\n");
}

// creates a copy of array arr1 in array arr2
void copyArray(int size, int *arr1, int *arr2){
	for (int i = 0; i < size; i++){
		arr2[i] = arr1[i];
        }
}

// HEAPSORT

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

void siftDown(int *arr, int start, int end, int *a, int *c){
	int aux;														// aux will always store the node which must be swapped with the root
	int child;
	int root = start;										// starting from the root / max element

	while(getLeftChild(root) <= end){		// while not at the end of heap
		child = getLeftChild(root);
		aux   = root;
		(*c)++;
		if (arr[aux] < arr[child]){				// checks left child
			aux = child;
		}																	// checks right child
		(*c)++;
		if (((child + 1) <= end) && (arr[aux] < arr[child + 1])){
			aux = child + 1;
		}
		if (aux == root){									// left and right children are smaller than root
			return;													// exit procedure
		}
		else{
			(*a) += 3;
			swap(arr[root], arr[aux]);			// put bigger child on first position
			root = aux;
		}
	}
}

// creates the heap structure inside the array using the bottom-up approach
void heapifyDown(int *arr, int size, int *a, int *c){
	int start = getParent(size - 1);

	while (start >= 0){
		siftDown(arr, start, size - 1, a, c);
		start--;
	}
}

// main algorithm
void heapSortDown(int *arr, int size, string caseScenario){
	int a = 0;													// number of assignments
	int c = 0;													// number of comparisons

	// sorting
	heapifyDown(arr, size, &a, &c);			// heap structure is created

	int end = size - 1;
	while (end > 0){
		a += 3;
		swap(arr[end], arr[0]);						// place the max element on the last index
		end--;														// decrease the size of unordered array / heap
		siftDown(arr, 0, end, &a, &c);		// recreate heap structure
	}
	// profiler usage
	//profiler.countOperation((caseScenario + "A").c_str(), size, a);
	//profiler.countOperation((caseScenario + "C").c_str(), size, c);
	profiler.countOperation((caseScenario + "AC").c_str(), size, a + c);
	//profiler.addSeries((caseScenario + "AC").c_str(), (caseScenario + "A").c_str(), (caseScenario + "C").c_str());
}

// QUICKSORT

int QuickPartition(int *arr, int p, int r, int *a, int *c){
        int x;
        int i;
        int j;

        x = arr[r];
	(*a)++;
        i = p - 1;

        for (j = p; j < r; j++){
                if (arr[j] <= x){
			(*c)++;
                        i++;
                        swap(arr[i], arr[j]);
			(*a) += 3;
                }
        }
        swap(arr[i + 1], arr[r]);
	(*a) += 3;
        return (i + 1);
}

int RandomSelect(int *arr, int p, int r, int *a, int *c){
        int i;

        i = (int)(p + rand() % (r + 1 - p));
        swap(arr[r], arr[i]);
	(*a) += 3;
        return (QuickPartition(arr, p, r, a, c));
}

void QuickSort(int *arr, int p, int r, int *a, int *c){
        int q;
	//int a = *qa;
	//int c = *qc;

        if (p < r){
		q = RandomSelect(arr, p, r, a, c);
		//q = QuickPartition(arr, p, r, a, c);
                QuickSort(arr, p, q - 1, a, c);
                QuickSort(arr, q + 1, r, a, c);
        }

	//profiler.countOperation((caseScenario + "AC").c_str(), size, a + c);
}

void GenerateReport(){
	int arr[MAX_SIZE];
	int arrCopy[MAX_SIZE];

	for(int NbOfElements = 100; NbOfElements <= 10000; NbOfElements += 100){
		// average case
		for(int i = 0; i < 5; i++){
			int qa = 0;
			int qc = 0;
			FillRandomArray(arr, NbOfElements, 1, 100, false, 0);
			copyArray(NbOfElements, arr, arrCopy);
			QuickSort(arr, 0, NbOfElements - 1, &qa, &qc);
			//printf("%d - %d\n\n", qa, qc);
			// profiler usage
			profiler.countOperation("AverageQuickAC", NbOfElements, qa + qc);
			//profiler.countOperation("AverageQuickC", NbOfElements, qc);
			//profiler.addSeries("AverageQuickAC", "AverageQuickA", "AverageQuickC");
			heapSortDown(arrCopy, NbOfElements, "AverageHeap");
		}
	}

	//profiler.createGroup("4_AverageCaseAssignments", "AverageQuickA",  "AverageHeapA");
	//profiler.createGroup("5_AverageCaseComparisons", "AverageQuickC",  "AverageHeapC");
	profiler.createGroup("6_AverageCaseSum", "AverageQuickAC", "AverageHeapAC");
	profiler.showReport();
}

// tests the algorithm's correctnesss
void Test(char *AlgorithmName, int size){
	int arr[100000];
	int MaxVal = 100;
	int a = 0, b = 0;

	if (!strcmp("HeapSort", AlgorithmName)){					// tests Bubble Sort
		printf(" ...testing HeapSort... \n\n");
		FillRandomArray(arr, size, 0, MaxVal, false, 0);
		printf("random input:    ");
                print(arr, size);
		heapSortDown(arr, size, "test");
		printf("computed output: ");
                print(arr, size);
		if (IsSorted(arr, size)){
			printf("\nHeapSort works just fine!\n\n");
		}
		else{
			printf("\nHeapSort does not work!\n\n");
		}
		printf("____________________________________________________\n\n");
	}

        else if (!strcmp("QuickSort", AlgorithmName)){					// tests Bubble Sort
                printf(" ...testing QuickSort... \n\n");
                FillRandomArray(arr, size, 0, MaxVal, false, 0);
                printf("random input:    ");
                print(arr, size);
                QuickSort(arr, 0, size - 1, &a, &b);
                printf("computed output: ");
                print(arr, size);
                if (IsSorted(arr, size)){
                        printf("\nQuickSort works just fine!\n\n");
                }
                else{
                        printf("\nQuickSort does not work!\n\n");
                }
        }
	else{
		printf("Not an algorithm name!\n\n");
	}
}

int main(int argc, char **argv){
	char q[11] = "QuickSort\0";
	char h[11] = "HeapSort\0";
        if (argc == 3 && strcmp(argv[1], "test") == 0){
                Test(h, atoi(argv[2]));
                Test(q, atoi(argv[2]));
        }
        else if (argc == 2 && strcmp(argv[1], "generate") == 0){
		GenerateReport();
        }
        else{
		printf("Please provide a valid command and / or arguments! ^.^\n");
                return (0);
        }
}
