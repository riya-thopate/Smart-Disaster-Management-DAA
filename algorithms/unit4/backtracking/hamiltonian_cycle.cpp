#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool isSafe(
    int vertex,
    int position,
    const vector<vector<int>>& graph,
    const vector<int>& path
) {
    // Vertex must be connected to previous vertex
    if (graph[path[position - 1]][vertex] == 0) {
        return false;
    }

    // Vertex must not already be present
    for (int i = 0; i < position; i++) {
        if (path[i] == vertex) {
            return false;
        }
    }

    return true;
}

bool solveHamiltonian(
    int position,
    int n,
    const vector<vector<int>>& graph,
    vector<int>& path
) {
    // All vertices included
    if (position == n) {
        // Check return to starting point
        return graph[path[n - 1]][path[0]] == 1;
    }

    // Try every possible vertex
    for (int vertex = 1; vertex < n; vertex++) {

        if (isSafe(
                vertex,
                position,
                graph,
                path)) {

            path[position] = vertex;

            if (solveHamiltonian(
                    position + 1,
                    n,
                    graph,
                    path)) {
                return true;
            }

            // Backtrack
            path[position] = -1;
        }
    }

    return false;
}

int main() {

    // Valid cycle from the disaster-management
    // city network:
    //
    // Central Junction
    // -> North Junction
    // -> Market Area
    // -> City Hospital
    // -> Central Junction

    vector<string> locations = {
        "Central Junction",
        "North Junction",
        "Market Area",
        "City Hospital"
    };

    int n = locations.size();

    vector<vector<int>> graph(
        n,
        vector<int>(n, 0)
    );

    auto addRoad =
        [&](int u, int v) {
            graph[u][v] = 1;
            graph[v][u] = 1;
        };

    // Road connections
    addRoad(0, 1); // Central -> North
    addRoad(1, 2); // North -> Market
    addRoad(2, 3); // Market -> Hospital
    addRoad(3, 0); // Hospital -> Central

    vector<int> path(n, -1);

    // Start from Central Junction
    path[0] = 0;

    cout << "========================================" << endl;
    cout << "         HAMILTONIAN CYCLE" << endl;
    cout << "             BACKTRACKING" << endl;
    cout << "========================================" << endl;

    cout << "\nSelected disaster-management locations:"
         << endl;

    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". "
             << locations[i]
             << endl;
    }

    cout << "\nStarting Location:" << endl;
    cout << locations[0]
         << endl;

    cout << "\n----------------------------------------" << endl;

    if (solveHamiltonian(
            1,
            n,
            graph,
            path)) {

        cout << "Hamiltonian Cycle Found:" << endl;
        cout << endl;

        for (int i = 0; i < n; i++) {
            cout << locations[path[i]]
                 << " -> ";
        }

        cout << locations[path[0]]
             << endl;

        cout << "\n----------------------------------------" << endl;

        cout << "Every selected location is visited"
             << endl;

        cout << "exactly once and the route returns"
             << endl;

        cout << "to the starting location."
             << endl;
    }
    else {

        cout << "No Hamiltonian Cycle exists."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Hamiltonian Cycle completed successfully."
         << endl;

    return 0;
}