#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool isClique(
    const vector<int>& subset,
    const vector<vector<int>>& graph
) {
    for (int i = 0; i < subset.size(); i++) {

        for (int j = i + 1;
             j < subset.size();
             j++) {

            if (graph[subset[i]][subset[j]] == 0) {
                return false;
            }
        }
    }

    return true;
}

bool findClique(
    int current,
    int n,
    int requiredSize,
    const vector<vector<int>>& graph,
    vector<int>& subset
) {
    // Required clique size reached
    if (subset.size() == requiredSize) {
        return true;
    }

    // No more vertices available
    if (current == n) {
        return false;
    }

    // Include current vertex
    subset.push_back(current);

    if (isClique(subset, graph)) {

        if (findClique(
                current + 1,
                n,
                requiredSize,
                graph,
                subset)) {

            return true;
        }
    }

    // Backtrack
    subset.pop_back();

    // Exclude current vertex
    if (findClique(
            current + 1,
            n,
            requiredSize,
            graph,
            subset)) {

        return true;
    }

    return false;
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
    addRoad(0, 1);
    addRoad(0, 2);
    addRoad(1, 2);

    addRoad(1, 3);
    addRoad(2, 3);

    addRoad(2, 4);
    addRoad(3, 4);

    int requiredSize = 3;

    vector<int> clique;

    cout << "========================================" << endl;
    cout << "              CLIQUE PROBLEM" << endl;
    cout << "          NP-COMPLETE DEMONSTRATION" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-management locations:" << endl;

    for (int i = 0; i < n; i++) {

        cout << i + 1
             << ". "
             << locations[i]
             << endl;
    }

    cout << "\nRequired clique size = "
         << requiredSize
         << endl;

    cout << "\n----------------------------------------" << endl;

    if (findClique(
            0,
            n,
            requiredSize,
            graph,
            clique)) {

        cout << "Clique found:" << endl;
        cout << endl;

        for (int vertex : clique) {

            cout << "- "
                 << locations[vertex]
                 << endl;
        }

        cout << "\n----------------------------------------" << endl;

        cout << "Verification:" << endl;

        cout << "Every pair of selected locations"
             << endl;

        cout << "has a direct road connection."
             << endl;
    }
    else {

        cout << "No clique of size "
             << requiredSize
             << " exists."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "The Clique problem determines whether"
         << endl;

    cout << "a graph contains a group of vertices"
         << endl;

    cout << "where every pair is directly connected."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Clique demonstration completed successfully."
         << endl;

    return 0;
}