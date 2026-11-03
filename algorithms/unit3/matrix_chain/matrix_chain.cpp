#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int matrixChainMultiplication(vector<int>& dimensions) {

    int n = dimensions.size() - 1;

    vector<vector<int>> dp(
        n,
        vector<int>(n, 0)
    );

    // Chain length
    for (int length = 2; length <= n; length++) {

        for (int i = 0; i <= n - length; i++) {

            int j = i + length - 1;

            dp[i][j] = INT_MAX;

            // Try every possible split
            for (int k = i; k < j; k++) {

                int cost =
                    dp[i][k]
                    + dp[k + 1][j]
                    + dimensions[i]
                    * dimensions[k + 1]
                    * dimensions[j];

                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }

    return dp[0][n - 1];
}

int main() {

    // Four matrices:
    // A1 = 10 x 20
    // A2 = 20 x 30
    // A3 = 30 x 40
    // A4 = 40 x 30

    vector<int> dimensions = {
        10, 20, 30, 40, 30
    };

    cout << "========================================" << endl;
    cout << "       MATRIX CHAIN MULTIPLICATION" << endl;
    cout << "       DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-management matrices:" << endl;

    cout << "A1 = 10 x 20" << endl;
    cout << "A2 = 20 x 30" << endl;
    cout << "A3 = 30 x 40" << endl;
    cout << "A4 = 40 x 30" << endl;

    cout << "\n----------------------------------------" << endl;

    int result =
        matrixChainMultiplication(dimensions);

    cout << "Minimum scalar multiplications = "
         << result << endl;

    cout << "\nInterpretation:" << endl;

    cout << "The DP algorithm finds the most efficient" << endl;
    cout << "order to multiply the disaster-management" << endl;
    cout << "data matrices with minimum computation." << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Matrix Chain Multiplication completed successfully."
         << endl;

    return 0;
}