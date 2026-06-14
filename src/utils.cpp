// *********************************************************
// Program: utils.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************

#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

vector<Record> read_dataset(const string& filename) {
    vector<Record> dataset;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return dataset;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip empty lines

        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            Record rec;
            // Convert string to unsigned long long (stoull)
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
        if (i != dataset.size() - 1) {
            outFile << "\n";
        }
    }
    
    outFile.close();
}
