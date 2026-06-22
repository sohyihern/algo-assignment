// *********************************************************
// Program: hash_table_search_v2.cpp
// Description:
// 1. Reads the dataset and builds the Hash Table.
// 2. Generates a Query CSV file based on the user-specified query size.
// 3. Performs batch search using the query file and generates search results CSV.
// 4. Performs Running Time Analysis (Best/Average/Worst).
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std;
using namespace std::chrono;

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

    // For batch search results (no path, just find value)
    bool search_value(unsigned long long key, string& foundValue) {
        AVLNode* curr = root;
        while (curr != nullptr) {
            if (key == curr->key) {
                foundValue = curr->value;
                return true;
            } else if (key < curr->key) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return false;
    }

    // For timing analysis (no I/O)
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

    bool search(unsigned long long key) {
        int index = hashFunction(key);
        return table[index].search(key);
    }

    bool search_and_record_batch(unsigned long long key, ofstream& outFile) {
        int index = hashFunction(key);
        string foundValue;
        bool found = table[index].search_value(key, foundValue);

        if (found) {
            outFile << key << ",found," << foundValue << "\n";
        } else {
            outFile << key << ",not found,-\n";
        }
        return found;
    }

    vector<unsigned long long> get_roots() {
        vector<unsigned long long> roots;
        for (int i = 0; i < table_size; i++) {
            if (table[i].root != nullptr) {
                roots.push_back(table[i].root->key);
            }
        }
        return roots;
    }
};

int main(int argc, char* argv[]) {
    // Usage: hash_table_search_v2 <dataset_file.csv> <query_size>
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv> <query_size>" << endl;
        return 1;
    }

    string datasetFilename = argv[1];
    int query_size = stoi(argv[2]);
    // Auto-generate filenames
    string queryFilename = "query_" + to_string(query_size) + ".csv";
    string resultFilename = "search_results_query_" + to_string(query_size) + ".txt";

    cout << "========================================================================" << endl;
    cout << "[STAGE 1] Loading and Building Hash Table" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << "Reading Database: " << datasetFilename << "..." << endl;
    
    vector<Record> dataset = read_dataset(datasetFilename);
    if (dataset.empty()) {
        cout << "Failed to read dataset." << endl;
        return 1;
    }

    long long n = dataset.size();
    cout << "Original dataset size: " << n << endl;

    // --- Step 1: Build Hash Table ---
    cout << "Building Hash Table with AVL Tree Collision Resolution..." << endl;
    HashTable ht(n);
    for (const auto& rec : dataset) {
        ht.insert(rec.key, rec.value);
    }
    cout << "Hash Table built successfully." << endl;
    
    // --- Step 2: Generate Query File ---
    cout << "\n========================================================================" << endl;
    cout << "[STAGE 2] Query Generation" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << "Extracting " << query_size << " random queries..." << endl;
    vector<int> indices(n);
    for(int i=0; i<n; i++) indices[i] = i;
    mt19937 rng_q(12345);
    shuffle(indices.begin(), indices.end(), rng_q);

    int actual_size = min((int)n, query_size);
    vector<unsigned long long> generated_queries;
    
    ofstream qFile(queryFilename);
    if(!qFile.is_open()) {
        cerr << "Failed to open output query file." << endl;
        return 1;
    }
    for(int i = 0; i < actual_size; i++) {
        unsigned long long k = dataset[indices[i]].key;
        generated_queries.push_back(k);
        qFile << k << "," << dataset[indices[i]].value << "\n";
    }
    qFile.close();
    cout << "Query CSV saved: " << queryFilename << endl;

    // --- Step 3: Batch Search using the generated queries ---
    cout << "\n========================================================================" << endl;
    cout << "[STAGE 3] Batch Searching" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << "Processing " << actual_size << " queries..." << endl;
    ofstream resultFile(resultFilename);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open output file " << resultFilename << endl;
        return 1;
    }
    resultFile << "query,status,value\n";

    int foundCount = 0;
    for (unsigned long long q : generated_queries) {
        bool found = ht.search_and_record_batch(q, resultFile);
        if (found) foundCount++;
    }
    resultFile.close();
    cout << "=> " << foundCount << " / " << actual_size << " match found." << endl;
    cout << "Result CSV saved: " << resultFilename << endl;

    // --- Step 4: Running Time Analysis (Best, Average, Worst) ---
    cout << "\n========================================================================" << endl;
    cout << "[STAGE 4] Running Time Analysis" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << "Performing running time analysis for " << n << " searches..." << endl;

    int stress_size = max(1LL, n / 100);
    HashTable stress_ht(stress_size);
    for (const auto& rec : dataset) {
        stress_ht.insert(rec.key, rec.value);
    }

    vector<unsigned long long> roots = stress_ht.get_roots();
    auto start_best = high_resolution_clock::now();
    for (long long i = 0; i < n; i++) {
        stress_ht.search(roots[i % roots.size()]);
    }
    auto end_best = high_resolution_clock::now();
    duration<double> time_best = end_best - start_best;

    auto start_avg = high_resolution_clock::now();
    for (long long i = 0; i < n; i++) {
        stress_ht.search(dataset[i % n].key);
    }
    auto end_avg = high_resolution_clock::now();
    duration<double> time_avg = end_avg - start_avg;

    vector<unsigned long long> fake_keys(n);
    mt19937_64 rng(54321);
    for (long long i = 0; i < n; i++) {
        fake_keys[i] = 9900000000ULL + (rng() % 100000000ULL);
    }
    auto start_worst = high_resolution_clock::now();
    for (long long i = 0; i < n; i++) {
        stress_ht.search(fake_keys[i]);
    }
    auto end_worst = high_resolution_clock::now();
    duration<double> time_worst = end_worst - start_worst;

    string time_filename = "hash_table_search_dataset_" + to_string(n) + ".txt";
    ofstream timeFile(time_filename);
    if (timeFile.is_open()) {
        timeFile << "Best case time: " << time_best.count() << " seconds\n";
        timeFile << "Average case time: " << time_avg.count() << " seconds\n";
        timeFile << "Worst case time: " << time_worst.count() << " seconds\n";
        timeFile.close();
    }
    cout << "Performance TXT saved: " << time_filename << endl;

    cout << "\nBest case time:    " << time_best.count() << " seconds\n";
    cout << "Average case time: " << time_avg.count() << " seconds\n";
    cout << "Worst case time:   " << time_worst.count() << " seconds\n";

    cout << "\n========================================================================" << endl;
    cout << "[SUCCESS] Search Pipeline Completed." << endl;
    cout << "========================================================================" << endl;

    return 0;
}
