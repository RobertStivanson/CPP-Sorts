#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

// Global Variables
// These are counters that are used when the sorting happens
int NUMBER_OF_COMPARISONS, NUMBER_OF_SWAPS;
const int MAX_UPPER_BOUND = 100, MAX_LOWER_BOUND = -100;

// This is a value that holds all sorts
// it is used to cycle through every sort
enum Sort {
	BOGO_SORT = 0,
	BUBBLE_SORT,
	SELECTION_SORT,
	SHELL_SORT,
	MERGE_SORT,
	QUICK_SORT,
	NONE
};

// Function prototypes
void BogoSort(int * array, const int & size);
void BubbleSort(int * array, const int & size);
void SelectionSort(int * array, const int & size);
void SelectionSortReverse(int * array, const int & size);
void ShellSort(int * array, const int & size);
void MergeSort(int * array, int * second, const int & left, const int & right);
void Merge(int * array, int * second, const int & left, const int & mid, const int & right);
void QuickSort(int * array, const int & low, const int & high);
int Partition(int * array, const int & low, const int & high);
void Swap(int * array, const int & first, const int & second);
string SortToString(Sort sort);
void ResetCounters();
void FillArray(int * array, const int & size,  const int & upperBound, const int & lowerBound);
int CalculateSpan(const int & upperBound, const int & lowerBound);
void PrintArray(int * array, const int & size);
void PrintCounters();
void TestSortedArray(int * array, const int & size, int * testArray);
void CopyArray(int * one, const int & size, int * two);

// main
// This is the programs main routine
int main() {
	int size;
	int * array = NULL, 
		* second = NULL, 
		* test = NULL;
	bool printAndTest;
		
	// Seed the random number generator
	srand(time(NULL));
	
	// While size is less than or equal to 125
	for (int size = 5; size <= 15625; size *= 5) {
		// Create the arrays
		array = new int[size];
		second = new int[size];
		test = new int[size];
		
		// For every type of sort
		for (int i = BOGO_SORT; i < NONE; i++) {
			printAndTest = true;
			
			cout << "// ********" << SortToString(Sort(i)) << "******** //" << endl;
			
			// Fill the array with random values
			FillArray(array, size, MAX_UPPER_BOUND, MAX_LOWER_BOUND);
			
			// Reverse sort the list		
			SelectionSortReverse(array, size);
			
			// Reset the sort counters
			ResetCounters();
			
			// Copy the unsorted array into a second array for the builtin sort
			CopyArray(array, size, test);
			
			// Perform the sort
			switch (Sort(i)) {
				case BOGO_SORT:
					if (size > 5) {
						cout << "Bogo sort skipped due to the time requirement of the algorithm." << endl;
						printAndTest = false;
					} else {
						BogoSort(array, size);
					}
					break;
				case BUBBLE_SORT:
					BubbleSort(array, size);
					break;
				case SELECTION_SORT:
					SelectionSort(array, size);
					break;
				case SHELL_SORT:
					ShellSort(array, size);
					break;
				case MERGE_SORT:
					MergeSort(array, second, 0, size - 1);
					break;
				case QUICK_SORT:
					QuickSort(array, 0, size - 1);
					break;
			}
			
			if (printAndTest) {
			// Print the array statistics
				PrintCounters();
				TestSortedArray(array, size, test);
			}
			
			cout << endl;
		}
		
		// free the dynamic memory
		delete [] array;
		delete [] second;
		delete [] test;
	}
	
	return 0;
}

// BogoSort
// params:
//	array: This is the array in which the elements are stored
//	size: This is the size of the array in which is being sorted
// Performs the Bogo sort algorithm on the array
void BogoSort(int * array, const int & size) {
	bool sorted = false;
	
	while (!sorted) {
		// For each item in the array randomly swap it with another
		for (int i = 0; i < size; ++i) {
			Swap(array, i, rand() % size);
		}
		
		// Assume the array is sorted
		sorted = true;
		
		// Test each item in the array to see if it is sorted
		for (int i = 0; i < (size - 1) && sorted; ++i) {
			++NUMBER_OF_COMPARISONS;
			if (array[i] > array[i + 1]) {
				sorted = false;
			}
		}
	}
}

