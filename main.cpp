#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <limits>
#include <cstdlib>

using namespace std;

const int ROWS = 5;
const int COLS = 10;
const int PAIRS = (ROWS * COLS) / 2;

void fillBoard(vector<vector<int>> &board);
void showAll(const vector<vector<int>> &board);
void showBoard(const vector<vector<int>> &board, const vector<vector<bool>> &revealed, int r1 = -1, int c1 = -1, int r2 = -1, int c2 = -1);
bool isFinished(const vector<vector<bool>> &revealed);
void clearScreen();
bool getInput(int &r, int &c, const vector<vector<bool>> &revealed, const string &prompt);

int main()
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rng(seed);

    vector<vector<int>> board(ROWS, vector<int>(COLS));
    vector<vector<bool>> revealed(ROWS, vector<bool>(COLS, false));
    fillBoard(board);

    int guesses = 0;

    clearScreen();
    cout << "Memorize the numbers for 10 seconds..." << endl;
    showAll(board);
    this_thread::sleep_for(chrono::seconds(10));
    clearScreen();

    while (!isFinished(revealed))
    {
        showBoard(board, revealed);

        int r1, c1, r2, c2;
        if (!getInput(r1, c1, revealed, "Enter first cell indices (row and column): "))
            continue;
        if (!getInput(r2, c2, revealed, "Enter second cell indices (row and column): "))
            continue;
        if (r1 == r2 && c1 == c2)
        {
            cout << "You must select two different cells. Try again." << endl;
            continue;
        }

        guesses++;
        clearScreen();
        showBoard(board, revealed, r1, c1, r2, c2);

        if (board[r1][c1] == board[r2][c2])
        {
            cout << "Great! You found a pair." << endl;
            revealed[r1][c1] = true;
            revealed[r2][c2] = true;
            this_thread::sleep_for(chrono::seconds(2));
        }
        else
        {
            cout << "Not a match! Wait for 5 seconds..." << endl;
            this_thread::sleep_for(chrono::seconds(5));
        }
        clearScreen();
    }

    cout << "Congratulations! You found all pairs." << endl;
    cout << "Your total guesses: " << guesses << endl;
    return 0;
}

void fillBoard(vector<vector<int>> &board)
{
    vector<int> uniqueNumbers;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 99);

    while (uniqueNumbers.size() < PAIRS)
    {
        int num = dis(gen);
        if (find(uniqueNumbers.begin(), uniqueNumbers.end(), num) == uniqueNumbers.end())
        {
            uniqueNumbers.push_back(num);
        }
    }

    vector<int> numbers;
    for (int num : uniqueNumbers)
    {
        numbers.push_back(num);
        numbers.push_back(num);
    }

    shuffle(numbers.begin(), numbers.end(), gen);

    int idx = 0;
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            board[i][j] = numbers[idx++];
}

void showAll(const vector<vector<int>> &board)
{
    cout << "    ";
    for (int j = 0; j < COLS; ++j)
        cout << j << "\t";
    cout << endl;
    for (int i = 0; i < ROWS; ++i)
    {
        cout << i << " | ";
        for (int j = 0; j < COLS; ++j)
            cout << board[i][j] << "\t";
        cout << endl;
    }
}

void showBoard(const vector<vector<int>> &board, const vector<vector<bool>> &revealed, int r1, int c1, int r2, int c2)
{
    cout << "    ";
    for (int j = 0; j < COLS; ++j)
        cout << j << "\t";
    cout << endl;
    for (int i = 0; i < ROWS; ++i)
    {
        cout << i << " | ";
        for (int j = 0; j < COLS; ++j)
        {
            if (revealed[i][j] || (i == r1 && j == c1) || (i == r2 && j == c2))
                cout << board[i][j] << "\t";
            else
                cout << "-1\t";
        }
        cout << endl;
    }
}

bool isFinished(const vector<vector<bool>> &revealed)
{
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            if (!revealed[i][j])
                return false;
    return true;
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
bool getInput(int &r, int &c, const vector<vector<bool>> &revealed, const string &prompt)
{
    cout << prompt;
    cin >> r >> c;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter two integers." << endl;
        return false;
    }
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS)
    {
        cout << "Indices out of range. Try again." << endl;
        return false;
    }
    if (revealed[r][c])
    {
        cout << "Cell already revealed. Try another one." << endl;
        return false;
    }
    return true;
}