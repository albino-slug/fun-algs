/**
 * @author Șinca Mădălina Ștefania
 * @group 30423
 *
 * Compare the following sorting methods: BubbleSort, InsertionSort and SelectionSort
 *
 */

#include "stdio.h"
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include "Profiler_linux.h"

using namespace std;

Profiler profiler("Sorting");

// bubble sort
void BubbleSorting(int arr[], int size, string caseScenario){
	// variable declarations
	int i; 										// iterator
	int comparisons = 0;			// comparisons counter
	int assignments = 0;			// assignments counter
	int sizeClone = size;			// copy of the size of the array
	bool hasSwapped = true;		// checks whether a swap has been performed

	// sorting algorithm implementation
	while (hasSwapped){
		hasSwapped = false;
		for (i = 0; i < sizeClone - 1; i++){
			comparisons++;
			if (arr[i] > arr[i + 1]){
				swap(arr[i], arr[i+1]);
				assignments += 3;   // 3 assignments have been performed in the swap() function
				hasSwapped = true;  // there has been a swap
			}
		}
		sizeClone--;						// diminish size because the largest element is now on last position
	}

	// profiler usage
	profiler.countOperation((caseScenario + "_comparisons").c_str(), size, comparisons);
	profiler.countOperation((caseScenario + "_assignments").c_str(), size, assignments);
	//profiler.countOperation((caseScenario + "_comparisons_&_assignments_").c_str();, size, assignments + comparisons);
	profiler.addSeries((caseScenario + "_comparisons_+_assignments").c_str(), (caseScenario + "_comparisons").c_str(), (caseScenario + "_assignments").c_str());
}

// insertion sort
void InsertionSorting(int arr[], int size, string caseScenario){
	// variable declarations
	int i; 										// iterator
	int k;										// auxiliary iterator
	int aux;									// auxiliary variable
	int comparisons = 0;			// comparisons counter
	int assignments = 0;			// assignments counter

	// sorting algorithm implementation
	for (i = 1; i < size; i++){
		aux = arr[i];						// store the value to-be-inserted in aux
		assignments++;
		k = i - 1;							// begin searching for a slot starting from the first index on the left of the current one
		while ((aux < arr[k]) && (k >= 0)){
			comparisons++;
			arr[k + 1] = arr[k];	// make space for insertion
			assignments++;
			k--;
		}
		if (k){
			comparisons++;
		}
		arr[k + 1] = aux;				// insert the desired value at correct index
		assignments++;
	}

	// profiler usage
	profiler.countOperation((caseScenario + "_comparisons").c_str(), size, comparisons);
	profiler.countOperation((caseScenario + "_assignments").c_str(), size, assignments);
	//profiler.countOperation((caseScenario + "_comparisons_&_assignments_").c_str();, size, assignments + comparisons);
	profiler.addSeries((caseScenario + "_comparisons_+_assignments").c_str(), (caseScenario + "_comparisons").c_str(), (caseScenario + "_assignments").c_str());
}

// selection sort
void SelectionSorting(int arr[], int size, string caseScenario){
	// variable declarations
	int i;										// iterator 1
	int j;										// iterator 2
	int iMin;									// index of minimum value in the unsorted array
	int comparisons = 0;			// comparisons counter
	int assignments = 0;			// assignments counter

	// sorting algorithm implementation
	for (i = 0; i < size - 1; i++){
		iMin = i;
		for (j = i + 1; j < size; j++){
			comparisons++;
			if (arr[j] < arr[iMin]){
				iMin = j;
			}
		}

		if (i != iMin){
			swap(arr[i], arr[iMin]);
			assignments += 3;
		}
	}

	// profiler usage
	profiler.countOperation((caseScenario + "_comparisons").c_str(), size, comparisons);
	profiler.countOperation((caseScenario + "_assignments").c_str(), size, assignments);
	//profiler.countOperation((caseScenario + "_comparisons_&_assignments_").c_str();, size, assignments + comparisons);
	profiler.addSeries((caseScenario + "_comparisons_+_assignments").c_str(), (caseScenario + "_comparisons").c_str(), (caseScenario + "_assignments").c_str());
}

