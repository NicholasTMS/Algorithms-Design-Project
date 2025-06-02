#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

struct DataRow {
    int number;
    string text;
};

// swap function
void swap(DataRow &a, DataRow &b) {
    DataRow temp = a;
    a = b;
    b = temp;
}

// partition 
int partition(DataRow arr[], int low, int high) {
    int pivot = arr[high].number;
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j].number < pivot) {
            ++i;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// quickSort 
void quickSort(DataRow arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // use last element 
        quickSort(arr, low, pi - 1); // sort before the partition
        quickSort(arr, pi + 1, high); // sort after the partition
    }
}

// count how many lines are in the CSV
int countRows(const string &filename) {
    ifstream file(filename);
    int count = 0;
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            ++count;
    }
    file.close();
    return count;
}


void readCSVToArray(const string &filename, DataRow arr[], int rowCount) {
    ifstream file(filename);
    string line;
    int idx = 0;
    while (getline(file, line) && idx < rowCount) {
        if (line.empty()) continue; // skiup if empty line
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


void printDataset(const DataRow arr[], int rowCount) {
    for (int i = 0; i < rowCount; ++i) {
        cout << arr[i].number << "," << arr[i].text << "\n";
    }
}

void writeDatasetToFile(const string &outputFilename, const DataRow arr[], int rowCount) {
    ofstream outFile(outputFilename);
    for (int i = 0; i < rowCount; ++i) {
        outFile << arr[i].number << "," << arr[i].text << "\n";
    }

    outFile.close();
}

int main() {
    string filename;
    string outputFilename;
    
    int choice;
    //determine which dataset is being used
    cout << "Select dataset to be used: \n 1. dataset_sample_1000.csv \n 2. dataset_1000000.csv" << endl;
    cin >> choice;
    cout << endl;
    
    if(choice = 1){
        filename = "dataset_sample_1000.csv";
        outputFilename = "quick_sort_1000.csv";
    }
    else{
        filename = "dataset_1000000.csv";
        outputFilename = "quick_sort_1000000.csv";
    }

    // determine rows in the file
    int rowCount = countRows(filename);
    if (rowCount <= 0) {
        cerr << "No data to read or file not found.\n";
        return 1;
    }

    // create an array of that size
    DataRow *dataset = new DataRow[rowCount];

    // read the csv file
    readCSVToArray(filename, dataset, rowCount);

    // print before sorting
    //cout << "Before sorting:\n";
    //printDataset(dataset, rowCount);

    // quicksort
    clock_t start = clock();
    quickSort(dataset, 0, rowCount - 1);
    clock_t end = clock();
    cout << "Quicksort successfull" << endl;
    double time_taken = double(end - start) / CLOCKS_PER_SEC;

    // print after sorting
    //cout << "After sorting:\n";
    //printDataset(dataset, rowCount);

    writeDatasetToFile(outputFilename, dataset, rowCount);
    cout << "Sorted dataset succesfully written to output file" << endl;
    
    delete[] dataset;
    return 0;
}


