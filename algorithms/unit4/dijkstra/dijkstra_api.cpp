#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

const int INF = 1000000000;

struct Edge {
    int to;
    int distance;
    int time;
};

struct Node {
    int distance;
    int vertex;

    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

int main(int argc, char* argv[]) {

    // Expected:
    // dijkstra_api.exe L10 L7

    if (argc != 3) {

        cout << "{";
        cout << "\"success\":false,";
        cout << "\"message\":\"Source and destination are required\"";
        cout << "}";
        return 1;
    }

    string sourceId = argv[1];
    string destinationId = argv[2];

    // 12 locations from locations.csv
    vector<string> ids = {
        "L1", "L2", "L3", "L4",
        "L5", "L6", "L7", "L8",
        "L9", "L10", "L11", "L12"
    };

    vector<string> names = {
        "Central Junction",
        "North Junction",
        "East Junction",
        "West Junction",
        "South Junction",
        "City Hospital",
        "Central Shelter",
        "Emergency Center",
        "North Shelter",
        "Industrial Area",
        "School Ground",
        "Market Area"
    };

    int n = ids.size();

    vector<vector<Edge>> graph(n);

    auto addRoad = [&](int u, int v, int distance, int time) {

        graph[u].push_back({
            v,
            distance,
            time
        });

        graph[v].push_back({
            u,
            distance,
            time
        });
    };

    // Roads from roads.csv

    addRoad(0, 1, 10, 5);   // L1-L2
    addRoad(0, 2, 10, 5);   // L1-L3
    addRoad(0, 3, 10, 5);   // L1-L4
    addRoad(0, 4, 10, 5);   // L1-L5

    addRoad(1, 11, 14, 7);  // L2-L12
    addRoad(1, 6, 10, 5);   // L2-L7

    addRoad(2, 9, 10, 5);   // L3-L10
    addRoad(2, 5, 14, 7);   // L3-L6

    addRoad(4, 7, 10, 5);   // L5-L8
    addRoad(3, 6, 10, 5);   // L4-L7

    addRoad(11, 8, 10, 5);  // L12-L9
    addRoad(11, 5, 14, 7);  // L12-L6

    addRoad(9, 10, 10, 5);  // L10-L11
    addRoad(5, 10, 10, 5);  // L6-L11

    addRoad(7, 9, 14, 7);   // L8-L10

    int source = -1;
    int destination = -1;

    for (int i = 0; i < n; i++) {

        if (ids[i] == sourceId) {
            source = i;
        }

        if (ids[i] == destinationId) {
            destination = i;
        }
    }

    if (source == -1 || destination == -1) {

        cout << "{";
        cout << "\"success\":false,";
        cout << "\"message\":\"Invalid location ID\"";
        cout << "}";

        return 1;
    }

    vector<int> distance(n, INF);
    vector<int> travelTime(n, INF);
    vector<int> parent(n, -1);

    priority_queue<
        Node,
        vector<Node>,
        greater<Node>
    > pq;

    distance[source] = 0;
    travelTime[source] = 0;

    pq.push({
        0,
        source
    });

    // Dijkstra
    while (!pq.empty()) {

        Node current = pq.top();
        pq.pop();

        int u = current.vertex;

        if (current.distance != distance[u]) {
            continue;
        }

        for (const Edge& edge : graph[u]) {

            int newDistance =
                distance[u] + edge.distance;

            if (newDistance < distance[edge.to]) {

                distance[edge.to] =
                    newDistance;

                travelTime[edge.to] =
                    travelTime[u] + edge.time;

                parent[edge.to] = u;

                pq.push({
                    newDistance,
                    edge.to
                });
            }
        }
    }

    // No route
    if (distance[destination] == INF) {

        cout << "{";
        cout << "\"success\":false,";
        cout << "\"message\":\"No route exists between the selected locations\"";
        cout << "}";

        return 0;
    }

    // Reconstruct path
    vector<int> path;

    int current = destination;

    while (current != -1) {

        path.push_back(current);
        current = parent[current];
    }

    reverse(
        path.begin(),
        path.end()
    );

    // JSON result
    cout << "{";

    cout << "\"success\":true,";

    cout << "\"algorithm\":\"Dijkstra\",";

    cout << "\"source\":\""
         << sourceId
         << "\",";

    cout << "\"source_name\":\""
         << names[source]
         << "\",";

    cout << "\"destination\":\""
         << destinationId
         << "\",";

    cout << "\"destination_name\":\""
         << names[destination]
         << "\",";

    cout << "\"distance\":"
         << distance[destination]
         << ",";

    cout << "\"estimated_time\":"
         << travelTime[destination]
         << ",";

    cout << "\"path\":[";

    for (int i = 0; i < path.size(); i++) {

        cout << "\""
             << ids[path[i]]
             << "\"";

        if (i != path.size() - 1) {
            cout << ",";
        }
    }

    cout << "],";

    cout << "\"path_names\":[";

    for (int i = 0; i < path.size(); i++) {

        cout << "\""
             << names[path[i]]
             << "\"";

        if (i != path.size() - 1) {
            cout << ",";
        }
    }

    cout << "],";

    cout << "\"time_complexity\":\"O((V + E) log V)\"";

    cout << "}";

    return 0;
}