// *********************************************************
// Program: utils.h
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
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
