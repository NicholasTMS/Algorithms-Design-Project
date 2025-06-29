// merge_sort_timed.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <chrono>

using namespace std;

// A struct to hold one record of the dataset
struct Data {
    long key;
    string value;
};

// Merge the two sorted ranges [first, mid) and [mid, last)
template<typename RandomIt, typename Compare>
void merge_range(RandomIt first, RandomIt mid, RandomIt last, Compare comp) {
    vector<typename RandomIt::value_type> buffer;
    buffer.reserve(distance(first, last));

    RandomIt i = first, j = mid;
    while (i != mid && j != last) {
        if (comp(*i, *j))
            buffer.push_back(*i++);
        else
            buffer.push_back(*j++);
    }

    // copy any leftovers
    buffer.insert(buffer.end(), i, mid);
    buffer.insert(buffer.end(), j, last);

    // move back into original range
    move(buffer.begin(), buffer.end(), first);
}

// Classic recursive merge-sort over [first, last)
template<typename RandomIt, typename Compare>
void merge_sort(RandomIt first, RandomIt last, Compare comp) {
    auto len = distance(first, last);
    if (len <= 1) return;
    RandomIt mid = first + len/2;
    merge_sort(first, mid, comp);
    merge_sort(mid, last, comp);
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
    ifstream in(argv[1]);
    if (!in) {
        cerr << "Error: could not open \"" << argv[1] << "\"\n";
        return 1;
    }

    // 2) Read and parse each line "number/value"
    vector<Data> vec;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        auto sep = line.find('/');
        if (sep == string::npos) continue;  // skip bad lines
        string numPart = line.substr(0, sep);
        string txtPart = line.substr(sep + 1);
        try {
            Data d;
            d.key   = stol(numPart);
            d.value = move(txtPart);
            vec.push_back(move(d));
        } catch (const exception&) {
            // skip lines where stol() fails
            continue;
        }
    }

    // 3) Time the merge_sort
    auto t0 = chrono::high_resolution_clock::now();
    merge_sort(vec.begin(), vec.end(),
               [](const Data &a, const Data &b){ return a.key < b.key; });
    auto t1 = chrono::high_resolution_clock::now();

    double elapsed_ms =
        chrono::duration<double, milli>(t1 - t0).count();

    // 4) Report timing to stderr
    cerr << "Merge sort elapsed time: "
         << elapsed_ms << " ms\n";

    // 5) Output sorted data to stdout
    for (auto &d : vec) {
        cout << d.key << "/" << d.value << "\n";
    }

    return 0;
}
