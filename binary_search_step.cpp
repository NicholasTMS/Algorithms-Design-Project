#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct DataRow {
    long long number;
    string text;
};

// count rows in CSV
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

// read each row into arr[]
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
        arr[idx].number = stoll(numStr);
        arr[idx].text = text;
        ++idx;
    }
    file.close();
}

// binary search with some loggning
void binarySearchWithLogging(const DataRow dataset[], int rowCount, long long target) {
    ostringstream filenameStream;
    filenameStream << "binary_search_step_" << target << ".txt";
    string logFilename = filenameStream.str();
    ofstream logFile(logFilename);

    int low = 0, high = rowCount - 1;
    bool found = false;

    while (low <= high) {
        int mid = (low + high) / 2;

        // log current step 
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
    int choice;
    string sortedFilename;

    cout << "Select dataset to be used: \n 1. quick_sort_1000.csv \n 2. Other \nEnter choice: ";
    cin >> choice;
    cout << endl;
    
    if(choice == 1){
        sortedFilename = "quick_sort_1000.csv";
    }
    else{
        int size;
        cout << "Enter dataset size: ";
        cin >> size;
        sortedFilename = "quick_sort_" + to_string(size) + ".csv";
    }
    //initilisae empty array 
    int rowCount = countRows(sortedFilename);
    DataRow* dataset = new DataRow[rowCount];

    //fill array from sorted csv file
    readCSVToArray(sortedFilename, dataset, rowCount);

    // actual binary search
    long long target;
    cout << "Enter target integer to search: ";
    cin >> target;

    binarySearchWithLogging(dataset, rowCount, target);

    delete[] dataset;
    return 0;
}
