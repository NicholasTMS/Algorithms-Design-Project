#include <iostream>
#include <vector>
#include <string>
#include <algorithm>  
#include <iterator>   
#include <chrono>      // for timing

using namespace std;

struct Data {
    long key;
    string value;
};

template<typename RandomIt, typename Compare>
void merge_range(RandomIt first, RandomIt mid, RandomIt last, Compare comp) {
    vector<typename RandomIt::value_type> buffer;
    buffer.reserve(distance(first, last));

    RandomIt i = first, j = mid;
    while (i != mid && j != last) {
        if (comp(*i, *j))
            buffer.push_back(*i++);
        else
            buffer.push_back(*j++);
    }

    buffer.insert(buffer.end(), i, mid);
    buffer.insert(buffer.end(), j, last);

    move(buffer.begin(), buffer.end(), first);
}

template<typename RandomIt, typename Compare>
void merge_sort(RandomIt first, RandomIt last, Compare comp) {
    auto len = distance(first, last);
    if (len <= 1) return;
    RandomIt mid = first + len/2;
    merge_sort(first, mid, comp);
    merge_sort(mid, last, comp);
    merge_range(first, mid, last, comp);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Data> vec;
    string line;

    // Read each line "number,string" from stdin
    while (getline(cin, line)) {
        if (line.empty()) continue;
        auto comma = line.find(',');
        if (comma == string::npos) continue;
        Data d;
        d.key   = stol(line.substr(0, comma));
        d.value = line.substr(comma + 1);
        vec.push_back(move(d));
    }

    //  Timing starts here 
    auto t0 = chrono::high_resolution_clock::now();

    merge_sort(vec.begin(), vec.end(),
               [](auto &a, auto &b){ return a.key < b.key; });

    auto t1 = chrono::high_resolution_clock::now();
    //Timing ends here 

    double elapsed_ms =
        chrono::duration<double, milli>(t1 - t0).count();

    cerr << "Merge sort elapsed time: " 
         << elapsed_ms << " ms\n";

    // Print out sorted data
    for (auto &d : vec) {
        cout << d.key << "," << d.value << "\n";
    }

    return 0;
}
