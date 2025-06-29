// binary_search_timed.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>

struct DataRow { long number; std::string text; };

bool readCSV(const std::string &filename, std::vector<DataRow> &data) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string line;
    while (std::getline(file, line)) {
        auto sep = line.find(',');
        if (sep == std::string::npos) continue;
        try {
            DataRow d;
            d.number = std::stol(line.substr(0, sep));
            d.text   = line.substr(sep+1);
            data.push_back(std::move(d));
        } catch(...) { }
    }
    return true;
}

int binarySearch(const std::vector<DataRow>& A, long target) {
    int lo = 0, hi = (int)A.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo)/2;
        if (A[mid].number == target) return mid;
        if (A[mid].number <  target) lo = mid + 1;
        else                           hi = mid - 1;
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <file.csv> <target>\n";
        return 1;
    }
    std::vector<DataRow> data;
    if (!readCSV(argv[1], data)) {
        std::cerr << "Cannot open " << argv[1] << "\n";
        return 1;
    }
    long target = std::stol(argv[2]);

    auto t0 = std::chrono::high_resolution_clock::now();
    int idx = binarySearch(data, target);
    auto t1 = std::chrono::high_resolution_clock::now();

    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    // Print milliseconds to stdout:
    std::cout << ms << "\n";

    return 0;
}
