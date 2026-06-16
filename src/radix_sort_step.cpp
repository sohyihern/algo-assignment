// *********************************************************
// Program: radix_sort_step.cpp
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
// Member_2: Radix sort step
// Member_3:
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
const int END_ROW = 7;
// ────────────────────────────────────────────────────────

vector<Record> readDatasetRows(const string &filename, int startRow, int endRow) {
    vector<Record> all = read_dataset(filename);
    vector<Record> subset;

    for (int i = startRow - 1; i < endRow && i < (int)all.size(); i++) {
        subset.push_back(all[i]);
    }

    return subset;
}

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

    // Build output filename: dataset_<n>_radix_sorted_step_<start>_<end>.txt
    string sizeText = getSizeFromFilename(INPUT_FILE);
    string outputFilename = "dataset_" + sizeText
                          + "_radix_sorted_step_"
                          + to_string(START_ROW) + "_"
                          + to_string(END_ROW) + ".txt";

    ofstream outFile(outputFilename);

    if (!outFile) {
        cout << "Error: Cannot create output file." << endl;
        return 1;
    }

    // Print original array before any sorting
    writeStep(outFile, records, "original");

    // LSD Radix Sort — rightmost digit first (place=1 = units digit)
    // Labels count DOWN: d=10 (units), d=9 (tens), ..., d=1 (billions)
    // digitPosition 1 → place=1           → label d=10
    // digitPosition 2 → place=10          → label d=9
    // ...
    // digitPosition 10 → place=1000000000 → label d=1
    unsigned long long place = 1;

    for (int digitPosition = 1; digitPosition <= 10; digitPosition++) {
        countingSortByDigit(records, place);

        int label = 11 - digitPosition;
        writeStep(outFile, records, "d=" + to_string(label));

        place *= 10;
    }

    outFile.close();

    cout << "Radix sort step completed successfully." << endl;
    cout << "Rows processed: " << START_ROW << " to " << END_ROW
         << " (" << records.size() << " records)" << endl;
    cout << "Output file: " << outputFilename << endl;

    return 0;
}