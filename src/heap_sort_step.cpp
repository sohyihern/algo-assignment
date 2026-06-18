// *********************************************************
// Program: heap_sort_step.cpp
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
// Member_2:
// Member_3: Heap sort step
// Member_4:
// *********************************************************

#include "utils.h"
#include <iostream>
#include <fstream>

using namespace std;

// ── Configure these before running ──────────────────────
// *dataset_n.csv
const string INPUT_FILE = "dataset_1000.csv";
// *start row (1-based row number in csv file)
const int START_ROW = 1;
// *end row (1-based, inclusive)
const int END_ROW   = 7;
// ────────────────────────────────────────────────────────

vector<Record> readDatasetRows(const string &filename, int startRow, int endRow) {
    vector<Record> all = read_dataset(filename);
    vector<Record> subset;

    for (int i = startRow - 1; i < endRow && i < (int)all.size(); i++) {
        subset.push_back(all[i]);
    }

    return subset;
}

void writeStep(ofstream &outFile, const vector<Record> &records, const string &label) {
    outFile << "[";

    for (size_t i = 0; i < records.size(); i++) {
        outFile << records[i].key << "/" << records[i].value;
        if (i != records.size() - 1) {
            outFile << ", ";
        }
    }

    outFile << "] " << label << "\n";
}

// Heapify down from index i in array of size n (max-heap)
void heapify(vector<Record> &records, int n, int i) {
    int largest = i;
    int left    = 2 * i + 1;
    int right   = 2 * i + 2;

    if (left < n && records[left].key > records[largest].key) {
        largest = left;
    }

    if (right < n && records[right].key > records[largest].key) {
        largest = right;
    }

    if (largest != i) {
        swap(records[i], records[largest]);
        heapify(records, n, largest);
    }
}

string getSizeFromFilename(const string &filename) {
    size_t start = filename.find("dataset_");
    size_t end   = filename.find(".csv");

    if (start == string::npos || end == string::npos) {
        return "n";
    }

    start += 8;
    return filename.substr(start, end - start);
}

int main() {
    vector<Record> records = readDatasetRows(INPUT_FILE, START_ROW, END_ROW);

    if (records.empty()) {
        cout << "No records found. Check START_ROW/END_ROW against file size." << endl;
        return 1;
    }

    int n = records.size();

    // Build output filename: dataset_<n>_heap_sorted_step_<start>_<end>.txt
    string sizeText = getSizeFromFilename(INPUT_FILE);
    string outputFilename = "dataset_" + sizeText
                          + "_heap_sorted_step_"
                          + to_string(START_ROW) + "_"
                          + to_string(END_ROW) + ".txt";

    ofstream outFile(outputFilename);

    if (!outFile) {
        cout << "Error: Cannot create output file." << endl;
        return 1;
    }

    // Phase 1: Build max-heap (bottom-up)
    // Start from last non-leaf node down to root
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(records, n, i);
    }

    // Print the initial max-heap state (after build-heap, before extraction)
    writeStep(outFile, records, "initial");

    // Phase 2: Extract max one by one
    // Print state after EACH extraction (i = n-1 down to 1)
    for (int i = n - 1; i > 0; i--) {
        swap(records[0], records[i]);
        heapify(records, i, 0);
        writeStep(outFile, records, "i = " + to_string(i));
    }

    outFile.close();

    cout << "Heap sort step completed successfully." << endl;
    cout << "Rows processed: " << START_ROW << " to " << END_ROW
         << " (" << records.size() << " records)" << endl;
    cout << "Output file: " << outputFilename << endl;

    return 0;
}