// tests whether all algorithms work accordingly
void Test(char *AlgorithmName){
	int arr[100000];

	if (!strcmp("BubbleSorting", AlgorithmName)){					// tests Bubble Sort
		printf(" ...testing Bubble Sort... \n");
		FillRandomArray(arr, 100, 10, 50000, false, 0);
		BubbleSorting(arr, 100, "test");
		if (IsSorted(arr, 100)){
			printf("Bubble Sort works just fine!\n\n");
		}
		else{
			printf("Bubble Sort does not work!\n\n");
		}
	}

	else if (!strcmp("InsertionSorting", AlgorithmName)){ // tests Insertion Sort
		printf(" ...testing Insertion Sort... \n");
		FillRandomArray(arr, 100, 10, 50000, false, 0);
		InsertionSorting(arr, 100, "test");
		if (IsSorted(arr, 100)){
			printf("Insertion Sort works just fine!\n\n");
		}
		else{
			printf("Insertion Sort does not work!\n\n");
		}
	}

	else if (!strcmp("SelectionSorting", AlgorithmName)){ // tests Selection Sort
		printf(" ...testing Selection Sort... \n");
		FillRandomArray(arr, 100, 10, 50000, false, 0);
		InsertionSorting(arr, 100, "test");
		if (IsSorted(arr, 100)){
			printf("Selection Sort works just fine!\n\n");
		}
		else{
			printf("Selection Sort does not work!\n\n");
		}
	}

	else{
		printf("Not an algorithm name!\n\n");
	}
}

void copyArray(int arr1[], int arr2[]){
	int i;
	for (i = 0; i <= 100000; i++){
		arr2[i] = arr1[i];
	}
}

