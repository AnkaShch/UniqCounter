#pragma once

#include <utility>
#include <vector>

typedef __gnu_cxx::__normal_iterator<std::pair<int, int> *, std::vector<std::pair<int, int>>> iterator;

class UniqCounter {
public:
    void add(int x);
    int get_uniq_num() const;

private:
    const int MAX_RANGES_NUMB = 30*1024 / sizeof(std::pair<int, int>);
    //const int MAX_RANGES_NUMB = 4;
    std::vector<std::pair<int, int>> ranges;
    double error = 0;
    int covered = 0;

    void add_new_range(int x);
    void insert_range(int x);
    void unite_two_range(int x);
};
