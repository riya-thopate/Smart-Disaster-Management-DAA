#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int minimumCoins(vector<int>& coins, int amount) {

    vector<int> dp(amount + 1, INT_MAX);

    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {

        for (int coin : coins) {

            if (coin <= i && dp[i - coin] != INT_MAX) {

                dp[i] = min(
                    dp[i],
                    dp[i - coin] + 1
                );
            }
        }
    }

    return dp[amount];
}

int main() {

    // Resource package values
    vector<int> resources = {
        1, 5, 10, 25
    };

    int requiredAmount = 63;

    cout << "========================================" << endl;
    cout << "          COIN CHANGE PROBLEM" << endl;
    cout << "          DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nResource package values:" << endl;

    for (int value : resources) {
        cout << value << " ";
    }

    cout << "\n\nRequired resource value: "
         << requiredAmount << endl;

    cout << "\n----------------------------------------" << endl;

    int result =
        minimumCoins(resources, requiredAmount);

    if (result == INT_MAX) {

        cout << "Required amount cannot be formed."
             << endl;

    }
    else {

        cout << "Minimum number of resource packages = "
             << result << endl;

        cout << "\nInterpretation:" << endl;

        cout << "The DP algorithm finds the minimum number"
             << endl;

        cout << "of available resource packages required"
             << endl;

        cout << "to satisfy the emergency requirement."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Coin Change Dynamic Programming completed successfully."
         << endl;

    return 0;
}