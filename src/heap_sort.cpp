// *********************************************************
// Program: heap_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
//Member_1: 243UC246W1 | KOH YOU XIANG | KOH.YOU.XIANG@student.mmu.edu.my | 019-6581165
//Member_2: 251UC250KN | PATRICK TOH TZY GUAN | PATRICK.TOH.TZY@student.mmu.edu.my | 0182086422
//Member_3: 243UC246W0 | SOH YI HERN | SOH.YI.HERN@student.mmu.edu.my | 018-2991143
//Member_4: 243UC246W3 | YAP JIET IN | YAP.JIET.IN@student.mmu.edu.my | 011-10991332
// *********************************************************
// Task Distribution
// Member_1: Hash table search, Hash table search step
// Member_2: Data Generation
// Member_3: Radix sort, Radix sort step 
// Member_4: Heap sort, Heap sort step
// *********************************************************

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
using namespace std::chrono;

// ── Data model + CSV reader/writer (was in utils.h/.cpp) ──
struct Record {
    unsigned long long key;
    string value;
};

vector<Record> read_dataset(const string& filename) {
    vector<Record> dataset;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return dataset;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            Record rec;
            rec.key = stoull(line.substr(0, commaPos));
            rec.value = line.substr(commaPos + 1);
            dataset.push_back(rec);
        }
    }
    file.close();
    return dataset;
}

void write_dataset(const string& filename, const vector<Record>& dataset) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not open file for writing: " << filename << endl;
        return;
    }
    for (size_t i = 0; i < dataset.size(); i++) {
        outFile << dataset[i].key << "," << dataset[i].value;
        if (i != dataset.size() - 1) outFile << "\n";
    }
    outFile.close();
}

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

int main(int argc, char* argv[]) {
    // Usage: heap_sort <dataset_file.csv>
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv>" << endl;
        return 1;
    }

    string inputFilename = argv[1];

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