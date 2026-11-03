#include <iostream>
#include <vector>

using namespace std;


// --------------------------------------------------
// Fibonacci using Dynamic Programming
// Bottom-Up / Tabulation
// --------------------------------------------------

long long fibonacci(int n) {

    if (n <= 1) {
        return n;
    }

    vector<long long> dp(n + 1);

    dp[0] = 0;
    dp[1] = 1;


    for (int i = 2; i <= n; i++) {

        dp[i] = dp[i - 1] + dp[i - 2];
    }


    return dp[n];
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    int n = 10;


    cout << "========================================"
         << endl;

    cout << "       DYNAMIC PROGRAMMING"
         << endl;

    cout << "          FIBONACCI"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nNumber of disaster-response stages: "
         << n
         << endl;


    long long result =
        fibonacci(n);


    cout << "\n----------------------------------------"
         << endl;

    cout << "Fibonacci("
         << n
         << ") = "
         << result
         << endl;


    cout << "\nDP Table:"
         << endl;

    cout << "----------------------------------------"
         << endl;


    vector<long long> dp(n + 1);

    dp[0] = 0;
    dp[1] = 1;


    for (int i = 2; i <= n; i++) {

        dp[i] =
            dp[i - 1] + dp[i - 2];
    }


    for (int i = 0; i <= n; i++) {

        cout << "F[" << i << "] = "
             << dp[i]
             << endl;
    }


    cout << "\n----------------------------------------"
         << endl;

    cout << "Fibonacci Dynamic Programming completed successfully."
         << endl;


    return 0;
}