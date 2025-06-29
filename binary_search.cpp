// binary_search_step.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// A simple struct to hold one row of our dataset
struct DataRow {
    int number;
    std::string text;
};

// Reads a CSV with format “number,text” into `data`.
// Returns false on open failure.
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
        std::string numStr, text;
        if (!std::getline(ss, numStr, ',') || !std::getline(ss, text))
            continue;  // skip malformed lines

        try {
            DataRow row;
            row.number = std::stoi(numStr);
            row.text = std::move(text);
            data.push_back(std::move(row));
        } catch (...) {
            // skip non-integer values
        }
    }

    return true;
}

// Performs binary search on data[].number for `target`.
// Logs each probe (index & value) to `logFilename`, then exits upon found or exhausted.
void binarySearchStep(const std::vector<DataRow>& data,
                      int target,
                      const std::string& logFilename)
{
    std::ofstream logFile(logFilename);
    if (!logFile.is_open()) {
        std::cerr << "Warning: could not open log file \"" << logFilename << "\" for writing\n";
        return;
    }

    int low = 0;
    int high = static_cast<int>(data.size()) - 1;
    int step = 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        // Log the current step:
        logFile << step++ << ": index=" << mid
                << ", value=" << data[mid].number
                << " / \"" << data[mid].text << "\"\n";

        if (data[mid].number == target) {
            // Found — stop logging
            return;
        } else if (data[mid].number < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Not found
    logFile << "-1\n";
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <sorted_dataset.csv> <target_integer>\n";
        return 1;
    }

    std::string datasetFile = argv[1];
    int target;
    try {
        target = std::stoi(argv[2]);
    } catch (...) {
        std::cerr << "Error: target must be an integer\n";
        return 1;
    }

    // 1. Read the sorted CSV into memory
    std::vector<DataRow> data;
    if (!readCSV(datasetFile, data)) {
        return 1;
    }

    // 2. Build the log filename and perform the search
    std::string logFile = "binary_search_step_" + std::to_string(target) + ".txt";
    binarySearchStep(data, target, logFile);

    // 3. Inform the user
    std::cout << "Detailed steps logged to: " << logFile << "\n";
    return 0;
}
