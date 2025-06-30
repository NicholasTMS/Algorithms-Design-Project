#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <limits>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct Record
{
    string id;
    string value;

    Record(const string &id, const string &value) : id(id), value(value) {}

    bool operator<(const Record &other) const
    {
        return id < other.id;
    }
};

vector<Record> readCSV(const string &filePath)
{
    vector<Record> records;
    ifstream file(filePath);

    if (!file.is_open())
    {
        throw runtime_error("Could not open file: " + filePath);
    }

    string line;
    while (getline(file, line))
    {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos)
        {
            string id = line.substr(0, commaPos);
            string value = line.substr(commaPos + 1);
            // Trim whitespace
            id.erase(0, id.find_first_not_of(" \t"));
            id.erase(id.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            records.emplace_back(id, value);
        }
    }

    // Verify the records are sorted by ID
    for (size_t i = 1; i < records.size(); i++)
    {
        if (records[i - 1].id > records[i].id)
        {
            throw runtime_error("CSV is not sorted by ID. Problem at line " + to_string(i + 1) +
                                "\nFound: " + records[i - 1].id +
                                " before " + records[i].id);
        }
    }

    return records;
}

const Record *binarySearch(const vector<Record> &records, const string &targetId)
{
    size_t left = 0;
    size_t right = records.size() - 1;

    while (left <= right)
    {
        size_t mid = left + (right - left) / 2;
        const Record &current = records[mid];
        int comparison = current.id.compare(targetId);

        if (comparison == 0)
        {
            return &current;
        }
        else if (comparison < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return nullptr;
}

void benchmarkSearch(const vector<Record> &records)
{
    long long minTime = numeric_limits<long long>::max();
    long long maxTime = numeric_limits<long long>::min();
    long long totalTime = 0;
    size_t totalSearches = records.size();

    cout << "\nRunning benchmark (" << totalSearches << " searches)..." << endl;

    // Warm up
    cout << "Warming up...";
    size_t warmupCount = min(1000, static_cast<int>(records.size()));
    for (size_t i = 0; i < warmupCount; i++)
    {
        binarySearch(records, records[i].id);
    }
    cout << " done." << endl;

    // Main benchmarking
    for (const auto &record : records)
    {
        auto start = high_resolution_clock::now();
        const Record *found = binarySearch(records, record.id);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start).count();

        if (found == nullptr || found->id != record.id)
        {
            cerr << "Search failed for ID: " << record.id << endl;
            continue;
        }

        minTime = min(minTime, duration);
        maxTime = max(maxTime, duration);
        totalTime += duration;
    }

    // Calculate statistics
    double avgTime = static_cast<double>(totalTime) / totalSearches;
    double minMs = minTime / 1'000'000.0;
    double maxMs = maxTime / 1'000'000.0;
    double avgMs = avgTime / 1'000'000.0;

    // Display performance metrics
    cout << "\nPerformance Metrics:" << endl;
    cout << "--------------------------------" << endl;
    cout << "Dataset size:    " << setw(12) << totalSearches << " records" << endl;
    cout << "Best case:       " << setw(12) << minTime << " ns (" << fixed << setprecision(6) << minMs << " ms)" << endl;
    cout << "Worst case:      " << setw(12) << maxTime << " ns (" << fixed << setprecision(6) << maxMs << " ms)" << endl;
    cout << "Average:         " << setw(12) << fixed << setprecision(0) << avgTime << " ns ("
         << fixed << setprecision(6) << avgMs << " ms)" << endl;
    cout << "--------------------------------" << endl;
    cout << "Total time:      " << setw(12) << totalTime << " ns (" << fixed << setprecision(3)
         << totalTime / 1'000'000.0 << " ms)" << endl;
    cout << "Throughput:      " << setw(12) << fixed << setprecision(1)
         << totalSearches / (totalTime / 1'000'000.0) << " searches/ms" << endl;
    cout << "--------------------------------" << endl;
}

int main()
{
    cout << "Enter path to sorted CSV file: ";
    string filePath;
    getline(cin, filePath);

    try
    {
        // Read and validate the sorted CSV file
        vector<Record> records = readCSV(filePath);
        cout << "\nLoaded " << records.size() << " records from: " << filePath << endl;

        // Benchmark search performance
        benchmarkSearch(records);
    }
    catch (const exception &e)
    {
        cerr << "\nError: " << e.what() << endl;
        return 1;
    }

    return 0;
}