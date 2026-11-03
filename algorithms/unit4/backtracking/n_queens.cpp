#include <iostream>
#include <vector>

using namespace std;

const int N = 4;

bool isSafe(
    vector<vector<int>>& board,
    int row,
    int col
) {
    // Check column
    for (int i = 0; i < row; i++) {
        if (board[i][col] == 1) {
            return false;
        }
    }

    // Check upper-left diagonal
    for (
        int i = row - 1, j = col - 1;
        i >= 0 && j >= 0;
        i--, j--
    ) {
        if (board[i][j] == 1) {
            return false;
        }
    }

    // Check upper-right diagonal
    for (
        int i = row - 1, j = col + 1;
        i >= 0 && j < N;
        i--, j++
    ) {
        if (board[i][j] == 1) {
            return false;
        }
    }

    return true;
}

bool solveNQueens(
    vector<vector<int>>& board,
    int row
) {
    // All queens placed
    if (row == N) {
        return true;
    }

    // Try every column
    for (int col = 0; col < N; col++) {

        if (isSafe(board, row, col)) {

            board[row][col] = 1;

            // Recur for next row
            if (solveNQueens(board, row + 1)) {
                return true;
            }

            // Backtrack
            board[row][col] = 0;
        }
    }

    return false;
}

void displayBoard(
    vector<vector<int>>& board
) {
    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            if (board[i][j] == 1) {
                cout << " Q ";
            }
            else {
                cout << " . ";
            }
        }

        cout << endl;
    }
}

int main() {

    vector<vector<int>> board(
        N,
        vector<int>(N, 0)
    );

    cout << "========================================" << endl;
    cout << "             N-QUEENS PROBLEM" << endl;
    cout << "             BACKTRACKING" << endl;
    cout << "========================================" << endl;

    cout << "\nEmergency planning grid: "
         << N << " x " << N
         << endl;

    cout << "\nFinding a safe placement for "
         << N << " response units..."
         << endl;

    cout << "\n----------------------------------------" << endl;

    if (solveNQueens(board, 0)) {

        cout << "Valid placement found:" << endl;
        cout << "\n";

        displayBoard(board);

        cout << "\n----------------------------------------" << endl;

        cout << "Each response unit is placed so that"
             << endl;
        cout << "no two units share the same row,"
             << endl;
        cout << "column, or diagonal."
             << endl;
    }
    else {

        cout << "No valid placement exists."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "N-Queens Backtracking completed successfully."
         << endl;

    return 0;
}