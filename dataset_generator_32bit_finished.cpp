#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cmath>

using namespace std;
using namespace std::chrono;

vector<string> generate_all_possible_strings(int min_len, int max_len)
{
    const string chars = "abcdefghijklmnopqrstuvwxyz";
    vector<string> all_strings;

    // Calculate total number of possible strings
    size_t total = 0;
    for (int len = min_len; len <= max_len; len++)
    {
        total += static_cast<size_t>(pow(chars.size(), len));
    }
    all_strings.reserve(total);

    // Generate all possible combinations
    for (int len = min_len; len <= max_len; len++)
    {
        string s(len, 'a');
        bool more = true;
        while (more)
        {
            all_strings.push_back(s);

            // Increment the string like a number
            more = false;
            for (int i = len - 1; i >= 0; --i)
            {
                if (s[i] < 'z')
                {
                    s[i]++;
                    more = true;
                    break;
                }
                else
                {
                    s[i] = 'a';
                }
            }
        }
    }

    return all_strings;
}

void generate_dataset(int amount, const string &filename)
{
    const int min_str_len = 4;
    const int max_str_len = 5;

    // Pre-generate all possible strings (4-5 chars)
    cout << "Generating all possible " << min_str_len << "-" << max_str_len
         << " character strings..." << endl;
    auto all_strings = generate_all_possible_strings(min_str_len, max_str_len);
    cout << "Generated " << all_strings.size() << " possible strings" << endl;

    // Shuffle the strings for random selection
    cout << "Shuffling strings..." << endl;
    random_device rd;
    mt19937 g(rd());
    shuffle(all_strings.begin(), all_strings.end(), g);

    // Initialize random number generator
    mt19937 generator(rd());
    uniform_int_distribution<uint32_t> num_distribution(1000000001, numeric_limits<uint32_t>::max());

    // Open output file
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Failed to open output file!" << endl;
        return;
    }

    unordered_set<uint32_t> used_numbers;
    used_numbers.reserve(amount);

    cout << "Generating " << amount << " records..." << endl;
    auto start_time = high_resolution_clock::now();
    auto last_report_time = start_time;
    size_t strings_used = 0;
    int duplicates_skipped = 0;

    for (int i = 0; i < amount;)
    {
        // Generate unique number
        uint32_t number = num_distribution(generator);
        if (used_numbers.count(number))
        {
            duplicates_skipped++;
            continue;
        }
        used_numbers.insert(number);

        // Get next unique string
        if (strings_used >= all_strings.size())
        {
            cerr << "Error: Ran out of unique strings!" << endl;
            break;
        }
        string str = all_strings[strings_used++];

        // Write to file
        file << number << "," << str << "\n";
        i++;

        // Progress reporting
        auto current_time = high_resolution_clock::now();
        if (i % 1000000 == 0 ||
            duration_cast<seconds>(current_time - last_report_time).count() >= 5)
        {
            last_report_time = current_time;

            auto elapsed = current_time - start_time;
            auto elapsed_sec = duration_cast<seconds>(elapsed).count();
            auto elapsed_ms = duration_cast<milliseconds>(elapsed).count() % 1000;

            cout << "Generated " << i << " records ("
                 << elapsed_sec << "s " << elapsed_ms << "ms elapsed)" << endl;
        }
    }

    file.close();

    // debugging purposes

    // auto end_time = high_resolution_clock::now();
    // auto duration = end_time - start_time;
    // auto duration_sec = duration_cast<seconds>(duration).count();
    // auto duration_ms = duration_cast<milliseconds>(duration).count() % 1000;

    // cout << "\nGeneration completed!" << endl;
    // cout << "Total time: " << duration_sec << "s " << duration_ms << "ms" << endl;
    // cout << "Total duplicates skipped: " << duplicates_skipped << endl;
    // cout << "Strings used: " << strings_used << "/" << all_strings.size() << endl;
    // cout << amount << " unique records written to " << filename << endl;

    // double rate = amount / (duration_sec + duration_ms / 1000.0);
    // if (rate > 1000000)
    // {
    //     cout << "Generation rate: " << rate / 1000000 << " million records/second" << endl;
    // }
    // else if (rate > 1000)
    // {
    //     cout << "Generation rate: " << rate / 1000 << " thousand records/second" << endl;
    // }
    // else
    // {
    //     cout << "Generation rate: " << rate << " records/second" << endl;
    // }
}

int main()
{
    int amount = 0;

    cout << "Enter dataset size: ";
    cin >> amount;

    // 100 million records
    const string filename = ("dataset_" + to_string(amount) + ".csv");

    generate_dataset(amount, filename);

    return 0;
}