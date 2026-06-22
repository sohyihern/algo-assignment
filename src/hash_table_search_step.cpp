// *********************************************************
// Program: hash_table_search_step_v2.cpp
// Description:
// Searches for a SINGLE target key in the Hash Table.
// Records the detailed search path/steps to an output file.
// Usage: hash_table_search_step_v2 <dataset_file.csv> <target_key>
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

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
        if (node == nullptr) return new AVLNode(key, value);
        if (key < node->key) node->left = insert(node->left, key, value);
        else if (key > node->key) node->right = insert(node->right, key, value);
        else return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) return rightRotate(node);
        if (balance < -1 && key > node->right->key) return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void insert(unsigned long long key, const string& value) {
        root = insert(root, key, value);
    }

    // Search and record the path (detailed steps)
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
};

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int nextPrime(int n) {
    while (!isPrime(n)) n++;
    return n;
}

class HashTable {
private:
    int table_size;
    vector<AVLTree> table;

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

        if (found) {
            outFile << key << " = " << key << "/" << foundValue << "\n";
        } else {
            outFile << "-1 != " << key << "\n";
        }

        outFile.close();
        return found;
    }
};

int main(int argc, char* argv[]) {
    // Usage: hash_table_search_step_v2 <dataset_file.csv> <target_key>
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv> <target_key>" << endl;
        return 1;
    }

    string filename = argv[1];
    unsigned long long target = stoull(argv[2]);

    cout << "Loading dataset " << filename << "..." << endl;
    vector<Record> dataset = read_dataset(filename);
    if (dataset.empty()) {
        cout << "Failed to read dataset." << endl;
        return 1;
    }

    long long n = dataset.size();

    cout << "Building Hash Table with AVL Tree Collision Resolution..." << endl;
    HashTable ht(n);
    for (const auto& rec : dataset) {
        ht.insert(rec.key, rec.value);
    }
    cout << "Hash Table built successfully." << endl;

    // Search for the single target key
    string baseName = filename;
    size_t dotPos = baseName.rfind('.');
    if (dotPos != string::npos) baseName = baseName.substr(0, dotPos);
    
    string step_filename = baseName + "_hash_table_search_step_" + to_string(target) + ".txt";
    bool isFound = ht.search_and_record(target, step_filename);
    cout << "Search result and path written to " << step_filename << endl;
    if (isFound) cout << "-> Target FOUND." << endl;
    else cout << "-> Target NOT FOUND." << endl;

    return 0;
}
