#include <iostream>
#include <vector>
#include <climits>
#include <string>

using namespace std;

int main() {

    // Disaster-management locations
    vector<string> locations = {
        "Hospital",
        "Shelter",
        "Emergency Center",
        "Industrial Area"
    };

    // Search frequencies
    // Higher frequency = location searched more often
    vector<int> frequency = {
        40, 30, 20, 10
    };

    int n = locations.size();

    // dp[i][j] = minimum search cost for locations i to j
    vector<vector<int>> dp(
        n,
        vector<int>(n, 0)
    );

    // Prefix sums for frequency ranges
    vector<int> prefix(n + 1, 0);

    for (int i = 0; i < n; i++) {
        prefix[i + 1] =
            prefix[i] + frequency[i];
    }

    // One location has its own frequency as cost
    for (int i = 0; i < n; i++) {
        dp[i][i] = frequency[i];
    }

    // Consider chains of increasing length
    for (int length = 2; length <= n; length++) {

        for (int i = 0;
             i <= n - length;
             i++) {

            int j = i + length - 1;

            dp[i][j] = INT_MAX;

            int frequencySum =
                prefix[j + 1] - prefix[i];

            // Try every location as root
            for (int root = i;
                 root <= j;
                 root++) {

                int leftCost = 0;
                int rightCost = 0;

                if (root > i) {
                    leftCost =
                        dp[i][root - 1];
                }

                if (root < j) {
                    rightCost =
                        dp[root + 1][j];
                }

                int cost =
                    leftCost
                    + rightCost
                    + frequencySum;

                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                }
            }
        }
    }

    cout << "========================================" << endl;
    cout << "             OPTIMAL BST" << endl;
    cout << "          DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-location search frequencies:" << endl;

    for (int i = 0; i < n; i++) {

        cout << locations[i]
             << " -> "
             << frequency[i]
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Minimum weighted search cost = "
         << dp[0][n - 1]
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "The DP algorithm finds an efficient" << endl;
    cout << "Binary Search Tree arrangement for" << endl;
    cout << "frequently searched disaster locations." << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Optimal BST Dynamic Programming completed successfully."
         << endl;

    return 0;
}