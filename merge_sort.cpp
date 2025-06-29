// merge_sort_timed.cpp

#include <iostream>
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

    RandomIt mid = first + len / 2;
    merge_sort(first, mid, comp);
    merge_sort(mid, last, comp);
    merge_range(first, mid, last, comp);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Data> vec;
    string line;

    // 1) Read input from stdin, each line is "number/string"
    while (getline(cin, line)) {
        if (line.empty()) continue;

        // split on '/'
        auto sep = line.find('/');
        if (sep == string::npos) continue;  // skip malformed

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

    // 2) Time the merge-sort
    auto t0 = chrono::high_resolution_clock::now();

    merge_sort(vec.begin(), vec.end(),
               [](auto &a, auto &b){ return a.key < b.key; });

    auto t1 = chrono::high_resolution_clock::now();

    double elapsed_ms =
        chrono::duration<double, milli>(t1 - t0).count();

    // Print timing to stderr so stdout stays clean
    cerr << "Merge sort elapsed time: "
         << elapsed_ms << " ms\n";

    // 3) Output the sorted data to stdout
    for (auto &d : vec) {
        cout << d.key << "/" << d.value << "\n";
    }

    return 0;
}
