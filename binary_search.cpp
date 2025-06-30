// binary_search.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <limits>
#include <sstream>
#include <iomanip>

struct DataRow {
    long key;
    std::string text;
};

bool readCSV(const std::string &fname, std::vector<DataRow> &out) {
    std::ifstream in(fname);
    if (!in) return false;
    std::string line;
    while (std::getline(in, line)) {
        auto sep = line.find(',');
        if (sep == std::string::npos) continue;
        try {
            long k = std::stol(line.substr(0, sep));
            out.push_back({k, line.substr(sep+1)});
        } catch (...) {}
    }
    return true;
}

int binarySearch(const std::vector<DataRow> &A, long target) {
    int lo = 0, hi = (int)A.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo)/2;
        if (A[mid].key == target)      return mid;
        else if (A[mid].key < target)  lo = mid + 1;
        else                            hi = mid - 1;
    }
    return -1;
}

static std::string commafy(long long x) {
    std::ostringstream ss;
    ss.imbue(std::locale(""));
    ss << x;
    return ss.str();
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << "Enter path to sorted CSV file: ";
    std::string fname;
    if (!std::getline(std::cin, fname) || fname.empty()) {
        std::cerr << "No filename\n";
        return 1;
    }

    std::vector<DataRow> data;
    if (!readCSV(fname, data)) {
        std::cerr << "Error opening " << fname << "\n";
        return 1;
    }
    int N = (int)data.size();
    std::cout << "Loaded " << commafy(N) << " records from: " << fname << "\n\n";

    std::cout << "Running benchmark (" << commafy(N) << " searches)...\n";
    // Warm-up
    for (int i = 0; i < N; ++i)
        binarySearch(data, data[i].key);
    std::cout << "Warming up... done.\n\n";

    // Benchmark
    long long best_ns  = std::numeric_limits<long long>::max();
    long long worst_ns = 0;
    long long sum_ns   = 0;

    auto start_all = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        auto t0 = std::chrono::high_resolution_clock::now();
        binarySearch(data, data[i].key);
        auto t1 = std::chrono::high_resolution_clock::now();
        long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
        sum_ns += ns;
        if (ns < best_ns)  best_ns  = ns;
        if (ns > worst_ns) worst_ns = ns;
    }
    auto end_all = std::chrono::high_resolution_clock::now();
    long long total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_all - start_all).count();

    double best_ms    = best_ns  / 1e6;
    double worst_ms   = worst_ns / 1e6;
    double avg_ns     = double(sum_ns) / N;
    double avg_ms     = avg_ns   / 1e6;
    double total_ms   = total_ns / 1e6;
    double throughput = N / total_ms;  // searches per ms

    // Output
    std::cout << "\nPerformance Metrics:\n";
    std::cout << "--------------------------------\n";
    std::cout << "Dataset size:  " << commafy(N) << " records\n";
    std::cout << "Best case:     " << commafy(best_ns)  << " ns"
              << " (" << std::fixed << std::setprecision(6) << best_ms  << " ms)\n";
    std::cout << "Worst case:    " << commafy(worst_ns) << " ns"
              << " (" << std::fixed << std::setprecision(6) << worst_ms << " ms)\n";
    std::cout << "Average:       " << commafy((long long)avg_ns) << " ns"
              << " (" << std::fixed << std::setprecision(6) << avg_ms   << " ms)\n";
    std::cout << "--------------------------------\n";
    std::cout << "Total time:    " << commafy(total_ns) << " ns"
              << " (" << std::fixed << std::setprecision(3) << total_ms   << " ms)\n";
    std::cout << "Throughput:    " << std::fixed << std::setprecision(1)
              << throughput << " searches/ms\n";
    std::cout << "--------------------------------\n";

    return 0;
}
