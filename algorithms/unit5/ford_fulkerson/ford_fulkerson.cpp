#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <climits>

using namespace std;

class FordFulkerson {

private:

    int n;

    vector<vector<int>> capacity;

    bool bfs(
        int source,
        int sink,
        vector<int>& parent,
        vector<vector<int>>& residual
    ) {

        vector<bool> visited(n, false);

        queue<int> q;

        q.push(source);
        visited[source] = true;

        parent[source] = -1;

        while (!q.empty()) {

            int current = q.front();
            q.pop();

            for (int next = 0; next < n; next++) {

                if (!visited[next] &&
                    residual[current][next] > 0) {

                    parent[next] = current;

                    visited[next] = true;

                    q.push(next);

                    if (next == sink) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

public:

    FordFulkerson(
        int vertices
    ) {

        n = vertices;

        capacity.assign(
            n,
            vector<int>(n, 0)
        );
    }

    void addEdge(
        int source,
        int destination,
        int value
    ) {

        capacity[source][destination] =
            value;
    }

    int maxFlow(
        int source,
        int sink
    ) {

        vector<vector<int>> residual =
            capacity;

        vector<int> parent(n);

        int maximumFlow = 0;

        // Find augmenting paths
        while (bfs(
            source,
            sink,
            parent,
            residual
        )) {

            int pathFlow = INT_MAX;

            // Find minimum capacity
            // on the selected path
            for (
                int current = sink;
                current != source;
                current = parent[current]
            ) {

                int previous =
                    parent[current];

                pathFlow =
                    min(
                        pathFlow,
                        residual[previous][current]
                    );
            }

            // Update residual capacities
            for (
                int current = sink;
                current != source;
                current = parent[current]
            ) {

                int previous =
                    parent[current];

                residual[previous][current]
                    -= pathFlow;

                residual[current][previous]
                    += pathFlow;
            }

            maximumFlow += pathFlow;
        }

        return maximumFlow;
    }
};

int main() {

    vector<string> locations = {

        "Emergency Center",
        "Rescue Hub A",
        "Rescue Hub B",
        "Central Shelter",
        "School Ground",
        "Evacuation Complete"
    };

    int n = locations.size();

    int source = 0;
    int sink = 5;

    FordFulkerson network(n);

    // Emergency Center -> Rescue Hubs
    network.addEdge(0, 1, 100);
    network.addEdge(0, 2, 80);

    // Rescue Hubs -> Shelters
    network.addEdge(1, 3, 60);
    network.addEdge(1, 4, 40);

    network.addEdge(2, 3, 30);
    network.addEdge(2, 4, 50);

    // Shelters -> final evacuation point
    network.addEdge(3, 5, 70);
    network.addEdge(4, 5, 80);

    cout << "========================================" << endl;
    cout << "           FORD-FULKERSON" << endl;
    cout << "             NETWORK FLOW" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster evacuation network:" << endl;

    for (int i = 0; i < n; i++) {

        cout << i + 1
             << ". "
             << locations[i]
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Network capacities:" << endl;

    cout << "Emergency Center -> Rescue Hub A : 100"
         << endl;

    cout << "Emergency Center -> Rescue Hub B : 80"
         << endl;

    cout << "Rescue Hub A -> Central Shelter : 60"
         << endl;

    cout << "Rescue Hub A -> School Ground : 40"
         << endl;

    cout << "Rescue Hub B -> Central Shelter : 30"
         << endl;

    cout << "Rescue Hub B -> School Ground : 50"
         << endl;

    cout << "Central Shelter -> Evacuation Complete : 70"
         << endl;

    cout << "School Ground -> Evacuation Complete : 80"
         << endl;

    cout << "\n----------------------------------------" << endl;

    int result =
        network.maxFlow(
            source,
            sink
        );

    cout << "Maximum evacuation flow = "
         << result
         << " people"
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "Ford-Fulkerson finds the maximum amount"
         << endl;

    cout << "of evacuation flow that can move from"
         << endl;

    cout << "the Emergency Center to the final"
         << endl;

    cout << "evacuation point without exceeding"
         << endl;

    cout << "the capacity of any connection."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Ford-Fulkerson completed successfully."
         << endl;

    return 0;
}