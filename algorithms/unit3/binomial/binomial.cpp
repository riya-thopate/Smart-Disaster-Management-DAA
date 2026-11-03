#include <iostream>
#include <vector>

using namespace std;


// --------------------------------------------------
// Binomial Coefficient using Dynamic Programming
// C(n, k)
// --------------------------------------------------

long long binomialCoefficient(int n, int k) {

    vector<vector<long long>> dp(
        n + 1,
        vector<long long>(k + 1, 0)
    );


    // Build the DP table
    for (int i = 0; i <= n; i++) {

        // C(i, 0) = 1
        dp[i][0] = 1;

        // Calculate values up to k
        for (int j = 1; j <= min(i, k); j++) {

            // C(i, i) = 1
            if (j == i) {

                dp[i][j] = 1;
            }
            else {

                dp[i][j] =
                    dp[i - 1][j - 1]
                    + dp[i - 1][j];
            }
        }
    }


    return dp[n][k];
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    int n = 6;
    int k = 2;


    cout << "========================================"
         << endl;

    cout << "       BINOMIAL COEFFICIENT"
         << endl;

    cout << "       DYNAMIC PROGRAMMING"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nAvailable response teams: "
         << n
         << endl;

    cout << "Teams to select: "
         << k
         << endl;


    long long result =
        binomialCoefficient(n, k);


    cout << "\n----------------------------------------"
         << endl;

    cout << "C("
         << n
         << ", "
         << k
         << ") = "
         << result
         << endl;


    cout << "\nInterpretation:"
         << endl;

    cout << "There are "
         << result
         << " possible ways to select "
         << k
         << " teams from "
         << n
         << " available teams."
         << endl;


    cout << "\n----------------------------------------"
         << endl;

    cout << "Binomial Coefficient completed successfully."
         << endl;


    return 0;
}