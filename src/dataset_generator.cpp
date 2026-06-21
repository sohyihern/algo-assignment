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
// Member_2: Dataset generator，radix_sort.cpp, radix_sort_step.cpp
// Member_3: 
// Member_4:
// *********************************************************

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <cstdlib>

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

int main(int argc, char* argv[]) {
    // Usage: dataset_generator <n>
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <n>" << endl;
        return 1;
    }

    long long n = atoll(argv[1]);

    if (n <= 0) {
        cout << "Invalid dataset size." << endl;
        return 1;
    }

    // Valid 10-digit range: 1,000,000,000 .. 9,999,999,999 (9 billion values)
    if (n > 9000000000LL) {
        cout << "Dataset size exceeds the number of unique 10-digit keys (9,000,000,000)." << endl;
        return 1;
    }

    // Replace this seed with your group leader student ID seed.
    // Example from assignment: 243UC247CT -> 2431324730
    unsigned long long seed = 2431324631ULL;

    mt19937_64 rng(seed);

    cout << "Generating " << n << " unique random numbers..." << endl;

    // ── Unique random 10-digit keys via rejection sampling ──
    // Draw a uniformly random key in [1,000,000,000 .. 9,999,999,999] and
    // keep it only if it has not appeared before; otherwise draw again.
    // Because the range (9 billion) is far larger than n, repeats are rare,
    // so this stays close to O(n). uniform_int_distribution gives an even
    // spread with no modulo bias.
    //
    // To check "have I seen this key?" quickly we use our own open-addressing
    // hash table `taken` (sized ~2n so it stays about half full, keeping the
    // linear probe short). We deliberately avoid std::set / unordered_set,
    // since the assignment disallows containers that search internally.
    // A slot value of 0 means "empty" (a real key is always >= 1,000,000,000).
    const unsigned long long LO = 1000000000ULL;
    const unsigned long long HI = 9999999999ULL;
    uniform_int_distribution<unsigned long long> keyDist(LO, HI);

    const unsigned long long tableSize = 2ULL * (unsigned long long)n + 1ULL;
    vector<unsigned long long> taken(tableSize, 0);

    vector<unsigned long long> numbers;
    numbers.reserve(n);

    while ((long long)numbers.size() < n) {
        unsigned long long key = keyDist(rng);

        // Linear probing: walk from the home slot until we meet either this
        // same key (duplicate -> reject) or an empty slot (key is new).
        unsigned long long slot = key % tableSize;
        bool duplicate = false;
        while (taken[slot] != 0) {
            if (taken[slot] == key) {
                duplicate = true;
                break;
            }
            slot = (slot + 1) % tableSize;
        }
        if (duplicate) continue;

        taken[slot] = key;      // remember this key so it can't repeat
        numbers.push_back(key); // accept it into the dataset
    }

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