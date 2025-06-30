// binary_search.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

struct DataRow {
    long key;
    std::string text;
};

bool readCSV(const std::string &filename, std::vector<DataRow> &out) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Error: could not open \"" << filename << "\"\n";
        return false;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto sep = line.find(',');
        if (sep == std::string::npos) continue;
        try {
            long k = std::stol(line.substr(0, sep));
            std::string txt = line.substr(sep + 1);
            out.push_back({k, std::move(txt)});
        } catch (...) {
            // skip malformed lines
        }
    }
    return true;
}

int binarySearch(const std::vector<DataRow> &A, long target) {
    int lo = 0, hi = (int)A.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (A[mid].key == target)      return mid;
        else if (A[mid].key < target)  lo = mid + 1;
        else                            hi = mid - 1;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <file.csv> <target>\n";
        return 1;
    }

    const std::string filename = argv[1];
    long target;
    try {
        target = std::stol(argv[2]);
    } catch (...) {
        std::cerr << "Error: target must be an integer\n";
        return 1;
    }

    std::vector<DataRow> data;
    if (!readCSV(filename, data)) {
        return 1;
    }

    // time the search
    auto t0 = std::chrono::high_resolution_clock::now();
    int idx = binarySearch(data, target);
    auto t1 = std::chrono::high_resolution_clock::now();

    double elapsed_ms =
        std::chrono::duration<double, std::milli>(t1 - t0).count();

    // print the time in milliseconds
    std::cout << elapsed_ms << "\n";

    return 0;
}
