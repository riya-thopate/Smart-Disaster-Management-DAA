
#include <bits/stdc++.h>
using namespace std;

struct Road {
    string id, source, destination, status;
    int capacity;
};

struct Edge {
    int to, reverseIndex, residual;
};

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

vector<string> splitCSV(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);

    while (getline(ss, field, ',')) {
        fields.push_back(trim(field));
    }

    return fields;
}

string jsonString(const string& s) {
    string result = "\"";

    for (unsigned char c : s) {
        if (c == '"' || c == '\\') {
            result += '\\';
            result += c;
        } else if (c == '\n') {
            result += "\\n";
        } else if (c == '\r') {
            result += "\\r";
        } else if (c == '\t') {
            result += "\\t";
        } else {
            result += c;
        }
    }

    result += '"';
    return result;
}

string upper(string s) {
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) {
            return static_cast<char>(toupper(c));
        });
    return s;
}

vector<Road> loadRoads(const string& path) {
    ifstream file(path);

    if (!file) {
        throw runtime_error("Cannot open roads.csv");
    }

    vector<Road> roads;
    string line;
    getline(file, line); // Skip the header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);
        if (f.size() < 7) continue;

        Road road;
        road.id = f[0];
        road.source = f[1];
        road.destination = f[2];
        road.capacity = stoi(f[5]);
        road.status = upper(f[6]);

        if (road.status == "OPEN" && road.capacity > 0) {
            roads.push_back(road);
        }
    }

    return roads;
}

void addDirectedEdge(
    vector<vector<Edge>>& graph,
    int u, int v, int capacity
) {
    int forwardIndex = (int)graph[u].size();
    int reverseIndex = (int)graph[v].size();

    graph[u].push_back({v, reverseIndex, capacity});
    graph[v].push_back({u, forwardIndex, 0});
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 4) {
            cerr << "Usage: min_cut_api.exe "
                 << "roads.csv SOURCE_ID DESTINATION_ID\n";
            return 1;
        }

        string sourceId = argv[2];
        string destinationId = argv[3];

        if (sourceId == destinationId) {
            throw runtime_error(
                "Source and destination must be different"
            );
        }

        vector<Road> roads = loadRoads(argv[1]);

        map<string, int> nodeIndex;
        vector<string> nodeNames;

        auto getNode = [&](const string& id) {
            auto it = nodeIndex.find(id);

            if (it != nodeIndex.end()) {
                return it->second;
            }

            int index = (int)nodeNames.size();
            nodeIndex[id] = index;
            nodeNames.push_back(id);
            return index;
        };

        for (const Road& road : roads) {
            getNode(road.source);
            getNode(road.destination);
        }

        if (!nodeIndex.count(sourceId)) {
            throw runtime_error("Source not found in open-road graph");
        }

        if (!nodeIndex.count(destinationId)) {
            throw runtime_error(
                "Destination not found in open-road graph"
            );
        }

        int n = (int)nodeNames.size();
        vector<vector<Edge>> graph(n);

        // Treat every listed road as bidirectional,
        // with its stated capacity in each direction.
        for (const Road& road : roads) {
            int u = nodeIndex.at(road.source);
            int v = nodeIndex.at(road.destination);

            addDirectedEdge(graph, u, v, road.capacity);
            addDirectedEdge(graph, v, u, road.capacity);
        }

        int source = nodeIndex.at(sourceId);
        int sink = nodeIndex.at(destinationId);
        long long maxFlow = 0;

        // Edmonds-Karp: repeatedly find augmenting paths using BFS.
        while (true) {
            vector<int> parentNode(n, -1);
            vector<int> parentEdge(n, -1);
            queue<int> q;

            q.push(source);
            parentNode[source] = source;

            while (!q.empty() && parentNode[sink] == -1) {
                int u = q.front();
                q.pop();

                for (int i = 0;
                     i < (int)graph[u].size(); i++) {
                    const Edge& edge = graph[u][i];

                    if (edge.residual > 0 &&
                        parentNode[edge.to] == -1) {
                        parentNode[edge.to] = u;
                        parentEdge[edge.to] = i;
                        q.push(edge.to);
                    }
                }
            }

            if (parentNode[sink] == -1) break;

            int pathFlow = INT_MAX;

            for (int v = sink; v != source;
                 v = parentNode[v]) {
                int u = parentNode[v];
                int ei = parentEdge[v];

                pathFlow = min(
                    pathFlow, graph[u][ei].residual
                );
            }

            for (int v = sink; v != source;
                 v = parentNode[v]) {
                int u = parentNode[v];
                int ei = parentEdge[v];
                int reverseIndex = graph[u][ei].reverseIndex;

                graph[u][ei].residual -= pathFlow;
                graph[v][reverseIndex].residual += pathFlow;
            }

            maxFlow += pathFlow;
        }

        // Find all vertices reachable from the source
        // through positive residual capacity.
        vector<bool> reachable(n, false);
        queue<int> q;

        q.push(source);
        reachable[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const Edge& edge : graph[u]) {
                if (edge.residual > 0 &&
                    !reachable[edge.to]) {
                    reachable[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }

        // Each CSV row represents one physical road.
        // Count a crossing road exactly once.
        long long cutCapacity = 0;

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":\"Edmonds-Karp Min Cut\",";
        cout << "\"source\":" << jsonString(sourceId) << ",";
        cout << "\"destination\":"
             << jsonString(destinationId) << ",";
        cout << "\"maximum_flow\":" << maxFlow << ",";
        cout << "\"source_side\":[";

        bool first = true;

        for (int i = 0; i < n; i++) {
            if (!reachable[i]) continue;

            if (!first) cout << ",";
            first = false;

            cout << jsonString(nodeNames[i]);
        }

        cout << "],\"sink_side\":[";
        first = true;

        for (int i = 0; i < n; i++) {
            if (reachable[i]) continue;

            if (!first) cout << ",";
            first = false;

            cout << jsonString(nodeNames[i]);
        }

        cout << "],\"cut_edges\":[";
        first = true;

        for (const Road& road : roads) {
            bool sourceReachable =
                reachable[nodeIndex.at(road.source)];

            bool destinationReachable =
                reachable[nodeIndex.at(road.destination)];

            if (sourceReachable == destinationReachable) {
                continue;
            }

            if (!first) cout << ",";
            first = false;

            cutCapacity += road.capacity;

            string from = sourceReachable
                ? road.source : road.destination;

            string to = sourceReachable
                ? road.destination : road.source;

            cout << "{";
            cout << "\"road_id\":" << jsonString(road.id) << ",";
            cout << "\"from\":" << jsonString(from) << ",";
            cout << "\"to\":" << jsonString(to) << ",";
            cout << "\"capacity\":" << road.capacity;
            cout << "}";
        }

        cout << "],";
        cout << "\"cut_capacity\":" << cutCapacity << ",";
        cout << "\"theorem_verified\":"
             << (cutCapacity == maxFlow ? "true" : "false") << ",";
        cout << "\"time_complexity\":\"O(V * E^2)\",";
        cout << "\"space_complexity\":\"O(V + E)\"";
        cout << "}\n";

        return cutCapacity == maxFlow ? 0 : 2;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
