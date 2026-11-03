#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool isSafe(
    int vertex,
    int color,
    vector<vector<int>>& graph,
    vector<int>& colors
) {
    for (int neighbour = 0;
         neighbour < graph.size();
         neighbour++) {

        if (graph[vertex][neighbour] == 1 &&
            colors[neighbour] == color) {

            return false;
        }
    }

    return true;
}

bool graphColoring(
    int vertex,
    int totalVertices,
    int totalColors,
    vector<vector<int>>& graph,
    vector<int>& colors
) {
    // All vertices colored
    if (vertex == totalVertices) {
        return true;
    }

    // Try every color
    for (int color = 1;
         color <= totalColors;
         color++) {

        if (isSafe(
                vertex,
                color,
                graph,
                colors)) {

            colors[vertex] = color;

            // Color next vertex
            if (graphColoring(
                    vertex + 1,
                    totalVertices,
                    totalColors,
                    graph,
                    colors)) {

                return true;
            }

            // Backtrack
            colors[vertex] = 0;
        }
    }

    return false;
}

int main() {

    vector<string> locations = {
        "Emergency Center",
        "Industrial Area",
        "City Hospital",
        "Market Area",
        "Central Shelter",
        "School Ground"
    };

    int n = locations.size();

    // City road connections
    vector<vector<int>> graph(n, vector<int>(n, 0));

    auto addRoad =
        [&](int u, int v) {

            graph[u][v] = 1;
            graph[v][u] = 1;
        };

    addRoad(0, 1);
    addRoad(0, 2);
    addRoad(1, 2);
    addRoad(1, 3);
    addRoad(2, 3);
    addRoad(2, 4);
    addRoad(3, 4);
    addRoad(1, 5);

    // Try using 3 colors
    int totalColors = 3;

    vector<int> colors(
        n,
        0
    );

    cout << "========================================" << endl;
    cout << "          GRAPH COLORING" << endl;
    cout << "             BACKTRACKING" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-management locations:" << endl;

    for (int i = 0; i < n; i++) {

        cout << i + 1
             << ". "
             << locations[i]
             << endl;
    }

    cout << "\nNumber of available colors = "
         << totalColors
         << endl;

    cout << "\n----------------------------------------" << endl;

    if (graphColoring(
            0,
            n,
            totalColors,
            graph,
            colors)) {

        cout << "Valid coloring found:" << endl;
        cout << endl;

        for (int i = 0; i < n; i++) {

            cout << locations[i]
                 << " -> Color "
                 << colors[i]
                 << endl;
        }

        cout << "\n----------------------------------------" << endl;

        cout << "All directly connected locations"
             << endl;

        cout << "have different colors."
             << endl;
    }
    else {

        cout << "No valid coloring exists with "
             << totalColors
             << " colors."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Graph Coloring completed successfully."
         << endl;

    return 0;
}