#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int minLaptopsRequired(const vector<pair<int, int>>& intervals) {
    vector<pair<int, char>> events;

    // Convert intervals into events
    for (const auto& interval : intervals) {
        events.push_back({interval.first, 's'});   // 's' for start of an interval
        events.push_back({interval.second, 'e'});  // 'e' for end of an interval
    }

    // Sort events: first by time, then by type ('e' should come before 's' when times are the same)
    sort(events.begin(), events.end(), [](const pair<int, char>& a, const pair<int, char>& b) {
        if (a.first == b.first) return a.second > b.second;
        return a.first < b.first;
    });

    // Determine the maximum number of overlapping intervals
    int maxLaptops = 0;
    int currentLaptops = 0;
    for (const auto& event : events) {
        if (event.second == 's') {
            currentLaptops++;  // A new laptop is needed
        } else {
            currentLaptops--;  // A laptop is released
        }
        maxLaptops = max(maxLaptops, currentLaptops);
    }

    return maxLaptops;
}

int main() {
    vector<pair<int, int>> intervals = {
        {0, 2},
        {1, 4},
        {4, 6},
        {0, 4},
        {7, 8},
        {9, 11},
        {3, 10}
    };

    cout << "Minimum number of laptops required: " << minLaptopsRequired(intervals) << endl;

    return 0;
}
