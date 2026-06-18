// *********************************************************
// Program: heap_sort_step.cpp
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
// Member_1: Hash table search
// Member_2:
// Member_3: 
// Member_4:
// *********************************************************


#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace std::chrono;

// --- AVL Tree Implementation ---

struct AVLNode {
    unsigned long long key;
    string value;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(unsigned long long k, string v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
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
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode* insert(AVLNode* node, unsigned long long key, const string& value) {
        if (node == nullptr)
            return new AVLNode(key, value);

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else // Equal keys not allowed, but if happens, just return
            return node;

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

    // Helper to find the deepest leaf (for true worst case simulation)
    unsigned long long get_deepest_leaf() {
        AVLNode* curr = root;
        if (!curr) return 0;
        while (curr->left != nullptr || curr->right != nullptr) {
            int lh = height(curr->left);
            int rh = height(curr->right);
            if (lh >= rh) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return curr->key;
    }

    // Search and record the path
    bool search_and_record(unsigned long long key, ofstream& outFile, string& foundValue) {
        AVLNode* curr = root;
        bool first = true;
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

    // Pure search for timing analysis (no I/O)
    bool search(unsigned long long key) {
        AVLNode* curr = root;
        while (curr != nullptr) {
            if (key == curr->key) return true;
            else if (key < curr->key) curr = curr->left;
            else curr = curr->right;
        }
        return false;
    }
};

// --- Hash Table Implementation ---

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
    while (!isPrime(n)) {
        n++;
    }
    return n;
}

class HashTable {
private:
    int table_size;
    vector<AVLTree> table;

public:
    HashTable(int size) {
        // Use nearest prime number for hash table size to reduce collisions
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

        // Following the strict format requested by the PDF
        if (found) {
            outFile << key << " = " << key << "/" << foundValue << "\n";
        } else {
            outFile << "-1 != " << key << "\n";
        }

        outFile.close();
        return found;
    }

    bool search(unsigned long long key) {
        int index = hashFunction(key);
        return table[index].search(key);
    }

    // Helper to get roots for Best Case analysis
    vector<unsigned long long> get_roots() {
        vector<unsigned long long> roots;
        for (int i = 0; i < table_size; i++) {
            if (table[i].root != nullptr) {
                roots.push_back(table[i].root->key);
            }
        }
        return roots;
    }

    // Helper to find the absolute worst-case key in the entire Hash Table
    unsigned long long get_worst_case_key() {
        int max_height = -1;
        int max_index = 0;
        for (int i = 0; i < table_size; i++) {
            int h = table[i].height(table[i].root);
            if (h > max_height) {
                max_height = h;
                max_index = i;
            }
        }
        if (max_height <= 0) return 0; // Fallback
        return table[max_index].get_deepest_leaf();
    }
};

int main() {
    long long n;
    cout << "Enter dataset size to search on (e.g. 1000): ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid size." << endl;
        return 1;
    }

    string filename = "datasets/dataset_" + to_string(n) + ".csv";
    cout << "Loading dataset " << filename << "..." << endl;
    
    // Using our common utils to read the file
    vector<Record> dataset = read_dataset(filename);
    if (dataset.empty()) {
        cout << "Failed to read dataset. Ensure it has been generated first!" << endl;
        return 1;
    }

    cout << "Building Hash Table with AVL Tree Collision Resolution..." << endl;
    HashTable ht(n);
    for (const auto& rec : dataset) {
        ht.insert(rec.key, rec.value);
    }
    cout << "Hash Table built successfully." << endl;

    // --- Step 3: Specific Target Search ---
    unsigned long long target;
    cout << "\nEnter a target 10-digit key to search (copy one from the csv, or enter a fake one): ";
    cin >> target;

    string step_filename = "outputs/hash_table_search_step_" + to_string(target) + ".txt";
    bool isFound = ht.search_and_record(target, step_filename);
    cout << "Search result and path written to " << step_filename << endl;
    if (isFound) cout << "-> Target FOUND." << endl;
    else cout << "-> Target NOT FOUND." << endl;

    // --- Step 7: Running Time Analysis ---
    cout << "\nPerforming running time analysis (Best, Average, Worst) for " << n << " searches..." << endl;

    // 1. Best Case: Searching for elements that are exactly at the ROOT of the AVL trees
    vector<unsigned long long> roots = ht.get_roots();
    auto start_best = high_resolution_clock::now();
    for (long long i = 0; i < n; i++) {
        // Rapidly hits the root node and returns immediately
        ht.search(roots[i % roots.size()]);
    }
    auto end_best = high_resolution_clock::now();
    duration<double> time_best = end_best - start_best;

    // 2. Average Case: Searching for randomly picked existing elements from the dataset
    auto start_avg = high_resolution_clock::now();
    for (long long i = 0; i < n; i++) {
        // Will traverse various depths of the tree
        ht.search(dataset[i % n].key);
    }
    auto end_avg = high_resolution_clock::now();
    duration<double> time_avg = end_avg - start_avg;

    // 3. Worst Case: Searching for the element situated at the deepest leaf in the most populated AVL tree bucket.
    // This represents the maximum number of comparisons needed for a successful search.
    unsigned long long worst_key = ht.get_worst_case_key();
    auto start_worst = high_resolution_clock::now();
    for (long long i = 0; i < n; i++) {
        ht.search(worst_key);
    }
    auto end_worst = high_resolution_clock::now();
    duration<double> time_worst = end_worst - start_worst;

    // Write timing output
    string time_filename = "outputs/hash_table_search_dataset_" + to_string(n) + ".txt";
    ofstream timeFile(time_filename);
    if (timeFile.is_open()) {
        timeFile << "Best case time: " << time_best.count() << " seconds\n";
        timeFile << "Average case time: " << time_avg.count() << " seconds\n";
        timeFile << "Worst case time: " << time_worst.count() << " seconds\n";
        timeFile.close();
        cout << "Running times written to " << time_filename << endl;
    }

    cout << "\n--- Time Analysis Summary (" << n << " searches) ---\n";
    cout << "Best case:    " << time_best.count() << " s\n";
    cout << "Average case: " << time_avg.count() << " s\n";
    cout << "Worst case:   " << time_worst.count() << " s\n";

    return 0;
}
