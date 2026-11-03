#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <climits>

using namespace std;

class MaxFlow {

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

    MaxFlow(int vertices) {
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
        capacity[source][destination] = value;
    }

    int calculate(
        int source,
        int sink
    ) {
        vector<vector<int>> residual = capacity;
        vector<int> parent(n);

        int maximumFlow = 0;
        int pathNumber = 1;

        cout << "\nAugmenting Paths:" << endl;
        cout << "----------------------------------------" << endl;

        while (bfs(
            source,
            sink,
            parent,
            residual
        )) {

            int pathFlow = INT_MAX;

            // Find bottleneck capacity
            for (
                int current = sink;
                current != source;
                current = parent[current]
            ) {
                int previous = parent[current];

                pathFlow = min(
                    pathFlow,
                    residual[previous][current]
                );
            }

            cout << "Path "
                 << pathNumber
                 << " -> Flow added: "
                 << pathFlow
                 << endl;

            pathNumber++;

            // Update residual graph
            for (
                int current = sink;
                current != source;
                current = parent[current]
            ) {
                int previous = parent[current];

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

    MaxFlow network(n);

    // Emergency Center -> Rescue Hubs
    network.addEdge(0, 1, 100);
    network.addEdge(0, 2, 80);

    // Rescue Hubs -> Shelters
    network.addEdge(1, 3, 60);
    network.addEdge(1, 4, 40);

    network.addEdge(2, 3, 30);
    network.addEdge(2, 4, 50);

    // Shelters -> Final evacuation point
    network.addEdge(3, 5, 70);
    network.addEdge(4, 5, 80);

    cout << "========================================" << endl;
    cout << "              MAXIMUM FLOW" << endl;
    cout << "             EDMONDS-KARP" << endl;
    cout << "          NETWORK FLOW ALGORITHM" << endl;
    cout << "========================================" << endl;

    cout << "\nSource:" << endl;
    cout << locations[source] << endl;

    cout << "\nSink:" << endl;
    cout << locations[sink] << endl;

    cout << "\n----------------------------------------" << endl;

    int result =
        network.calculate(
            source,
            sink
        );

    cout << "\n----------------------------------------" << endl;

    cout << "Maximum evacuation flow = "
         << result
         << " people"
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "The maximum-flow algorithm determines"
         << endl;

    cout << "the largest number of people that can"
         << endl;

    cout << "be transported from the Emergency Center"
         << endl;

    cout << "to the final evacuation point while"
         << endl;

    cout << "respecting all road capacities."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Maximum Flow completed successfully."
         << endl;

    return 0;
}