// BubbleSort
// params:
//	array: This is the array in which the elements are stored
//	size: This is the size of the array in which is being sorted
// Performs the Bubble sort algorithm on the array
void BubbleSort(int * array, const int & size) {
	// For (size - 1) interations
	for (int i = 0; i < size - 1; ++i) {
		// For every element in the array
		for (int j = 0; j < size - 1; ++j) {
			// Compare the two values, if the first is bigger than the second
			if (array[j] > array[j + 1]) {
				// Swap the elements
				Swap(array, j, (j + 1));
			}
			
			// Increment the number of comparisons
			++NUMBER_OF_COMPARISONS;
		}
		
	}
	
	return;
}

// SelectionSort
// params:
//	array: This is the array in which the elements are stored
//	size: This is the size of the array in which is being sorted
// Performs the Selection sort algorithm on the array
void SelectionSort(int * array, const int & size) {
	int minIndex;
	
	// For every element in the array minus the last one
	for (int i = 0; i < size - 1; ++i) {
		// Set this to the starting minimum value
		minIndex = i;
		
		for (int j = (i + 1); j < size; ++j) {
			// If this value is less than the current minimum
			if (array[j] < array[minIndex]) {
				// Then this is the new minimum value
				minIndex = j;
			}
			
			// Increment the number of comparisons
			++NUMBER_OF_COMPARISONS;
		}
		
		// Swap the mininum value to the index i
		Swap(array, i, minIndex);
	}
	
	return;
}

// SelectionSortReverse
// params:
//	array: This is the array in which the elements are stored
//	size: This is the size of the array in which is being sorted
// Performs the Selection sort algorithm on the array into descending order
void SelectionSortReverse(int * array, const int & size) {
	int minIndex;
	
	// For every element in the array minus the last one
	for (int i = 0; i < size - 1; ++i) {
		// Set this to the starting minimum value
		minIndex = i;
		
		for (int j = (i + 1); j < size; ++j) {
			// If this value is less than the current minimum
			if (array[j] > array[minIndex]) {
				// Then this is the new minimum value
				minIndex = j;
			}
			
			// Increment the number of comparisons
			++NUMBER_OF_COMPARISONS;
		}
		
		// Swap the mininum value to the index i
		Swap(array, i, minIndex);
	}
	
	return;
}

// MergeSort
// params:
//	array: This is the array in which the elements are stored
//	second: This is the array in which the values should be merged into
//	left: This is the left starting index in the array
//	right: This is the right ending index in the array
// Performs the MergeSort algorithm on the array
void MergeSort(int * array, int * second, const int & left, const int & right) {
	// If the left index is less than the right index
	if (left < right) {
		// Calculate a new mid point
		int mid = (left + right) / 2;
		
		// Split this part of the list in half
		MergeSort(array, second, left, mid);
		MergeSort(array, second, mid + 1, right);
		
		// Merge these two halfs of the lists
		Merge(array, second, left, mid, right);
	}
	
	return;
}

// Merge
// params:
//	array: This is the array in which the elements are stored
//	second: This is the array in which the values should be merged into
//	left: This is the left starting index in the array
//	mid: This is the value of the ((left + right) / 2) + 1
//	right: This is the right ending index in the array
// Merges two sub arrays together in order
void Merge(int * array, int * second, const int & left, const int & mid, const int & right) {
	int i = left,
		j = mid + 1,
		k = 0; 
	
	// While we still have two halves to merge
	while(i <= mid && j <= right) {
		// If the element in the left half is less
		if(array[i] < array[j]) {
			// Add it to the final array next
			second[k++] = array[i++];
			
		// If the element in the right half is less
		} else {
			// Add it to the final array next
			second[k++] = array[j++];
		}
		
		// Increment the counters
		++NUMBER_OF_COMPARISONS;
	}
	
	// If there are remaining elements in the left half
	while(i <= mid) {
		// add them to the final array
		second[k++] = array[i++];
	}
	
	// If there are remaining elements in the right half
	while(j <= right) {
		// Add them to the final array
		second[k++] = array[j++];
	}
	
	// Copy the contents of the second array to the first
	for(i = right; i >= left; i--) {
		array[i] = second[--k];
	}
	
	return;
}

