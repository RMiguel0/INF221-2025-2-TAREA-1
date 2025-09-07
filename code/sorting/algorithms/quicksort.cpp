// Use el algoritmo de Quicksort con pivote random, y tiene los mismos cambios que merge
// con respecto a la funcion lambda y quickSortImpl
// Codigo rescatado de la pagina: https://www.geeksforgeeks.org/dsa/quicksort-using-random-pivoting
//
// C++ implementation QuickSort 
// using Lomuto's partition Scheme.

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>



void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int generateRandomPivot(int low, int high) {
    srand(time(NULL));
    return low + rand() % (high - low + 1);
}

void quickSortImpl(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = generateRandomPivot(low, high);
        int pivotValue = arr[pivotIndex];

        swap(&arr[pivotIndex], &arr[high]);

        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] < pivotValue) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }

        swap(&arr[i + 1], &arr[high]);

        quickSortImpl(arr, low, i);
        quickSortImpl(arr, i + 2, high);
    }
}

void quickSort(std::vector<int>& arr) {
    if (!arr.empty()) {
        quickSortImpl(arr, 0, arr.size() - 1);
    }
}
