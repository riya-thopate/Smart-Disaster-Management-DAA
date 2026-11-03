#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

struct Edge {
    int u;
    int v;
};

bool satisfiesConstraints(
    const vector<int>& x,
    const vector<Edge>& edges
) {
    // For every edge (u,v):
    //
    // x[u] + x[v] >= 1
    //
    // Therefore, at least one endpoint
    // must be selected.

    for (const Edge& edge : edges) {

        if (x[edge.u] +
            x[edge.v] < 1) {

            return false;
        }
    }

    return true;
}

int main() {

    vector<string> locations = {

        "Emergency Center",
        "Industrial Area",
        "City Hospital",
        "Market Area",
        "Central Shelter"
    };

    int n = locations.size();

    // Disaster-management road network
    vector<Edge> edges = {

        {0, 1},
        {0, 2},
        {1, 2},
        {1, 3},
        {2, 3},
        {2, 4},
        {3, 4}
    };

    int bestObjective = INT_MAX;

    vector<int> bestSolution(
        n,
        0
    );

    // Enumerate all binary assignments.
    //
    // For n = 5:
    // total possibilities = 2^5 = 32

    int totalAssignments = 1 << n;

    for (int mask = 0;
         mask < totalAssignments;
         mask++) {

        vector<int> x(n, 0);

        int objective = 0;

        for (int i = 0; i < n; i++) {

            if (mask & (1 << i)) {

                x[i] = 1;
                objective++;
            }
        }

        // Check ILP constraints
        if (satisfiesConstraints(
                x,
                edges
            )) {

            // Minimize number of selected locations
            if (objective < bestObjective) {

                bestObjective = objective;

                bestSolution = x;
            }
        }
    }

    cout << "========================================" << endl;
    cout << "       INTEGER LINEAR PROGRAMMING" << endl;
    cout << "          ILP / 0-1 ILP MODEL" << endl;
    cout << "========================================" << endl;

    cout << "\nProblem:" << endl;

    cout << "Minimum Vertex Cover using ILP"
         << endl;

    cout << "\nDecision Variables:" << endl;

    for (int i = 0; i < n; i++) {

        cout << "x"
             << i + 1
             << " = 1 if "
             << locations[i]
             << " is selected"
             << endl;

        cout << "x"
             << i + 1
             << " = 0 otherwise"
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Objective Function:" << endl;

    cout << "Minimize ";

    for (int i = 0; i < n; i++) {

        cout << "x"
             << i + 1;

        if (i != n - 1) {
            cout << " + ";
        }
    }

    cout << endl;

    cout << "\nConstraints:" << endl;

    int constraintNumber = 1;

    for (const Edge& edge : edges) {

        cout << "C"
             << constraintNumber
             << ": x"
             << edge.u + 1
             << " + x"
             << edge.v + 1
             << " >= 1"
             << endl;

        constraintNumber++;
    }

    cout << "\nBinary constraints:" << endl;

    for (int i = 0; i < n; i++) {

        cout << "x"
             << i + 1
             << " ∈ {0,1}"
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Optimal ILP Solution:" << endl;

    for (int i = 0; i < n; i++) {

        cout << "x"
             << i + 1
             << " = "
             << bestSolution[i]
             << endl;
    }

    cout << "\nMinimum objective value = "
         << bestObjective
         << endl;

    cout << "\nSelected locations:" << endl;

    for (int i = 0; i < n; i++) {

        if (bestSolution[i] == 1) {

            cout << "- "
                 << locations[i]
                 << endl;
        }
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Verification:" << endl;

    if (satisfiesConstraints(
            bestSolution,
            edges
        )) {

        cout << "All ILP constraints are satisfied."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "ILP represents the Vertex Cover problem"
         << endl;

    cout << "using binary decision variables and"
         << endl;

    cout << "linear constraints."
         << endl;

    cout << "\nThe objective is to minimize the number"
         << endl;

    cout << "of selected disaster-management locations"
         << endl;

    cout << "while ensuring every road is covered."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "ILP demonstration completed successfully."
         << endl;

    return 0;
}