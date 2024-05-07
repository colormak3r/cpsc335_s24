// Authors:
// - Dang Khoa Nguyen (colormak3r@csu.fullerton.edu)
// - Alexis Nemsingh (alexisnemsingh@csu.fullerton.edu)
// - Michael G Duenas (mduenas1@csu.fullerton.edu)

#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;

vector<vector<char>> baseField =
    {
        {'.', '.', '.', '.', '.', '.', 'X', '.', 'X'},
        {'X', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', 'X', '.', '.', '.', 'X', '.'},
        {'.', '.', 'X', '.', '.', '.', '.', 'X', '.'},
        {'.', 'X', '.', '.', '.', '.', 'X', '.', '.'},
        {'.', '.', '.', '.', 'X', '.', '.', '.', '.'},
        {'.', '.', 'X', '.', '.', '.', '.', '.', 'X'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.'}};

vector<vector<char>> generateField(int numOfRow, int numOfCol, int maxSteps)
{
    // Generate a field of numOfRow and numOfCol filled with '.'
    vector<vector<char>> field(numOfRow, vector<char>(numOfCol, '.'));

    // Limit the amount of opponents per row to an abitrary number of numOfCol / 4
    int maxOpponent = numOfCol / 4;
    if (maxOpponent < 1)
        maxOpponent = 1;

    // Preemtively re-generate uninteresting cases where the field is impassable
    //  up to number of maxSteps
    int r = field.size();    // Number of rows
    int c = field[0].size(); // Number of columns
    int step = 0;
    bool valid = false;
    while (step < maxSteps && !valid)
    {
        // Assign cells with 'X' and shuffle each row for randomization
        random_device rd;
        mt19937 gen(rd());
        for (auto &row : field)
        {
            int opponentCount = 0;
            for (int i = 0; i < maxOpponent; i++)
            {
                row[i] = 'X';
                opponentCount++;
                if (opponentCount >= maxOpponent)
                    break;
            }
            shuffle(row.begin(), row.end(), gen);
        }

        // Determine if the field is impassible or not
        if (field[0][0] != 'X' && field[r - 1][c - 1] != 'X')
            valid = true;

        step++;
    }

    return field;
}

unsigned long algo1(const vector<vector<char>> &field)
{
    int r = field.size();    // Number of rows
    int c = field[0].size(); // Number of columns

    // Corner case: the field is impassable
    if (field[0][0] == 'X' || field[r - 1][c - 1] == 'X')
    {
        return 0;
    }

    int len = r + c - 2;       // Total number of moves needed to reach the bottom-right corner
    unsigned long counter = 0; // To count valid paths

    // Iterate from 0 to 2^𝑙𝑒𝑛 − 1
    for (int bits = 0; bits < (1 << len); bits++)
    {
        vector<char> candidate; // To store the current path of moves
        int x = 0, y = 0;       // Starting position

        bool isValid = true; // flag to check if the current path is valid

        for (int k = 0; k < len; ++k)
        {
            int bit = (bits >> k) & 1;

            if (bit == 1)
            { // Right move
                y++;
            }
            else
            { // Down move
                x++;
            }

            // Check if the move is outside the grid or crosses an 'X' cell
            if (x >= r || y >= c || field[x][y] == 'X')
            {
                isValid = false;
                break;
            }
        }

        // If ends at (r-1, c-1) without invalid moves
        if (isValid && x == r - 1 && y == c - 1)
        {
            counter++;
        }
    }

    return counter;
}

int algo2(const vector<vector<char>> &field)
{
    int r = field.size();    // Number of rows
    int c = field[0].size(); // Number of columns

    // Corner case: the field is impassable
    if (field[0][0] == 'X' || field[r - 1][c - 1] == 'X')
    {
        return 0;
    }

    vector<vector<int>> A(r, vector<int>(c, 0)); // Create r x c matrix initialized to zeroes

    // Base case
    A[0][0] = 1;

    // General cases
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            if (field[i][j] == 'X')
            {
                A[i][j] = 0; // Impassable cell has 0 ways to reach
                continue;
            }

            int above = 0, left = 0;
            // Check cell above unless it's the first row or blocked
            if (i > 0 && field[i - 1][j] != 'X')
            {
                above = A[i - 1][j];
            }

            // Check cell to the left unless it's the first column or blocked
            if (j > 0 && field[i][j - 1] != 'X')
            {
                left = A[i][j - 1];
            }

            // Do not double count the starting position
            if (i != 0 || j != 0)
            {
                A[i][j] = above + left;
            }

            // Debug
            // cout << '\n';
            // for (const auto &row : A)
            // {
            //     for (auto c : row)
            //     {
            //         cout << setfill(' ') << setw(3) << c << ' ';
            //     }
            //     cout << '\n';
            // }
        }
    }

    // Return the number of ways to reach the bottom-right corner
    return A[r - 1][c - 1];
}

int main()
{
    cout << "Welcome to Project 3: Staged Opponent Avoidance Problem.\n"
         << endl;

    // Get the user's rows and columns input
    int numOfCol = -1;
    int numOfRow = -1;
    while (numOfRow <= 0)
    {
        cout << "Please enter a positive number for the number of row of the field: ";
        cin >> numOfRow;
    }
    while (numOfCol <= 0)
    {
        cout << "Please enter a positive number for the number of column of the field: ";
        cin >> numOfCol;
    }

    // Define benchmark variables
    clock_t start;
    clock_t end;
    double elapsed;

    // Run and benchmarck algorithm 1 using the sameple case
    cout << "\nVerifying algorithm with the sample case. The result should be 102." << endl;
    start = clock();
    cout << "Algo 1 return " << algo1(baseField) << endl;
    end = clock();
    elapsed = double(end - start) / CLOCKS_PER_SEC;
    cout << "Time taken by Algo 1: " << elapsed << " seconds." << endl;

    // Run and benchmarck algorithm 2 using the sameple case
    start = clock();
    cout << "Algo 2 return " << algo2(baseField) << endl;
    end = clock();
    elapsed = double(end - start) / CLOCKS_PER_SEC;
    cout << "Time taken by Algo 2: " << elapsed << " seconds." << endl;

    // Generate a random field using the user's rows and columns input
    cout << "\nGenerating a new random field..." << endl;
    auto field = generateField(numOfRow, numOfCol, 100);

    // Print the generated field
    for (const auto &row : field)
    {
        for (char c : row)
        {
            cout << c << ' ';
        }
        cout << '\n';
    }

    // Run and benchmarck algorithm 1 using the user generated case
    cout << "\nApplying the algorithms on the new field..." << endl;
    start = clock();
    cout << "Algo 1 return " << algo1(field) << endl;
    end = clock();
    elapsed = double(end - start) / CLOCKS_PER_SEC;
    cout << "Time taken by Algo 1: " << elapsed << " seconds." << endl;

    // Run and benchmarck algorithm 2 using the user generated case
    start = clock();
    cout << "Algo 2 return " << algo2(field) << endl;
    end = clock();
    elapsed = double(end - start) / CLOCKS_PER_SEC;
    cout << "Time taken by Algo 2: " << elapsed << " seconds." << endl;

    return 0;
}
