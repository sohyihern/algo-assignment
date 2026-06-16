// *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************
// Task Distribution
// Member_1: 
// Member_2:
// Member_3:
// Member_4:
// *********************************************************

#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <string>

using namespace std;

string generateRandomString(mt19937_64 &rng) {
    string result = "";
    uniform_int_distribution<int> letterDist(0, 25);

    for (int i = 0; i < 5; i++) {
        char c = 'a' + letterDist(rng);
        result += c;
    }

    return result;
}

int main() {
    long long n;

    cout << "Enter dataset size: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid dataset size." << endl;
        return 1;
    }

    string filename = ".\\dataset_" + to_string(n) + ".csv";
    ofstream outFile(filename);

    if (!outFile) {
        cout << "Error: Cannot create output file." << endl;
        return 1;
    }

    // Group leader student ID: 243UC246W1
    // U -> 1, C -> 3, W -> 3
    // Seed: 2431324631
    unsigned long long seed = 2431324631ULL;//u is unsign,ll is longlong

    mt19937_64 rng(seed);

    uniform_int_distribution<unsigned long long> intDist(1000000000ULL, 9999999999ULL);

    unordered_set<unsigned long long> usedNumbers;

    cout << "Generating dataset..." << endl;

    for (long long i = 0; i < n; i++) {
        unsigned long long number;

        do {
            number = intDist(rng);
        } while (usedNumbers.find(number) != usedNumbers.end());

        usedNumbers.insert(number);

        string randomText = generateRandomString(rng);

        outFile << number << "/" << randomText;

        if (i != n - 1) {
            outFile << "\n";
        }
    }

    outFile.close();

    cout << "Dataset generated successfully." << endl;
    cout << "Output file: " << filename << endl;

    return 0;
}