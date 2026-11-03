#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <climits>

using namespace std;

class NetworkFlow {

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

    NetworkFlow(int vertices) {

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

    int maxFlow(
        int source,
        int sink,
        vector<vector<int>>& residual
    ) {
        residual = capacity;

        vector<int> parent(n);

        int maximumFlow = 0;

        while (bfs(
            source,
            sink,
            parent,
            residual
        )) {

            int pathFlow = INT_MAX;

            // Find bottleneck
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

    vector<bool> findReachable(
        int source,
        const vector<vector<int>>& residual
    ) {
        vector<bool> visited(n, false);

        queue<int> q;

        q.push(source);
        visited[source] = true;

        while (!q.empty()) {

            int current = q.front();
            q.pop();

            for (int next = 0; next < n; next++) {

                if (!visited[next] &&
                    residual[current][next] > 0) {

                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        return visited;
    }

    vector<vector<int>>& getCapacity() {
        return capacity;
    }
};

struct CutEdge {
    int source;
    int destination;
    int capacity;
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

    NetworkFlow network(n);

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
    cout << "             MINIMUM CUT" << endl;
    cout << "          MAX-FLOW MIN-CUT" << endl;
    cout << "            NETWORK FLOW" << endl;
    cout << "========================================" << endl;

    cout << "\nSource:" << endl;
    cout << locations[source] << endl;

    cout << "\nSink:" << endl;
    cout << locations[sink] << endl;

    cout << "\n----------------------------------------" << endl;

    vector<vector<int>> residual;

    int maximumFlow =
        network.maxFlow(
            source,
            sink,
            residual
        );

    cout << "Maximum Flow = "
         << maximumFlow
         << " people"
         << endl;

    // Find vertices reachable from source
    // in the final residual graph.
    vector<bool> reachable =
        network.findReachable(
            source,
            residual
        );

    vector<CutEdge> cutEdges;

    // An edge from reachable -> non-reachable
    // belongs to the minimum cut.
    vector<vector<int>>& capacity =
        network.getCapacity();

    for (int u = 0; u < n; u++) {

        for (int v = 0; v < n; v++) {

            if (capacity[u][v] > 0 &&
                reachable[u] &&
                !reachable[v]) {

                cutEdges.push_back({
                    u,
                    v,
                    capacity[u][v]
                });
            }
        }
    }

    int minCutCapacity = 0;

    cout << "\n----------------------------------------" << endl;

    cout << "Minimum Cut Edges:" << endl;

    for (const CutEdge& edge : cutEdges) {

        cout << locations[edge.source]
             << " -> "
             << locations[edge.destination]
             << " : "
             << edge.capacity
             << " capacity"
             << endl;

        minCutCapacity +=
            edge.capacity;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Minimum Cut Capacity = "
         << minCutCapacity
         << " people"
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Max-Flow / Min-Cut Verification:" << endl;

    if (maximumFlow == minCutCapacity) {

        cout << "Maximum Flow = Minimum Cut Capacity"
             << endl;

        cout << "The Max-Flow Min-Cut theorem is verified."
             << endl;
    }
    else {

        cout << "Values are different."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "The minimum cut identifies the smallest"
         << endl;

    cout << "total capacity of connections whose"
         << endl;

    cout << "removal separates the Emergency Center"
         << endl;

    cout << "from the final evacuation point."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Minimum Cut completed successfully."
         << endl;

    return 0;
}