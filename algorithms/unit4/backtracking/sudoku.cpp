#include <iostream>
#include <vector>

using namespace std;

const int N = 9;

void displayBoard(const vector<vector<int>>& board) {

    for (int row = 0; row < N; row++) {

        for (int col = 0; col < N; col++) {

            cout << board[row][col] << " ";

            if (col == 2 || col == 5) {
                cout << "| ";
            }
        }

        cout << endl;

        if (row == 2 || row == 5) {
            cout << "------+-------+------" << endl;
        }
    }
}

bool isSafe(
    const vector<vector<int>>& board,
    int row,
    int col,
    int number
) {
    // Check row
    for (int j = 0; j < N; j++) {

        if (board[row][j] == number) {
            return false;
        }
    }

    // Check column
    for (int i = 0; i < N; i++) {

        if (board[i][col] == number) {
            return false;
        }
    }

    // Check 3x3 box
    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 3; i++) {

        for (int j = 0; j < 3; j++) {

            if (board[startRow + i]
                     [startCol + j] == number) {

                return false;
            }
        }
    }

    return true;
}

bool solveSudoku(
    vector<vector<int>>& board
) {
    // Find an empty cell
    for (int row = 0; row < N; row++) {

        for (int col = 0; col < N; col++) {

            if (board[row][col] == 0) {

                // Try numbers 1 to 9
                for (int number = 1;
                     number <= 9;
                     number++) {

                    if (isSafe(
                            board,
                            row,
                            col,
                            number)) {

                        board[row][col] = number;

                        // Recursively solve remaining cells
                        if (solveSudoku(board)) {
                            return true;
                        }

                        // Backtrack
                        board[row][col] = 0;
                    }
                }

                // No valid number found
                return false;
            }
        }
    }

    // No empty cells remain
    return true;
}

int main() {

    // 0 represents an empty cell
    vector<vector<int>> board = {

        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},

        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},

        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    cout << "========================================" << endl;
    cout << "               SUDOKU" << endl;
    cout << "             BACKTRACKING" << endl;
    cout << "========================================" << endl;

    cout << "\nInitial Sudoku Board:" << endl;
    cout << "----------------------------------------" << endl;

    displayBoard(board);

    cout << "\n----------------------------------------" << endl;

    if (solveSudoku(board)) {

        cout << "Solved Sudoku Board:" << endl;
        cout << "----------------------------------------" << endl;

        displayBoard(board);

        cout << "\n----------------------------------------" << endl;

        cout << "Every row, column and 3x3 box"
             << endl;

        cout << "contains numbers 1 to 9 without repetition."
             << endl;
    }
    else {

        cout << "No solution exists for this Sudoku."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Sudoku Backtracking completed successfully."
         << endl;

    return 0;
}