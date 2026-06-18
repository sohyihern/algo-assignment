// *********************************************************
// Program: utils.h
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
// Member_1: 243UC246W1 | KOH YOU XIANG | KOH.YOU.XIANG@student.mmu.edu.my | 019-6581165
// Member_2: 243UC246W0 | SOH YI HERN | SOH.YI.HERN@student.mmu.edu.my | 018-2991143
// Member_3: 243UC246W3 | YAP JIET IN | YAP.JIET.IN@student.mmu.edu.my | 011-10991332
// Member_4: 251UC250KN | PATRICK TOH TZY GUAN | PATRICK.TOH.TZY@student.mmu.edu.my | 0182086422
// *********************************************************

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

using namespace std;

// Data structure representing a single row in the CSV
struct Record {
    unsigned long long key;
    string value;
};

// Function to read the dataset
vector<Record> read_dataset(const string& filename);

// Function to write the dataset
void write_dataset(const string& filename, const vector<Record>& dataset);

#endif
