#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Edge {
    int to;
};

vector<vector<Edge>> graph;
vector<bool> visited;
vector<int> traversal;

void dfs(int current) {

    visited[current] = true;

    traversal.push_back(current);

    for (const Edge& edge : graph[current]) {

        if (!visited[edge.to]) {
            dfs(edge.to);
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "{\"success\":false,\"message\":\"Source location ID is required\"}";
        return 1;
    }

    string sourceId = argv[1];

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

    graph.resize(n);
    visited.assign(n, false);

    auto addRoad = [&](int u, int v) {

        graph[u].push_back({v});
        graph[v].push_back({u});
    };

    addRoad(0, 1);
    addRoad(0, 2);
    addRoad(0, 3);
    addRoad(0, 4);

    addRoad(1, 11);
    addRoad(1, 6);

    addRoad(2, 9);
    addRoad(2, 5);

    addRoad(4, 7);
    addRoad(3, 6);

    addRoad(11, 8);
    addRoad(11, 5);

    addRoad(9, 10);
    addRoad(5, 10);

    addRoad(7, 9);

    int source = -1;

    for (int i = 0; i < n; i++) {

        if (ids[i] == sourceId) {
            source = i;
            break;
        }
    }

    if (source == -1) {
        cout << "{\"success\":false,\"message\":\"Invalid location ID\"}";
        return 1;
    }

    dfs(source);

    cout << "{";

    cout << "\"success\":true,";
    cout << "\"algorithm\":\"DFS\",";
    cout << "\"source\":\"" << sourceId << "\",";
    cout << "\"source_name\":\"" << names[source] << "\",";

    cout << "\"visited_count\":"
         << traversal.size()
         << ",";

    cout << "\"traversal\":[";

    for (int i = 0; i < traversal.size(); i++) {

        cout << "\""
             << ids[traversal[i]]
             << "\"";

        if (i != traversal.size() - 1) {
            cout << ",";
        }
    }

    cout << "],";

    cout << "\"path_names\":[";

    for (int i = 0; i < traversal.size(); i++) {

        cout << "\""
             << names[traversal[i]]
             << "\"";

        if (i != traversal.size() - 1) {
            cout << ",";
        }
    }

    cout << "],";

    cout << "\"time_complexity\":\"O(V + E)\",";
    cout << "\"space_complexity\":\"O(V)\"";

    cout << "}";

    return 0;
}