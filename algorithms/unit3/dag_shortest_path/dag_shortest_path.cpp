#include <iostream>
#include <vector>
#include <stack>
#include <climits>
#include <string>

using namespace std;

struct Edge {
    int to;
    int weight;
};

void topologicalSort(
    int node,
    vector<vector<Edge>>& graph,
    vector<bool>& visited,
    stack<int>& order
) {
    visited[node] = true;

    for (const Edge& edge : graph[node]) {
        if (!visited[edge.to]) {
            topologicalSort(
                edge.to,
                graph,
                visited,
                order
            );
        }
    }

    order.push(node);
}

int main() {

    // Disaster-response DAG
    //
    // 0 Emergency Center
    // 1 Damage Assessment
    // 2 Resource Center
    // 3 Rescue Team
    // 4 Evacuation Point
    // 5 Shelter

    vector<string> locations = {
        "Emergency Center",
        "Damage Assessment",
        "Resource Center",
        "Rescue Team",
        "Evacuation Point",
        "Shelter"
    };

    int n = locations.size();

    vector<vector<Edge>> graph(n);

    // Directed acyclic graph
    graph[0].push_back({1, 5});
    graph[0].push_back({2, 4});

    graph[1].push_back({3, 3});
    graph[1].push_back({4, 6});

    graph[2].push_back({3, 2});
    graph[2].push_back({4, 5});

    graph[3].push_back({4, 2});
    graph[4].push_back({5, 3});

    vector<bool> visited(n, false);
    stack<int> order;

    // Topological sorting
    for (int i = 0; i < n; i++) {

        if (!visited[i]) {
            topologicalSort(
                i,
                graph,
                visited,
                order
            );
        }
    }

    vector<int> distance(
        n,
        INT_MAX
    );

    int source = 0;

    distance[source] = 0;

    // Process nodes in topological order
    while (!order.empty()) {

        int current = order.top();
        order.pop();

        if (distance[current] == INT_MAX) {
            continue;
        }

        for (const Edge& edge : graph[current]) {

            if (distance[edge.to] >
                distance[current] + edge.weight) {

                distance[edge.to] =
                    distance[current] + edge.weight;
            }
        }
    }

    cout << "========================================" << endl;
    cout << "          DAG SHORTEST PATH" << endl;
    cout << "          DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-response locations:" << endl;

    for (int i = 0; i < n; i++) {
        cout << i
             << " -> "
             << locations[i]
             << endl;
    }

    cout << "\nStarting Location:" << endl;
    cout << locations[source] << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Shortest distances from Emergency Center:"
         << endl;

    for (int i = 0; i < n; i++) {

        cout << locations[i]
             << " -> ";

        if (distance[i] == INT_MAX) {
            cout << "Unreachable";
        }
        else {
            cout << distance[i]
                 << " units";
        }

        cout << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Shortest distance to Shelter = "
         << distance[5]
         << " units"
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "The DAG shortest-path algorithm computes"
         << endl;

    cout << "the minimum-cost response path from the"
         << endl;

    cout << "Emergency Center to each response stage."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "DAG Shortest Path completed successfully."
         << endl;

    return 0;
}