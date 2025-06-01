#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

struct DataRow {
    int number;
    string text;
};

int stepCounter;          
ofstream logFile;          

// swap two DataRow elements
void swap(DataRow &a, DataRow &b) {
    DataRow temp = a;
    a = b;
    b = temp;
}

// log the current state of the subarray being sorted
void logSubarrayState(DataRow subarr[], int size, int pi) {
    // write pivot index for this partition
    logFile << "pi=" << pi << " [";
    for (int i = 0; i < size; ++i) {
        logFile << subarr[i].number << "/" << subarr[i].text;
        if (i != size - 1) logFile << ", ";
    }
    logFile << "]\n";
}

// partition function but logs only within subarray
int partition(DataRow subarr[], int low, int high, int size) {
    int pivot = subarr[high].number;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (subarr[j].number < pivot) {
            ++i;
            swap(subarr[i], subarr[j]);
        }
    }
    swap(subarr[i + 1], subarr[high]);
    int pi = i + 1;
    logSubarrayState(subarr, size, pi);
    return pi;
}

// quickSorts each subarray and logging each partition step
void quickSort(DataRow subarr[], int low, int high, int size) {
    if (low < high) {
        int pi = partition(subarr, low, high, size);
        quickSort(subarr, low, pi - 1, size);
        quickSort(subarr, pi + 1, high, size);
    }
}


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

// read exactly rowCount lines from CSV into arr[]
void readCSVToArray(const string &filename, DataRow arr[], int rowCount) {
    ifstream file(filename);
    string line;
    int index = 0;
    while (getline(file, line) && index < rowCount) {
        if (line.empty()) continue;
        stringstream ss(line);
        string numStr, text;
        getline(ss, numStr, ',');
        getline(ss, text);
        arr[index].number = stoi(numStr);
        arr[index].text = text;
        ++index;
    }
    file.close();
}

// write the fully sorted subarray back to a CSV
void writeDatasetToFile(const string &outputFilename, const DataRow arr[], int rowCount) {
    ofstream outFile(outputFilename);
    for (int i = 0; i < rowCount; ++i) {
        outFile << arr[i].number << "," << arr[i].text << "\n";
    }
    outFile.close();
}

int main() {
    const string filename = "dataset_sample_1000.csv";
    const string sortedOutput = "quick_sort_1000.csv";

    //read CSV (1000 rows)
    int totalRows = countRows(filename);
    DataRow *dataset = new DataRow[totalRows];
    readCSVToArray(filename, dataset, totalRows);

    // get start row and end rwo
    int startRow, endRow;
    cout << "Enter start row (1 to " << totalRows << "): ";
    cin >> startRow;
    cout << "Enter end row (" << startRow << " to " << totalRows << "): ";
    cin >> endRow;
    if (startRow < 1 || endRow < startRow || endRow > totalRows) {
        cerr << "Invalid range.\n";
        delete[] dataset;
        return 1;
    }

    //derive subarray pointer and size
    int size = endRow - startRow + 1;
    DataRow *subarr = dataset + (startRow - 1);

    //open a log file named: quick_sort_step_startRow_endRow.txt
    ostringstream oss;
    oss << "quick_sort_step_" << startRow << "_" << endRow << ".txt";
    string logName = oss.str();
    logFile.open(logName);

    // log the initial subarray 
    logFile << "[";
    for (int i = 0; i < size; ++i) {
        logFile << subarr[i].number << "/" << subarr[i].text;
        if (i != size - 1) logFile << ", ";
    }
    logFile << "]\n";

    //perform QuickSort on just that subarray and logging each partition step
    quickSort(subarr, 0, size - 1, size);

    logFile.close();
    cout << "Partition steps logged to: " << logName << "\n";

    delete[] dataset;
    return 0;
}

