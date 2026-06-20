// *********************************************************
// Program: Dataset Generator
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
// Member_2: Dataset generator
// Member_3: 
// Member_4:
// *********************************************************

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <algorithm>
#include <unordered_set>

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
    // Group leader student ID: 243UC246W1
    // U -> 1, C -> 3, W -> 3
    // Seed: 2431324631
    unsigned long long seed = 2431324631ULL;

    long long sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000};
    
    for (long long n : sizes) {
        cout << "\n============================================\n";
        cout << "Generating dataset size: " << n << " directly to file..." << endl;

        string filename = "../datasets/dataset_" + to_string(n) + ".csv";
        ofstream outFile(filename);

        if (!outFile) {
            cout << "Error: Cannot create output file " << filename << endl;
            continue;
        }

        // Reset the RNG seed each time to ensure smaller datasets are exact subsets of larger ones
        mt19937_64 rng(seed);

        // True random distribution, range: 1,000,000,000 to 9,999,999,999
        uniform_int_distribution<unsigned long long> numDist(1000000000ULL, 9999999999ULL);
        unordered_set<unsigned long long> generatedNumbers;
        // Pre-allocate hash table size to improve performance
        generatedNumbers.reserve(n);

        cout << "Writing to file..." << endl;

        for (long long i = 0; i < n; i++) {
            unsigned long long num;
            
            // Generate random number and check for duplicates
            do {
                num = numDist(rng);
            } while (generatedNumbers.find(num) != generatedNumbers.end());
            
            generatedNumbers.insert(num);

            string randomText = generateRandomString(rng);
            outFile << num << "," << randomText;

            if (i != n - 1) {
                outFile << "\n";
            }

            // Progress update: print every 1 million records
            if (i > 0 && i % 1000000 == 0) {
                cout << "Progress: " << i << " / " << n << " records written...\n";
            }
        }

        outFile.close();
        cout << "Output file created: " << filename << endl;
    }

    cout << "\nAll datasets generated successfully." << endl;
    return 0;
}