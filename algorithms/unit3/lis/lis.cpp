#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {

    // Emergency priority values over time
    vector<int> priority = {
        3, 10, 2, 1, 20, 4, 6, 7
    };

    int n = priority.size();

    // dp[i] = length of LIS ending at i
    vector<int> dp(n, 1);

    // Previous index for reconstructing LIS
    vector<int> parent(n, -1);

    int maxLength = 1;
    int lastIndex = 0;

    // Dynamic Programming
    for (int i = 1; i < n; i++) {

        for (int j = 0; j < i; j++) {

            if (priority[j] < priority[i] &&
                dp[j] + 1 > dp[i]) {

                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }

        if (dp[i] > maxLength) {
            maxLength = dp[i];
            lastIndex = i;
        }
    }

    // Reconstruct LIS
    vector<int> lisSequence;

    int current = lastIndex;

    while (current != -1) {
        lisSequence.push_back(priority[current]);
        current = parent[current];
    }

    reverse(
        lisSequence.begin(),
        lisSequence.end()
    );

    cout << "========================================" << endl;
    cout << "     LONGEST INCREASING SUBSEQUENCE" << endl;
    cout << "          DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nEmergency priority sequence:" << endl;

    for (int value : priority) {
        cout << value << " ";
    }

    cout << "\n\n----------------------------------------" << endl;

    cout << "DP Table:" << endl;

    for (int i = 0; i < n; i++) {

        cout << "Priority "
             << priority[i]
             << " -> LIS length = "
             << dp[i]
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Longest Increasing Subsequence:" << endl;

    for (int value : lisSequence) {
        cout << value << " ";
    }

    cout << "\n\nLIS Length = "
         << maxLength
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "The DP algorithm identifies the longest"
         << endl;

    cout << "increasing pattern in emergency priority"
         << endl;

    cout << "values for trend analysis." << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "LIS Dynamic Programming completed successfully."
         << endl;

    return 0;
}