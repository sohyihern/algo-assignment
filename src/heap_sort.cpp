// *********************************************************
// Program: heap_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************
// Task Distribution
// Member_1:
// Member_2:
// Member_3: Heap sort
// Member_4:
// *********************************************************

#include "utils.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Heapify down from index i in array of size n (max-heap)
// Compares by integer key field only
void heapify(vector<Record> &records, int n, int i) {
    int largest = i;       // Assume current node is largest
    int left    = 2 * i + 1;
    int right   = 2 * i + 2;

    // Check if left child is larger than current largest
    if (left < n && records[left].key > records[largest].key) {
        largest = left;
    }

    // Check if right child is larger than current largest
    if (right < n && records[right].key > records[largest].key) {
        largest = right;
    }

    // If largest is not the root, swap and continue heapifying down
    if (largest != i) {
        swap(records[i], records[largest]);
        heapify(records, n, largest);
    }
}

void heapSort(vector<Record> &records) {
    int n = records.size();

    // Phase 1: Build max-heap (bottom-up)
    // Start from last non-leaf node down to root
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(records, n, i);
    }

    // Phase 2: Extract max one by one
    // Swap root (max) with last element, reduce heap size, heapify root
    for (int i = n - 1; i > 0; i--) {
        swap(records[0], records[i]);
        heapify(records, i, 0);
    }
}

string getSizeFromFilename(const string &filename) {
    // Example: dataset_1000.csv -> 1000
    size_t start = filename.find("dataset_");
    size_t end   = filename.find(".csv");

    if (start == string::npos || end == string::npos) {
        return "n";
    }

    start += 8;
    return filename.substr(start, end - start);
}

int main() {
    string inputFilename;

    cout << "Enter dataset filename: ";
    cin >> inputFilename;

    vector<Record> records = read_dataset(inputFilename);

    if (records.empty()) {
        cout << "No records found." << endl;
        return 1;
    }

    auto start = high_resolution_clock::now();

    heapSort(records);

    auto end = high_resolution_clock::now();

    duration<double> elapsed = end - start;

    string sizeText = getSizeFromFilename(inputFilename);
    string outputFilename = "heap_sorted_dataset_" + sizeText + ".csv";

    write_dataset(outputFilename, records);

    cout << "Heap sort completed successfully." << endl;
    cout << "Input size: " << records.size() << endl;
    cout << "Running time: " << elapsed.count() << " seconds" << endl;
    cout << "Output file: " << outputFilename << endl;

    return 0;
}