#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

const int INF = 999999;

struct Edge {
    int destination;
    int distance;
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

    vector<vector<Edge>> graph(n);

    // Disaster-management city graph
    //
    // Emergency Center -> Industrial Area = 14
    // Emergency Center -> City Hospital   = 34
    // Industrial Area -> City Hospital    = 20
    // Industrial Area -> Market Area      = 34
    // City Hospital -> Market Area        = 14
    // Market Area -> Central Shelter      = 24
    // City Hospital -> Central Shelter    = 24
    // Industrial Area -> School Ground    = 30

    auto addUndirectedEdge =
        [&](int u, int v, int weight) {

            graph[u].push_back({v, weight});
            graph[v].push_back({u, weight});
        };

    addUndirectedEdge(0, 1, 14);
    addUndirectedEdge(0, 2, 34);
    addUndirectedEdge(1, 2, 20);
    addUndirectedEdge(1, 3, 34);
    addUndirectedEdge(2, 3, 14);
    addUndirectedEdge(3, 4, 24);
    addUndirectedEdge(2, 4, 24);
    addUndirectedEdge(1, 5, 30);

    int source = 0;

    vector<int> distance(
        n,
        INF
    );

    vector<bool> visited(
        n,
        false
    );

    distance[source] = 0;

    // Dijkstra's algorithm
    for (int count = 0; count < n; count++) {

        int current = -1;
        int minimumDistance = INF;

        // Find unvisited vertex with minimum distance
        for (int i = 0; i < n; i++) {

            if (!visited[i] &&
                distance[i] < minimumDistance) {

                minimumDistance = distance[i];
                current = i;
            }
        }

        if (current == -1) {
            break;
        }

        visited[current] = true;

        // Relax adjacent edges
        for (const Edge& edge : graph[current]) {

            int newDistance =
                distance[current]
                + edge.distance;

            if (newDistance <
                distance[edge.destination]) {

                distance[edge.destination] =
                    newDistance;
            }
        }
    }

    cout << "========================================" << endl;
    cout << "        DIJKSTRA'S SHORTEST PATH" << endl;
    cout << "             GREEDY ALGORITHM" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-management locations:" << endl;

    for (int i = 0; i < n; i++) {

        cout << i + 1
             << ". "
             << locations[i]
             << endl;
    }

    cout << "\nStarting Location:" << endl;
    cout << locations[source]
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Shortest distances from Emergency Center:"
         << endl;

    for (int i = 0; i < n; i++) {

        cout << locations[i]
             << " -> ";

        if (distance[i] == INF) {
            cout << "Unreachable";
        }
        else {
            cout << distance[i]
                 << " km";
        }

        cout << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Shortest distance to Central Shelter = "
         << distance[4]
         << " km"
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "Dijkstra's greedy algorithm finds the"
         << endl;

    cout << "shortest distance from the Emergency Center"
         << endl;

    cout << "to every reachable disaster-management location."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Dijkstra's Shortest Path completed successfully."
         << endl;

    return 0;
}