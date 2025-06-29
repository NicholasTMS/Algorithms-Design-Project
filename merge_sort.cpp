// merge_sort.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// A struct to hold one record of the dataset
struct Data {
    long key;
    string value;
};

// Merge the two sorted ranges [first, mid) and [mid, last)
template<typename It, typename Comp>
void merge_range(It first, It mid, It last, Comp comp) {
    vector<typename It::value_type> buffer;
    buffer.reserve(distance(first, last));

    It i = first, j = mid;
    while (i != mid && j != last) {
        if (comp(*i, *j)) buffer.push_back(*i++);
        else               buffer.push_back(*j++);
    }
    buffer.insert(buffer.end(), i, mid);
    buffer.insert(buffer.end(), j, last);
    move(buffer.begin(), buffer.end(), first);
}

// Recursive merge-sort over [first,last)
template<typename It, typename Comp>
void merge_sort(It first, It last, Comp comp) {
    auto len = distance(first, last);
    if (len < 2) return;
    It mid = first + len/2;
    merge_sort(first, mid, comp);
    merge_sort(mid,   last, comp);
    merge_range(first, mid, last, comp);
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <dataset.csv>\n";
        return 1;
    }

    // 1) Open the input file
    ifstream in{argv[1]};
    if (!in) {
        cerr << "Error: could not open \"" << argv[1] << "\"\n";
        return 1;
    }

    // 2) Read and parse each line "number,value"
    vector<Data> vec;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        auto sep = line.find(',');          // look for comma now
        if (sep == string::npos) continue;

        string numPart = line.substr(0, sep);
        string txtPart = line.substr(sep + 1);
        try {
            Data d;
            d.key   = stol(numPart);
            d.value = move(txtPart);
            vec.push_back(move(d));
        } catch (...) {
            // skip bad lines or non-numeric keys
        }
    }

    // 3) Time the merge_sort
    auto t0 = chrono::high_resolution_clock::now();
    merge_sort(vec.begin(), vec.end(),
               [](auto &a, auto &b){ return a.key < b.key; });
    auto t1 = chrono::high_resolution_clock::now();

    double elapsed_ms =
        chrono::duration<double, milli>(t1 - t0).count();

    // 4) Report timing (ms) to stderr
    cerr << elapsed_ms << "\n";

    // 5) Output sorted data to stdout as "number,value"
    for (auto &d : vec) {
        cout << d.key << "," << d.value << "\n";
    }

    return 0;
}
