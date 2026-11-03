#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Edge {
    int u;
    int v;
};

bool coversAllEdges(
    const vector<int>& selected,
    const vector<Edge>& edges
) {
    for (const Edge& edge : edges) {

        bool uSelected = false;
        bool vSelected = false;

        for (int vertex : selected) {

            if (vertex == edge.u) {
                uSelected = true;
            }

            if (vertex == edge.v) {
                vSelected = true;
            }
        }

        // Every edge must have at least one
        // selected endpoint.
        if (!uSelected && !vSelected) {
            return false;
        }
    }

    return true;
}

bool findVertexCover(
    int current,
    int n,
    int requiredSize,
    const vector<Edge>& edges,
    vector<int>& selected
) {
    // Required number of vertices selected
    if (selected.size() == requiredSize) {

        return coversAllEdges(
            selected,
            edges
        );
    }

    // Not enough vertices remaining
    if (current == n) {
        return false;
    }

    // Include current vertex
    selected.push_back(current);

    if (findVertexCover(
            current + 1,
            n,
            requiredSize,
            edges,
            selected
        )) {
        return true;
    }

    // Backtrack
    selected.pop_back();

    // Exclude current vertex
    if (findVertexCover(
            current + 1,
            n,
            requiredSize,
            edges,
            selected
        )) {
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

    // Road edges
    vector<Edge> edges = {
        {0, 1},
        {0, 2},
        {1, 2},
        {1, 3},
        {2, 3},
        {2, 4},
        {3, 4}
    };

    int requiredSize = 3;

    vector<int> selected;

    cout << "========================================" << endl;
    cout << "          VERTEX COVER PROBLEM" << endl;
    cout << "          NP-COMPLETE DEMONSTRATION" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-management locations:" << endl;

    for (int i = 0; i < n; i++) {

        cout << i + 1
             << ". "
             << locations[i]
             << endl;
    }

    cout << "\nRoad connections:" << endl;

    for (const Edge& edge : edges) {

        cout << locations[edge.u]
             << " <-> "
             << locations[edge.v]
             << endl;
    }

    cout << "\nRequired vertex cover size = "
         << requiredSize
         << endl;

    cout << "\n----------------------------------------" << endl;

    if (findVertexCover(
            0,
            n,
            requiredSize,
            edges,
            selected
        )) {

        cout << "Vertex Cover Found:" << endl;
        cout << endl;

        for (int vertex : selected) {

            cout << "- "
                 << locations[vertex]
                 << endl;
        }

        cout << "\n----------------------------------------" << endl;

        cout << "Verification:" << endl;

        if (coversAllEdges(
                selected,
                edges
            )) {

            cout << "Every road has at least one"
                 << endl;

            cout << "endpoint in the selected set."
                 << endl;
        }
    }
    else {

        cout << "No vertex cover of size "
             << requiredSize
             << " exists."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "The Vertex Cover problem determines"
         << endl;

    cout << "whether a graph contains a set of"
         << endl;

    cout << "vertices that covers every edge."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Vertex Cover demonstration completed successfully."
         << endl;

    return 0;
}