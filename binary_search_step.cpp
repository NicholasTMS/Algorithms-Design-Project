#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

struct DataRow {
    int number;
    std::string text;
};



bool readCSV(const std::string &filename, std::vector<DataRow> &data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open \"" << filename << "\"\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string numStr, text;
        if (!std::getline(ss, numStr, ',') ||
            !std::getline(ss, text))
            continue;

        try {
            DataRow row;
            row.number = std::stoi(numStr);
            row.text   = std::move(text);
            data.push_back(std::move(row));
        } catch (...) {
            // skip malformed lines
        }
    }

    return true;
}


int binarySearch(const std::vector<DataRow> &data, int target) {
    int low  = 0;
    int high = static_cast<int>(data.size()) - 1;

    while (low <= high) {
        // avoid overflow:
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


void logSearchSteps(const std::vector<DataRow> &data,
                    int target,
                    const std::string &logFilename)
{
    std::ofstream logFile(logFilename);
    if (!logFile.is_open()) {
        std::cerr << "Warning: could not open log file for writing\n";
        return;
    }

    int low  = 0;
    int high = static_cast<int>(data.size()) - 1;
    int step = 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        logFile << step++ << ": index=" << mid
                << ", value=" << data[mid].number
                << " / \"" << data[mid].text << "\"\n";

        if (data[mid].number == target) {
            return;   // log stops when found
        } else if (data[mid].number < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    logFile << "-1\n";  // not found
}

int main() {
    const std::string filename = "dataset.csv";
    std::vector<DataRow> data;
    if (!readCSV(filename, data)) {
        return 1;
    }

    int target;
    std::cout << "Enter target integer to search: ";
    if (!(std::cin >> target)) {
        std::cerr << "Invalid integer input\n";
        return 1;
    }

    auto t0 = std::chrono::high_resolution_clock::now();
    int idx = binarySearch(data, target);
    auto t1 = std::chrono::high_resolution_clock::now();

   
    std::string logFile = "binary_search_steps_" + std::to_string(target) + ".txt";
    logSearchSteps(data, target, logFile);

    // Report results:
    auto elapsed = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "Search took " << elapsed << " ms\n";

    if (idx >= 0) {
        std::cout << "Found target at index " << idx << "\n";
    } else {
        std::cout << "Target not found\n";
    }

    std::cout << "Detailed steps logged to: " << logFile << "\n";
    return 0;
}
