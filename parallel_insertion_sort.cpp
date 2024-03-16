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

bool founded = false;

int binarySearch(int *arr, int size, int target) {
    int left = 0;
    int right = size - 1;
    while (left <= right) {
        if (founded) {
            break;
        }
        
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            founded = true;
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
    
    // Try to read argument which required 2 argument
    if (argc < 2) {
        cout << "Please enter argument: <filenmae> <thread amount>" << endl;
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

    // Take thread amount from 2nd argument to calculate chunk
    int threadAmount = stoi(argv[2]);
    int chunkSize = arrSize / threadAmount;
    int remaining = arrSize % chunkSize;
    printf("split array size %d with compute %d thread in to chunk size = %d\n", arrSize, threadAmount, chunkSize);

    vector<thread> threads;
    // Insertion Sort
    for (int i = 0; i < threadAmount; ++i) {
        int* begin = arr + (i * chunkSize);
        int* end = begin + chunkSize;
        if (i == threadAmount - 1) {
            end += remaining;
        }
        // Create thread process with 'thread'
        threads.push_back(
            thread([begin, end, i, chunkSize, target]() {
                insertionSort(begin, end - begin);
                // Binary Search
                int foundedIndex = binarySearch(begin, end - begin, target);
                if (foundedIndex != -1) {
                    printf("Thread (%d) founded target %d at index: %d\n", i, target,  (i * chunkSize) + foundedIndex);
                }
            })
        );
    }

    for (auto &thread: threads) {
        thread.join();
    }
    
    if (!founded) {
        printf("Target %d not founded.\n", target);
    }
    
    // Stop timer calculate execution time
    auto ended_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(ended_time - started_time);
    cout << "Total Execution time = " << duration.count() << " (milliseconds)" << endl;
    return 0;
}