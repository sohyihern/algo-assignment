// *********************************************************
// Program: radix_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
// Member_1: 243UC246W1 | KOH YOU XIANG | KOH.YOU.XIANG@student.mmu.edu.my | 019-6581165
// Member_2: 243UC246W0 | SOH YI HERN | SOH.YI.HERN@student.mmu.edu.my | 018-2991143
// Member_3: 243UC246W3 | YAP JIET IN | YAP.JIET.IN@student.mmu.edu.my | 011-10991332
// Member_4: 251UC250KN | PATRICK TOH TZY GUAN | PATRICK.TOH.TZY@student.mmu.edu.my | 0182086422
// *********************************************************
// Task Distribution
// Member_1:
// Member_2: Radix sort
// Member_3:
// Member_4:
// *********************************************************

#include "utils.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Counting sort by digit place value: 1, 10, 100, ...
void countingSortByDigit(vector<Record> &records, unsigned long long place) {
    const int base = 10;
    int n = records.size();

    vector<Record> output(n);
    int count[base] = {0};

    for (int i = 0; i < n; i++) {
        int digit = (records[i].key / place) % 10;
        count[digit]++;
    }

    for (int i = 1; i < base; i++) {
        count[i] += count[i - 1];
    }

    // Go from right to left to keep radix sort stable
    for (int i = n - 1; i >= 0; i--) {
        int digit = (records[i].key / place) % 10;
        output[count[digit] - 1] = records[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++) {
        records[i] = output[i];
    }
}

void radixSort(vector<Record> &records) {
    // Since assignment requires 10-digit integers,
    // process from rightmost digit to leftmost digit.
    unsigned long long place = 1;

    for (int digitPosition = 1; digitPosition <= 10; digitPosition++) {
        countingSortByDigit(records, place);
        place *= 10;
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
    // Usage: radix_sort <dataset_file.csv>
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

    radixSort(records);

    auto end = high_resolution_clock::now();

    duration<double> elapsed = end - start;

    string sizeText = getSizeFromFilename(inputFilename);
    string outputFilename = "radix_sorted_dataset_" + sizeText + ".csv";

    write_dataset(outputFilename, records);

    cout << "Radix sort completed successfully." << endl;
    cout << "Input size: " << records.size() << endl;
    cout << "Running time: " << elapsed.count() << " seconds" << endl;
    cout << "Output file: " << outputFilename << endl;

    return 0;
}