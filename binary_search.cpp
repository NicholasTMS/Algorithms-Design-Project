// binary_search_timed.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>     // for timing

struct DataRow {
    int number;
    std::string text;
};

bool readCSV(const std::string& filename, std::vector<DataRow>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open \"" << filename << "\"\n";
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string numStr, txt;
        if (!std::getline(ss, numStr, '/') ||
            !std::getline(ss, txt))
            continue;  // skip malformed
        try {
            DataRow row;
            row.number = std::stoi(numStr);
            row.text   = std::move(txt);
            data.push_back(std::move(row));
        } catch (...) {
            // skip bad lines
        }
    }
    return true;
}

int binarySearch(const std::vector<DataRow>& data, int target) {
    int low  = 0;
    int high = static_cast<int>(data.size()) - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (data[mid].number == target) {
            return mid;
        } else if (data[mid].number < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <sorted_dataset.csv> <target_integer>\n";
        return 1;
    }

    // 1) Load data
    std::vector<DataRow> data;
    if (!readCSV(argv[1], data)) {
        return 1;
    }

    // 2) Parse target
    int target;
    try {
        target = std::stoi(argv[2]);
    } catch (...) {
        std::cerr << "Error: target must be an integer\n";
        return 1;
    }

    // 3) Time the binary search
    auto t0 = std::chrono::high_resolution_clock::now();
    int index = binarySearch(data, target);
    auto t1 = std::chrono::high_resolution_clock::now();

    // 4) Compute elapsed ms
    double elapsed_ms =
        std::chrono::duration<double, std::milli>(t1 - t0).count();

    // 5) Report
    std::cout << "Search time: " << elapsed_ms << " ms\n";
    if (index >= 0) {
        std::cout << "Found target at index: " << index << "\n";
        std::cout << "Value/text: "
                  << data[index].number
                  << " / \"" << data[index].text << "\"\n";
    } else {
        std::cout << "Target not found\n";
    }

    return 0;
}
