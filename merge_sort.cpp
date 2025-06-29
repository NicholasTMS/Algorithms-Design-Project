// merge_sort.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// Data record
struct Data {
    long key;
    string value;
};

// Merge two sorted halves [first,mid) and [mid,last)
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

// Recursive merge sort
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
        cerr << "Usage: " << argv[0] << " <dataset_n.csv>\n";
        return 1;
    }

    string infile = argv[1];
    // Extract the N from dataset_N.csv
    auto uscore = infile.rfind('_');
    auto dot     = infile.rfind('.');
    if (uscore == string::npos || dot == string::npos || uscore >= dot) {
        cerr << "Error: input filename must be like dataset_N.csv\n";
        return 1;
    }
    string N = infile.substr(uscore+1, dot - (uscore+1));

    // Construct output filename
    string outfile = "merge_sort_" + N + ".csv";

    // 1) Read input
    ifstream in{infile};
    if (!in) {
        cerr << "Error: could not open \"" << infile << "\"\n";
        return 1;
    }
    vector<Data> vec;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        auto sep = line.find(',');            // now splitting on comma
        if (sep == string::npos) continue;
        try {
            Data d;
            d.key   = stol(line.substr(0, sep));
            d.value = line.substr(sep+1);
            vec.push_back(move(d));
        } catch (...) {
            continue;
        }
    }
    in.close();

    // 2) Sort & time
    auto t0 = chrono::high_resolution_clock::now();
    merge_sort(vec.begin(), vec.end(),
               [](auto &a, auto &b){ return a.key < b.key; });
    auto t1 = chrono::high_resolution_clock::now();
    double elapsed_ms =
        chrono::duration<double, milli>(t1 - t0).count();

    // 3) Write sorted output
    ofstream out{outfile};
    if (!out) {
        cerr << "Error: could not create \"" << outfile << "\"\n";
        return 1;
    }
    for (auto &d : vec) {
        out << d.key << "," << d.value << "\n";
    }
    out.close();

    // 4) Print running time
    cout << elapsed_ms << "\n";

    return 0;
}