// ShellSort
// params:
//	array: This is the array in which the elements are stored
//	size: This is the size of the array in which is being sorted
// Performs the Shellsort algorithm on the array
void ShellSort(int * array, const int & size) {
	int tmp;
	
	// Until the gap is less than 1
	for (int gap = size / 2; gap > 0; gap /= 2) {
		// For every gap
		for (int i = gap; i < size; ++i) {
			// For every element in this gap
			for (int j = i - gap; j >= 0; j -= gap) {
				// If the first value is bigger than the second
				if (array[j] > array[j + gap]) {
					// Swap the values in the array
					Swap(array, j, j + 1);
				}
				
				// Increment the number of comparisons
				++NUMBER_OF_COMPARISONS;
			}
		}
	}
	
	return;
}

// Quicksort
// params:
//	array: This is the array in which the elements are stored
//	size: This is the size of the array in which is being sorted
// Performs the Quicksort algorithm on the array
void QuickSort(int * array, const int & low, const int & high) {
	// If the gap is greater than one
	if (low < high) {
		// Create a partition point
		int partition = Partition(array, low, high);
		
		// Sort the sub arrays
		QuickSort(array, low, (partition - 1));
		QuickSort(array, (partition + 1), high);
	}
	
	return;
}

// Partition
// params:
//	array: This is the array in which the elements are stored
//	low: This is the lower bound index in the array to partition
//	high: This is the higher bound index in the array to partition
// Takes the array and returns a partition point used in the Quick sort algorithm
int Partition(int * array, const int & low, const int & high) {
	int pivot = array[high];
	int i = low;
	
	// For every element in the sub array
	for (int j = low; j < high; ++j) {
		// If this element is less than our pivot
		if (array[j] <= pivot) {
			// Swap the elements
			Swap(array, i, j);
			++i;
		}
		
		// Increment the number of comparisons
		++NUMBER_OF_COMPARISONS;
	}
	
	// Swap the lowest elements and the last element
	Swap(array, i, high);
	
	return i;
}

// Swap
// params:
//	array: This is the array in which the elements are stored
//	first: This is the index of the first element we are swapping
//	second: This is the index of the second element we are swapping
// Swaps two elements in an array
void Swap(int * array, const int & first, const int & second) {
	// Swap the elements
	int tmp = array[first];
	array[first] = array[second];
	array[second] = tmp;
	
	// Increment the swap counter
	++NUMBER_OF_SWAPS;
	
	return;
}

// ResetCounters
// Resets all global sort counters to zero
void ResetCounters() {
	NUMBER_OF_COMPARISONS = 0;
	NUMBER_OF_SWAPS = 0;
	
	return;
}

// SortToString
// params:
//	sort: This is the enum Sort value we wish to convert
// Converts a sort enum value to a string
string SortToString(Sort sort) {
	string str = "";
	
	switch (sort) {
		case BOGO_SORT:
			str = "Bogo Sort";
			break;
		case BUBBLE_SORT:
			str = "Bubble Sort";
			break;
		case SELECTION_SORT:
			str = "Selection Sort";
			break;
		case SHELL_SORT:
			str = "Shell Sort";
			break;
		case MERGE_SORT:
			str = "Merge Sort";
			break;
		case QUICK_SORT:
			str = "Quick Sort";
			break;
	}
	
	return str;
}

