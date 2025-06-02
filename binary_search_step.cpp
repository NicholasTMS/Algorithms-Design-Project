#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct DataRow {
    int number;
    string text;
};

// Count non‐empty lines in the CSV
int countRows(const string &filename) {
    ifstream file(filename);
    int count = 0;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) ++count;
    }
    file.close();
    return count;
}

// Read exactly rowCount lines from CSV into arr[]
void readCSVToArray(const string &filename, DataRow arr[], int rowCount) {
    ifstream file(filename);
    string line;
    int idx = 0;
    while (getline(file, line) && idx < rowCount) {
        if (line.empty()) continue;
        stringstream ss(line);
        string numStr, text;
        getline(ss, numStr, ',');
        getline(ss, text);
        arr[idx].number = stoi(numStr);
        arr[idx].text = text;
        ++idx;
    }
    file.close();
}

// Binary search with step logging
void binarySearchWithLogging(const DataRow dataset[], int rowCount, int target) {
    ostringstream filenameStream;
    filenameStream << "binary_search_step_" << target << ".txt";
    string logFilename = filenameStream.str();
    ofstream logFile(logFilename);

    int low = 0, high = rowCount - 1;
    bool found = false;

    while (low <= high) {
        int mid = (low + high) / 2;

        // Log current step (1-based index for row number)
        logFile << (mid + 1) << ": " << dataset[mid].number << "/" << dataset[mid].text << "\n";

        if (dataset[mid].number == target) {
            found = true;
            break;
        } else if (dataset[mid].number < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (!found) {
        logFile << "-1\n";
    }

    logFile.close();
    cout << "Binary search steps written to: " << logFilename << endl;
}

int main() {
    const string sortedFilename = "quick_sort_1000.csv";

    int rowCount = countRows(sortedFilename);

    DataRow* dataset = new DataRow[rowCount];
    readCSVToArray(sortedFilename, dataset, rowCount);

    int target;
    cout << "Enter target integer to search: ";
    cin >> target;

    binarySearchWithLogging(dataset, rowCount, target);

    delete[] dataset;
    return 0;
}
