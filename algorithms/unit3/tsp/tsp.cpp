#include <iostream>
#include <vector>
#include <climits>
#include <string>

using namespace std;

const int INF = 1000000;

vector<vector<int>> distMatrix;
vector<vector<int>> dp;
vector<vector<int>> parent;

int n;

// DP function
int tsp(int mask, int current) {

    // All locations have been visited
    if (mask == (1 << n) - 1) {
        return distMatrix[current][0];
    }

    // Already calculated
    if (dp[mask][current] != -1) {
        return dp[mask][current];
    }

    int answer = INF;

    for (int next = 0; next < n; next++) {

        // If next location is not visited
        if ((mask & (1 << next)) == 0) {

            int newCost =
                distMatrix[current][next]
                + tsp(mask | (1 << next), next);

            if (newCost < answer) {
                answer = newCost;
                parent[mask][current] = next;
            }
        }
    }

    return dp[mask][current] = answer;
}

void printRoute() {

    int mask = 1;      // Start from location 0
    int current = 0;

    cout << "\nOptimal Rescue Route:" << endl;
    cout << "----------------------------------------" << endl;

    cout << "1. Emergency Center";

    int step = 2;

    while (mask != (1 << n) - 1) {

        int next = parent[mask][current];

        if (next == -1)
            break;

        cout << " -> " << step << ". ";

        if (next == 1)
            cout << "Industrial Area";
        else if (next == 2)
            cout << "Market Area";
        else if (next == 3)
            cout << "City Hospital";

        mask |= (1 << next);
        current = next;
        step++;
    }

    cout << " -> Emergency Center" << endl;
}

int main() {

    vector<string> locations = {
        "Emergency Center",
        "Industrial Area",
        "Market Area",
        "City Hospital"
    };

    n = locations.size();

    // Shortest distances obtained from the city road network
    distMatrix = {

        {0,  14, 44, 34},
        {14, 0,  34, 20},
        {44, 34, 0,  14},
        {34, 20, 14, 0}
    };

    int totalStates = 1 << n;

    dp.assign(
        totalStates,
        vector<int>(n, -1)
    );

    parent.assign(
        totalStates,
        vector<int>(n, -1)
    );

    cout << "========================================" << endl;
    cout << "       TRAVELLING SALESMAN PROBLEM" << endl;
    cout << "       DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nRescue route locations:" << endl;

    for (int i = 0; i < n; i++) {

        cout << i + 1 << ". "
             << locations[i] << endl;
    }

    cout << "\nStarting Location:" << endl;
    cout << locations[0] << endl;

    cout << "\n----------------------------------------" << endl;

    int minimumDistance = tsp(1, 0);

    cout << "Minimum rescue route distance = "
         << minimumDistance << " km" << endl;

    printRoute();

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "The DP algorithm finds the minimum-distance"
         << endl;

    cout << "round trip that visits every selected"
         << endl;

    cout << "disaster location exactly once."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "TSP Dynamic Programming completed successfully."
         << endl;

    return 0;
}