#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {

    // Two disaster-response activity sequences
    string sequence1 =
        "ASSESS RESCUE EVACUATE SHELTER";

    string sequence2 =
        "ASSESS EVACUATE SHELTER REPORT";

    // Convert strings into words
    vector<string> A = {
        "ASSESS",
        "RESCUE",
        "EVACUATE",
        "SHELTER"
    };

    vector<string> B = {
        "ASSESS",
        "EVACUATE",
        "SHELTER",
        "REPORT"
    };

    int n = A.size();
    int m = B.size();

    // dp[i][j] stores LCS length
    // for first i elements of A
    // and first j elements of B
    vector<vector<int>> dp(
        n + 1,
        vector<int>(m + 1, 0)
    );

    // Dynamic Programming
    for (int i = 1; i <= n; i++) {

        for (int j = 1; j <= m; j++) {

            if (A[i - 1] == B[j - 1]) {

                dp[i][j] =
                    dp[i - 1][j - 1] + 1;
            }
            else {

                dp[i][j] =
                    max(
                        dp[i - 1][j],
                        dp[i][j - 1]
                    );
            }
        }
    }

    // Reconstruct LCS
    vector<string> lcs;

    int i = n;
    int j = m;

    while (i > 0 && j > 0) {

        if (A[i - 1] == B[j - 1]) {

            lcs.push_back(A[i - 1]);

            i--;
            j--;
        }
        else if (dp[i - 1][j] >
                 dp[i][j - 1]) {

            i--;
        }
        else {

            j--;
        }
    }

    reverse(
        lcs.begin(),
        lcs.end()
    );

    cout << "========================================" << endl;
    cout << "     LONGEST COMMON SUBSEQUENCE" << endl;
    cout << "          DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nResponse Sequence 1:" << endl;

    for (string activity : A) {
        cout << activity << " ";
    }

    cout << "\n\nResponse Sequence 2:" << endl;

    for (string activity : B) {
        cout << activity << " ";
    }

    cout << "\n\n----------------------------------------" << endl;

    cout << "Longest Common Subsequence:" << endl;

    for (string activity : lcs) {
        cout << activity << " ";
    }

    cout << "\n\nLCS Length = "
         << dp[n][m]
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "The DP algorithm identifies the longest"
         << endl;

    cout << "common sequence of disaster-response"
         << endl;

    cout << "activities appearing in both plans." << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "LCS Dynamic Programming completed successfully."
         << endl;

    return 0;
}