#include <iostream>
#include <vector>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;

void printArray(int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void insertionSort(int *arr, int size) {
    for (int i = 1; i < size; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int binarySearch(int *arr, int size, int target) {
    int left = 0;
    int right = size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid; // Found the target
        } else if (arr[mid] < target) {
            left = mid + 1; // Search half Right
        } else {
            right = mid - 1; // Search half Left
        }
    }
    return -1;
}


int main(int argc, char **argv) {
    // Start Timer
    auto started_time = chrono::high_resolution_clock::now();
    
    // Try to read argument which required 1 argument
    if (argc < 1) {
        cout << "Please enter argument: <filenmae>" << endl;
        return -1;
    }
    
    // Take filename from 1st argument to read file
    string filename = argv[1];
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file " << filename << endl;
        return -1;
    }
    cout << "Reading file " << filename << "..." << endl;;
    
    // Reading target from file
    int target;
    inputFile >> target;

    // Reading array size from file
    int arrSize;
    inputFile >> arrSize;

    // Create array from size
    int arr[arrSize];

    // Reading array data from file
    int temp, count = 0;
    while (inputFile >> temp) { 
        arr[count] = temp;
        ++count;
    }
    inputFile.close(); 

    // Insertion Sort
    insertionSort(arr, arrSize);
    // Binary Search
    int foundedIndex = binarySearch(arr, arrSize, target);
    if (foundedIndex != -1)
        printf("Founded founded target %d at index: %d\n", target,  foundedIndex);
    else 
        printf("Target %d not founded.\n", target);

    // Stop timer calculate execution time
    auto ended_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(ended_time - started_time);
    cout << "Total Execution time = " << duration.count() << " (milliseconds)" << endl;
    return 0;
}