int main(){
	int arr[100000];
	int arrCopy[100000];
	int caseScenario;
	int nbOfElements;


	// testing the algorithms' correctness
	// should not test at the same time with the report generator since it interferes
	Test("BubbleSorting");
	Test("InsertionSorting");
	Test("SelectionSorting");

/*
// testing the algorithms' efficienciy
for (nbOfElements = 100; nbOfElements <= 10000; nbOfElements += 100){
	for (caseScenario = 1; caseScenario < 8; caseScenario++){
		switch (caseScenario){
			// BEST
			case 1: FillRandomArray(arr, nbOfElements, 10, 50000, true, 1);   // best case scenario
							BubbleSorting(arr, nbOfElements, "Best_Bubble");

							FillRandomArray(arr, nbOfElements, 10, 50000, true, 1);
							InsertionSorting(arr, nbOfElements, "Best_Insertion");

							FillRandomArray(arr, nbOfElements, 10, 50000, true, 1);
							SelectionSorting(arr, nbOfElements, "Best_Selection");
			break;
			// AVERAGE
			case 2: FillRandomArray(arrCopy, nbOfElements, 10, 50000, false, 0);   // average case scenario 1
							copyArray(arrCopy, arr);
							BubbleSorting(arr, nbOfElements, "Average_Bubble");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							InsertionSorting(arr, nbOfElements, "Average_Insertion");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							SelectionSorting(arr, nbOfElements, "Average_Selection");
			break;
			case 3: FillRandomArray(arrCopy, nbOfElements, 10, 50000, false, 0);   // average case scenario 2
							copyArray(arrCopy, arr);
							BubbleSorting(arr, nbOfElements, "Average_Bubble");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							InsertionSorting(arr, nbOfElements, "Average_Insertion");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							SelectionSorting(arr, nbOfElements, "Average_Selection");
			break;
			case 4: FillRandomArray(arrCopy, nbOfElements, 10, 50000, false, 0);   // average case scenario 3
							copyArray(arrCopy, arr);
							BubbleSorting(arr, nbOfElements, "Average_Bubble");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							InsertionSorting(arr, nbOfElements, "Average_Insertion");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							SelectionSorting(arr, nbOfElements, "Average_Selection");
			break;
			case 5: FillRandomArray(arrCopy, nbOfElements, 10, 50000, false, 0);   // average case scenario 4
							copyArray(arrCopy, arr);
							BubbleSorting(arr, nbOfElements, "Average_Bubble");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							InsertionSorting(arr, nbOfElements, "Average_Insertion");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							SelectionSorting(arr, nbOfElements, "Average_Selection");
			break;
			case 6: FillRandomArray(arrCopy, nbOfElements, 10, 50000, false, 0);   // average case scenario 5
							copyArray(arrCopy, arr);
							BubbleSorting(arr, nbOfElements, "Average_Bubble");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							InsertionSorting(arr, nbOfElements, "Average_Insertion");

							//FillRandomArray(arr, nbOfElements, 10, 50000, false, 0);
							copyArray(arrCopy, arr);
							SelectionSorting(arr, nbOfElements, "Average_Selection");
			// WORST
			case 7: FillRandomArray(arr, nbOfElements, 10, 50000, false, 2);   // worst case scenario
							BubbleSorting(arr, nbOfElements, "Worst_Bubble");

							FillRandomArray(arr, nbOfElements, 10, 50000, false, 2);
							InsertionSorting(arr, nbOfElements, "Worst_Insertion");

							FillRandomArray(arr, nbOfElements, 10, 50000, false, 2);
							SelectionSorting(arr, nbOfElements, "Worst_Selection");
			break;
		}
	}
}

// group the graphs by Case Scenario and by the size that is measured
// assignments
profiler.createGroup("1_Best_Case_Assignments", "Best_Bubble_assignments", "Best_Insertion_assignments", "Best_Selection_assignments");
profiler.createGroup("1_Best_Case_Assignments2", "Best_Bubble_assignments");
profiler.createGroup("2_Average_Case_Assignments", "Average_Bubble_assignments", "Average_Insertion_assignments", "Average_Selection_assignments");
profiler.createGroup("3_Worst_Case_Assignments", "Worst_Bubble_assignments", "Worst_Insertion_assignments", "Worst_Selection_assignments");

// comparisons
profiler.createGroup("4_Best_Case_Comparisons", "Best_Bubble_comparisons", "Best_Insertion_comparisons", "Best_Selection_comparisons");
profiler.createGroup("4_Best_Case_Comparisons2", "Best_Bubble_comparisons");
profiler.createGroup("4_Best_Case_Comparisons3", "Best_Insertion_comparisons");
profiler.createGroup("5_Average_Case_Comparisons", "Average_Bubble_comparisons", "Average_Insertion_comparisons", "Average_Selection_comparisons");
profiler.createGroup("5_Average_Case_Comparisons2", "Average_Bubble_comparisons");
profiler.createGroup("6_Worst_Case_Comparisons", "Worst_Bubble_comparisons", "Worst_Insertion_comparisons", "Worst_Selection_comparisons");
profiler.createGroup("6_Worst_Case_Comparisons2", "Worst_Bubble_comparisons");

// assignments and comparisons
profiler.createGroup("7_Best_Case_Assignments_+_Comparisons", "Best_Bubble_comparisons_+_assignments", "Best_Insertion_comparisons_+_assignments", "Best_Selection_comparisons_+_assignments");
profiler.createGroup("7_Best_Case_Assignments2_+_Comparisons", "Best_Bubble_comparisons_+_assignments");
profiler.createGroup("8_Average_Case_Assignments_+_Comparisons", "Average_Bubble_comparisons_+_assignments", "Average_Insertion_comparisons_+_assignments", "Average_Selection_comparisons_+_assignments");
profiler.createGroup("8_Average_Case_Assignments_+_Comparisons2", "Average_Insertion_comparisons_+_assignments");
profiler.createGroup("9_Worst_Case_Assignments_+_Comparisons", "Worst_Bubble_comparisons_+_assignments", "Worst_Insertion_comparisons_+_assignments", "Worst_Selection_comparisons_+_assignments");

// generate the reports
profiler.showReport();*/
return 0;
}
