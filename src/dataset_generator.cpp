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

    // Replace this seed with your group leader student ID seed.
    // Example from assignment: 243UC247CT -> 2431324730
    unsigned long long seed = 2431324730ULL;

    mt19937_64 rng(seed);

    // ── Fast unique number generation using shuffle ──────
    // Instead of rejection sampling (slow for large n),
    // we fill a sequential range then shuffle it.
    // Range: 1,000,000,000 to 9,999,999,999 (9 billion possible values)
    // For n up to ~500M this is safe with no duplicates guaranteed.
    cout << "Generating " << n << " unique numbers..." << endl;

    // Build sequential array starting from 1,000,000,000
    vector<unsigned long long> numbers(n);
    for (long long i = 0; i < n; i++) {
        numbers[i] = 1000000000ULL + i;
    }

    // Shuffle to randomize order
    shuffle(numbers.begin(), numbers.end(), rng);

    string filename = "dataset_" + to_string(n) + ".csv";
    ofstream outFile(filename);

    if (!outFile) {
        cout << "Error: Cannot create output file." << endl;
        return 1;
    }

    cout << "Writing to file..." << endl;

    for (long long i = 0; i < n; i++) {
        string randomText = generateRandomString(rng);
        outFile << numbers[i] << "," << randomText;

        if (i != n - 1) {
            outFile << "\n";
        }

        // Progress update every 10 million records
        if (n >= 10000000 && i > 0 && i % 10000000 == 0) {
            cout << "Progress: " << i << "/" << n << " records written...\n";
        }
    }

    outFile.close();

    cout << "Dataset generated successfully." << endl;
    cout << "Output file: " << filename << endl;

    return 0;
}