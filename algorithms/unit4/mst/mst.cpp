#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Edge {
    int source;
    int destination;
    int distance;
};

class DisjointSet {

private:
    vector<int> parent;
    vector<int> rankValue;

public:

    DisjointSet(int n) {

        parent.resize(n);
        rankValue.assign(n, 0);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int node) {

        if (parent[node] == node) {
            return node;
        }

        return parent[node] =
            find(parent[node]);
    }

    bool unite(int a, int b) {

        int rootA = find(a);
        int rootB = find(b);

        if (rootA == rootB) {
            return false;
        }

        if (rankValue[rootA] <
            rankValue[rootB]) {

            parent[rootA] = rootB;
        }
        else if (rankValue[rootA] >
                 rankValue[rootB]) {

            parent[rootB] = rootA;
        }
        else {

            parent[rootB] = rootA;
            rankValue[rootA]++;
        }

        return true;
    }
};

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

    vector<Edge> edges = {

        {0, 1, 14},
        {0, 2, 34},
        {1, 2, 20},
        {1, 3, 34},
        {2, 3, 14},
        {3, 4, 24},
        {2, 4, 24},
        {1, 5, 30}
    };

    // Sort roads by distance
    sort(
        edges.begin(),
        edges.end(),
        [](const Edge& a, const Edge& b) {
            return a.distance < b.distance;
        }
    );

    DisjointSet ds(n);

    vector<Edge> mstEdges;

    int totalDistance = 0;

    // Kruskal's Algorithm
    for (const Edge& edge : edges) {

        if (ds.unite(
                edge.source,
                edge.destination)) {

            mstEdges.push_back(edge);

            totalDistance += edge.distance;
        }

        // MST needs n - 1 edges
        if (mstEdges.size() ==
            n - 1) {

            break;
        }
    }

    cout << "========================================" << endl;
    cout << "        MINIMUM SPANNING TREE" << endl;
    cout << "          KRUSKAL'S ALGORITHM" << endl;
    cout << "             GREEDY METHOD" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-management locations:" << endl;

    for (int i = 0; i < n; i++) {

        cout << i + 1
             << ". "
             << locations[i]
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Selected roads for MST:" << endl;

    for (const Edge& edge : mstEdges) {

        cout << locations[edge.source]
             << " -> "
             << locations[edge.destination]
             << " : "
             << edge.distance
             << " km"
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Total MST distance = "
         << totalDistance
         << " km"
         << endl;

    cout << "\nNumber of roads selected = "
         << mstEdges.size()
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "Kruskal's greedy algorithm selects the"
         << endl;

    cout << "minimum-distance roads required to connect"
         << endl;

    cout << "all disaster-management locations without"
         << endl;

    cout << "creating unnecessary cycles."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Minimum Spanning Tree completed successfully."
         << endl;

    return 0;
}