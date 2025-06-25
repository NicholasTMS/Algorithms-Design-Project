#include <iostream>

#include <fstream>

#include <random>        // used for mt199937 (pseudo random number generator with very big potential values, very nice, very good, 10/10)
#include <unordered_set> // used to stored the values, using unordered_set cuz it'll ignore duplicate values.

#include <string> // obv. if you ask.... man...

using namespace std;

unordered_set<uint32_t> random_num(int size)
{
    // cout << "Generator" << endl;

    random_device seed;
    mt19937 generator(seed());
    uniform_int_distribution<uint32_t> distribution;

    unordered_set<uint32_t> unique_nums;
    while (unique_nums.size() < size)
    {
        // cout << "Generating" << endl;

        uint32_t random_val = distribution(generator);

        if (random_val <= 1000000000)
        {
            continue;
        }

        unique_nums.insert(random_val);
    }

    cout << "unique 32-Bit integer generated!" << endl;

    return unique_nums;
}

unordered_set<string> random_str(int size)
{
    random_device seed;
    mt19937 generator(seed());
    uniform_int_distribution<char> char_distribution{'a', 'z'};
    uniform_int_distribution<int> len_distribution(4, 5);

    unordered_set<string> unique_str;
    while (unique_str.size() < size)
    {
        int length = len_distribution(generator);
        string str;
        str.reserve(length);

        for (int i = 0; i < length; ++i)
        {
            str.push_back(char_distribution(generator));
        }

        unique_str.insert(str);
    }

    cout << "unique String generated!" << endl;

    return unique_str;
}

int main()
{

    int amount = 100000000;

    unordered_set<uint32_t> numbers = random_num(amount);
    unordered_set<string> strings = random_str(amount);

    ofstream file;

    file.open("dataset.csv", ios::out);

    // Generate combined output (number,string pairs)
    auto num_it = numbers.begin();
    auto str_it = strings.begin();

    for (int i = 0; i < amount && num_it != numbers.end() && str_it != strings.end(); ++i, ++num_it, ++str_it)
    {
        // cout << "Line " << i + 1 << " : " << *num_it << "/" << *str_it << endl;
        file << *num_it << "/" << *str_it << endl;
    }

    cout << amount << " data generated in Dataset.csv" << endl;

    return 0;
}