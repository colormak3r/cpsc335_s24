// Authors:
// - Dang Khoa Nguyen (colormak3r@csu.fullerton.edu)

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int start_end[][2] = {
    {0, 2},
    {1, 4},
    {4, 6},
    {0, 4},
    {7, 8},
    {9, 11},
    {3, 10}};

int times[][3] = {
    {2, 1, 1},
    {2, 3, 1},
    {3, 4, 1}};

void printIntervals(int intervals[][2], size_t size)
{
    for (int i = 0; i < size; i++)
    {
        cout << "{" << intervals[i][0] << ", " << intervals[i][1] << "}";
        if (i < size - 1)
        {
            cout << ", ";
        }
    }
}

void printTimes(int times[][3], size_t size)
{
    for (int i = 0; i < size; i++)
    {
        cout << "{" << times[i][0] << ", " << times[i][1] << ", " << times[i][2] << "}";
        if (i < size - 1)
        {
            cout << ", ";
        }
    }
}

int minLaptop(int intervals[][2], size_t size)
{
    // Find the maximum end time from the intervals
    int maxEndTime = 0;
    for (size_t i = 0; i < size; ++i)
    {
        if (intervals[i][1] > maxEndTime)
            maxEndTime = intervals[i][1];
    }

    maxEndTime++; // Account for hour zero
    // An array to track the changes in laptops needed each hour
    int laptopDelta[maxEndTime] = {};

    // Loop through the intervals and
    //  increase the one laptop needed at start time
    //  decrease the one laptop needed at end time
    for (size_t i = 0; i < size; ++i)
    {
        laptopDelta[intervals[i][0]]++;
        laptopDelta[intervals[i][1]]--;
    }

    // Calculate the maximum number of concurrent laptops needed
    int minLaptop = 0;
    int concurrent = 0; // Tracks the current count of laptops in use
    for (size_t i = 0; i < maxEndTime; ++i)
    {
        concurrent += laptopDelta[i];
        if (concurrent > minLaptop)
            minLaptop = concurrent;
    }

    return minLaptop;
}

int minTime(int times[][3], size_t size, int n, int k)
{
    // Create the adjacency list
    vector<vector<pair<int, int>>> adjList(n + 1);
    for (size_t i = 0; i < size; i++) {
        adjList[times[i][0]].push_back({times[i][1], times[i][2]});
    }

    // Min-heap to store minimum travel time to each node
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> distances(n + 1, 99999);
    distances[k] = 0;
    pq.push({k, 0}); // Push starting node with distance 0

    // Dijkstra's algorithm
    while (!pq.empty()) {
        int node = pq.top().first;
        int dist = pq.top().second;
        pq.pop();

        for (auto &edge : adjList[node]) {
            int v = edge.first;
            int weight = edge.second;
            if (dist + weight < distances[v]) {
                distances[v] = dist + weight;
                pq.push({v, distances[v]});
            }
        }
    }

    // Find the maximum distance to ensure all nodes received the signal
    int maxTime = -1;
    for (int i = 1; i <= n; i++) {
        if (distances[i] == 99999) return -1; // Node i is unreachable
        maxTime = max(maxTime, distances[i]);
    }

    return maxTime;
}

int main()
{
    cout << "Welcome to Project 4: Implementing Algorithms.\n"
         << endl;

    // Determine the number of intervals
    size_t size = sizeof(start_end) / sizeof(start_end[0]);

    // Problem 1
    cout << "Problem 1 Input: ";
    printIntervals(start_end, size);
    cout << endl;
    cout << "Minimum number of laptops required: " << minLaptop(start_end, size) << endl;

    cout << endl;
    size = sizeof(times) / sizeof(times[0]);
    int n = 4;
    int k = 2;

    // Problem 2
    cout << "Problem 2 Input: ";
    printTimes(times, size);
    cout << ", n = " << n << ", k = " << k << endl;
    cout << "Minimum time for all nodes to recieve signal is " << minTime(times, size, n, k) << endl;

    return 0;
}
