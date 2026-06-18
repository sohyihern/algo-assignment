// *********************************************************
// Program: utils.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
// Member_1: 243UC246W1 | KOH YOU XIANG | KOH.YOU.XIANG@student.mmu.edu.my | 019-6581165
// Member_2: 243UC246W0 | SOH YI HERN | SOH.YI.HERN@student.mmu.edu.my | 018-2991143
// Member_3: 243UC246W3 | YAP JIET IN | YAP.JIET.IN@student.mmu.edu.my | 011-10991332
// Member_4: 251UC250KN | PATRICK TOH TZY GUAN | PATRICK.TOH.TZY@student.mmu.edu.my | 0182086422
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
