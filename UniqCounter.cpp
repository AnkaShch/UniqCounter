#include <climits>
#include <cstdlib>
#include "UniqCounter.h"

void UniqCounter::add(int x) {
    if (ranges.empty()) {
        ranges.push_back({x, x});
        covered++;
        return;
    }
    iterator it = std::__lower_bound(ranges.begin(), ranges.end(), x,
                                     [](iterator range, int x){return (*range).first <= x;});
    if (it == ranges.end()) {
        it--;
        if (x <= (*it).second + 1) {
            if ((*it).second + 1 == x) {
                (*it).second++;
                covered++;
            } else {
                error -= error / covered;
            }
        } else {
            add_new_range(x);
        }
        return;
    }
    int index = it - ranges.begin();
    if (index == 0) {
        if (x == ranges[0].first - 1) {
            ranges[0].first -= 1;
            covered++;
        } else {
            add_new_range(x);
        }
        return;
    }

    if (ranges[index - 1].first <= x && ranges[index - 1].second >= x) {
        error -= error / covered;
        return;
    }

    if (x == ranges[index].first - 1) {
        if (x == ranges[index - 1].second + 1) {
            ranges[index - 1].second = ranges[index].second;
            ranges.erase(it);
            covered++;
            return;
        } else {
            ranges[index].first -= 1;
            covered++;
            return;
        }
    }
    add_new_range(x);
}

int UniqCounter::get_uniq_num() const {
    return covered - (int)error;
}

void UniqCounter::add_new_range(int x) {
    if (ranges.size() < MAX_RANGES_NUMB - 1) {
        insert_range(x);
    } else {
        unite_two_range(x);
    }
}

void UniqCounter::insert_range(int x) {
    iterator it = std::__lower_bound(ranges.begin(), ranges.end(), x,
                                     [](iterator range, int x){return (*range).first <= x;});
    ranges.insert(it, {x, x});
    covered++;
}

void UniqCounter::unite_two_range(int x) {
    int min_error = INT_MAX, err, index;
    for (int i = 1; i < ranges.size(); ++i) {
        err = ranges[i].first - ranges[i - 1].second;
        if (x > ranges[i - 1].second && x < ranges[i].first) {
            err--;
        }
        if (err < min_error) {
            index = i;
            min_error = err;
        }
    }
    iterator it = std::__lower_bound(ranges.begin(), ranges.end(), x,
                                     [](iterator range, int x){return (*range).first <= x;});

    int index_x = it - ranges.begin();
    if (it == ranges.end()) {
        index_x--;
        err = x - ranges[index_x].second;
    } else if (index_x == 0) {
        err = ranges[0].first - x;
    } else {
        err = std::min(ranges[index_x].first - x, x - ranges[index_x - 1].second);
    }
    if (min_error < err) {
        error += ranges[index].first - ranges[index - 1].second;
        covered += ranges[index].first - ranges[index - 1].second - 1;
        ranges[index - 1].second = ranges[index].second;
        ranges.erase(ranges.begin() + index);
        insert_range(x);
    } else {
        if (x > ranges[index_x].second) {
            error += x - ranges[index_x - 1].second;
            covered += x - ranges[index_x - 1].second;
            ranges[index_x - 1].second = x;
        } else {
            error += ranges[index_x].first - x;
            covered += ranges[index_x].first - x;
            ranges[index_x].first = x;
        }
    }
}