// FillArray
// params:
//	size: this is the size of the array that we would like to create
//	upperBound: this is the max value that we would like to possibly be in the array
//	lowerBound: This is the min value that we would like to possibly be in the array
// This function will create a return an array of the given size filled with random values between our bounds
void FillArray(int * array, const int & size, const int & upperBound, const int & lowerBound) {
	int span = CalculateSpan(upperBound, lowerBound);
  
	// If the size is invalid
	if (size <= 0) {
		cout << "The size is invalid" << endl;
		return;
	}
  
	// If the upperBound is bigger than the lowerBound
	// If the 
	if (span > 0) {
		// Display the parameters in which the array will be filled
		cout << "Filling " << size << " random values between ";
		cout << lowerBound << " and " << upperBound << "... ";
		
		// For every element that we wish to be in the array
		for (int i = 0; i < size; ++i) {
			// Calculate a random value in our span then shift it by the lowerBound
			array[i] = rand() % span + lowerBound;
		}
		
		// We are done with the filling
		cout << "Done!" << endl;
		
	// The span is invalid
	} else {
		cout << " -- Unable to fill the array. Invalid span: " << span << endl; 
	}
	
	return;
}

// CalculateSpan
// params:
//	upperBound: this is the max value that we would like to possibly be in the array
//	lowerBound: This is the min value that we would like to possibly be in the array
// This function will calculate the difference between the upper
// and lower bounds then shift it up to an origin of zero
int CalculateSpan(const int & upperBound, const int & lowerBound) {
	int span = 0;
	
	// If the upperBound is bigger than the lowerBound
	if (upperBound > lowerBound) {
		// If the upperBound is greater than or equal to zero
		if (upperBound >= 0) {
			// If the lowerBound is less than zero
			if (lowerBound < 0) {
				// Our span is the upperBound shifted up to include the negative numbers plus zero
				span = upperBound + (-1 * lowerBound) + 1;
				
			// If the lowerBound is greater than or equal to zero
			} else {
				// Our span is just the difference between the upperBound and the lowerBound
				span = upperBound - lowerBound;
			}
			
		// If the upperBound is less than zero
		} else {
			// The span is the upperBound shifted up by the lowerBound
			span = upperBound - lowerBound;
		}
		
	// If ther upperBound and lowerBounds have no middle
	} else {
		cout << " -- Cannot calculate a value above " << lowerBound << " and below " << upperBound << "!" << endl;
	}
	
	return span;
}

// PrintaArray
// params:
//	array: This is the array whose contents should be printed
//	size: This is the size of the aove array
// This functions print the contents of an array
void PrintArray(int * array, const int & size) {
	// For every element in the array
	for (int i = 0; i < size; i++) {
		// Print this element
		cout << array[i] << " ";
	}
	
	// Print a new line
	cout << endl;
	
	return;
}

// PrintCounters
// Prints the current values of the array sorting counters
void PrintCounters() {
	cout << "Performance results:" << endl;
	cout << "Comparisons: " << NUMBER_OF_COMPARISONS << endl;
	cout << "Swaps:       " << NUMBER_OF_SWAPS << endl;
	
	return;
}

// TestSortedArray
// params:
//	array: This is the array in which we will test if its correctly sorted
//	size: This is the size of both passed arrays
//	testArray: This is the array that will be sent through the c++ sort
// Uses the built in sort to sort and test if the array is correctly sorted
void TestSortedArray(int * array, const int & size, int * testArray) {
	bool correct = true;
	
	// Sort the test array using the built in standard sort
	sort(testArray, testArray + size);
	
	// For every element in the arrays
	for (int i = 0; i < size; i++) {
		// If the values are not the same
		if (array[i] != testArray[i]) {
			// The sort is not correct
			correct = false;
		}
	}
	
	// If the sort is not correct
	if (!correct) {
		cout << "The array was NOT sorted properly!" << endl;
		
	// If the sort is correct
	} else {
		cout << "The array was sorted properly!" << endl;
	}
	
	return;
}

// CopyArray
// params:
//	one: This is the array whose values will be copied from
//	size: This is the size of the array whose values we are copying from
//	two: This is the array in which we are copying values into
// Copys the contents of the first array into the second
void CopyArray(int * one, const int & size, int * two) {
	// For every element in the array
	for (int i = 0; i < size; ++i) {
		// Copy the values over
		two[i] = one[i];
	}
	
	return;
}
