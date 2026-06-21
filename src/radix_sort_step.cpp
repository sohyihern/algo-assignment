// *********************************************************
// Program: radix_sort_step.cpp
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
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Input is taken from the command line — see usage in main().

// ── Data model + CSV reader (was in utils.h/.cpp) ──
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

int main(int argc, char* argv[]) {
    // Usage: radix_sort_step <dataset_file.csv> <start_row> <end_row>
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv> <start_row> <end_row>" << endl;
        return 1;
    }

    const string INPUT_FILE = argv[1];
    const int START_ROW = stoi(argv[2]);
    const int END_ROW = stoi(argv[3]);

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