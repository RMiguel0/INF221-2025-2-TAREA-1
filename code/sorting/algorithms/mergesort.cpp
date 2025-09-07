// Codigo rescatado de la siguiente pagina: https://www.geeksforgeeks.org/cpp/cpp-program-for-merge-sort
// Las diferencias que hay son porque en el archivo que son llamadas se instancian con una funcion 
// lambda, por eso esta la funcion mergeSortImpl, pero es el mismo esquema de codigo.


#include <iostream>
#include <vector>

// Merges two subarrays of vec.
// First subarray is vec[left..mid]
// Second subarray is vec[mid+1..right]
void merge(std::vector<int>& vec, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary vectors
    std::vector<int> leftVec(n1), rightVec(n2);

    // Copy data to temporary vectors
    for (i = 0; i < n1; i++)
        leftVec[i] = vec[left + i];
    for (j = 0; j < n2; j++)
        rightVec[j] = vec[mid + 1 + j];

    // Merge the temporary vectors back into vec[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (leftVec[i] <= rightVec[j]) {
            vec[k] = leftVec[i];
            i++;
        } else {
            vec[k] = rightVec[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftVec[], if any
    while (i < n1) {
        vec[k] = leftVec[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightVec[], if any
    while (j < n2) {
        vec[k] = rightVec[j];
        j++;
        k++;
    }
}

// Helper function that does the recursive sorting
void mergeSortImpl(std::vector<int>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortImpl(vec, left, mid);
        mergeSortImpl(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}

// Wrapper function with the correct signature for the record function
void mergeSort(std::vector<int>& vec) {
    if (!vec.empty()) {
        mergeSortImpl(vec, 0, vec.size() - 1);
    }
}