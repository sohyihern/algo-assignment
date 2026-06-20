// *********************************************************
// Program: hash_table_search_step.cpp
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
// Member_2:
// Member_3:
// Member_4: Hash table search step
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

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

// Input is taken from the command line — see usage in main().

// --- AVL Tree ---

struct AVLNode {
    unsigned long long key;
    string value;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(unsigned long long k, string v)
        : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
public:
    AVLNode* root;

    AVLTree() : root(nullptr) {}

    int height(AVLNode* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    int getBalance(AVLNode* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x  = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left  = T2;

        y->height = max(height(y->left),  height(y->right))  + 1;
        x->height = max(height(x->left),  height(x->right))  + 1;

        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y  = x->right;
        AVLNode* T2 = y->left;

        y->left  = x;
        x->right = T2;

        x->height = max(height(x->left),  height(x->right))  + 1;
        y->height = max(height(y->left),  height(y->right))  + 1;

        return y;
    }

    AVLNode* insert(AVLNode* node, unsigned long long key, const string& value) {
        if (node == nullptr)
            return new AVLNode(key, value);

        if (key < node->key)
            node->left  = insert(node->left,  key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else
            return node; // duplicate keys not allowed

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void insert(unsigned long long key, const string& value) {
        root = insert(root, key, value);
    }

    // Search and record every node visited, then print final result line
    bool search_and_record(unsigned long long key, ofstream& outFile, string& foundValue) {
        AVLNode* curr  = root;
        bool     first = true;

        while (curr != nullptr) {
            if (!first) outFile << " -> ";
            outFile << curr->key;
            first = false;

            if (key == curr->key) {
                foundValue = curr->value;
                outFile << " (Target FOUND!)\n";
                return true;
            } else if (key < curr->key) {
                outFile << " (Go LEFT)";
                curr = curr->left;
            } else {
                outFile << " (Go RIGHT)";
                curr = curr->right;
            }
        }

        if (!first) outFile << " (Hit NULL, NOT FOUND)\n";
        return false;
    }
};

// --- Hash Table ---

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

int nextPrime(int n) {
    while (!isPrime(n)) n++;
    return n;
}

class HashTable {
private:
    int              table_size;
    vector<AVLTree>  table;

public:
    HashTable(int size) {
        table_size = nextPrime(size);
        table.resize(table_size);
    }

    int hashFunction(unsigned long long key) {
        return key % table_size;
    }

    void insert(unsigned long long key, const string& value) {
        int index = hashFunction(key);
        table[index].insert(key, value);
    }

    // Search for key and write full step trace to output file
    bool search_and_record(unsigned long long key, const string& outFilename) {
        ofstream outFile(outFilename);
        if (!outFile.is_open()) {
            cout << "Error: Could not open " << outFilename << endl;
            return false;
        }

        int index = hashFunction(key);

        outFile << "Hash Index: " << index << "\n";
        outFile << "Search Path: ";

        string foundValue;
        bool found = table[index].search_and_record(key, outFile, foundValue);

        // Final result line — matches assignment PDF format exactly
        if (found) {
            outFile << key << " = " << key << "/" << foundValue << "\n";
        } else {
            outFile << "-1 != " << key << "\n";
        }

        outFile.close();
        return found;
    }
};

// --- Main ---

string getSizeFromFilename(const string& filename) {
    size_t start = filename.find("dataset_");
    size_t end   = filename.find(".csv");

    if (start == string::npos || end == string::npos) {
        return "n";
    }

    start += 8;
    return filename.substr(start, end - start);
}

int main(int argc, char* argv[]) {
    // Usage: hash_table_search_step <dataset_file.csv> <target_key>
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv> <target_key>" << endl;
        return 1;
    }

    const string INPUT_FILE = argv[1];
    const unsigned long long TARGET = stoull(argv[2]);

    // Load dataset
    vector<Record> dataset = read_dataset(INPUT_FILE);

    if (dataset.empty()) {
        cout << "Error: No records found in " << INPUT_FILE << endl;
        return 1;
    }

    int n = dataset.size();

    // Build hash table
    HashTable ht(n);
    for (const auto& rec : dataset) {
        ht.insert(rec.key, rec.value);
    }

    // Build output filename: dataset_<n>_hash_table_search_step_<target>.txt
    string sizeText    = getSizeFromFilename(INPUT_FILE);
    string outFilename = "dataset_" + sizeText
                       + "_hash_table_search_step_"
                       + to_string(TARGET) + ".txt";

    // Run search and write step trace
    bool found = ht.search_and_record(TARGET, outFilename);

    // Print result to console
    cout << "Hash table search step completed." << endl;
    cout << "Dataset: " << INPUT_FILE << " (" << n << " records)" << endl;
    cout << "Target:  " << TARGET << endl;

    if (found) {
        cout << "Result:  FOUND" << endl;
    } else {
        cout << "Result:  NOT FOUND" << endl;
    }

    cout << "Output file: " << outFilename << endl;

    return 0;
}