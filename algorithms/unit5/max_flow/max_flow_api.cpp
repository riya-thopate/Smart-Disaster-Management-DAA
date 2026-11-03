
#include <bits/stdc++.h>
using namespace std;

struct Road {
    string id, source, destination, status;
    int capacity;
};

struct Edge {
    int to, reverseIndex;
    int capacity, residual;
};

struct ArcHandle {
    int from, edgeIndex;
    string roadId, source, destination;
    int capacity;
};

string trim(string s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
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

vector<Road> loadRoads(const string& path) {
    ifstream file(path);

    if (!file)
        throw runtime_error("Cannot open roads.csv");

    vector<Road> roads;
    string line;

    getline(file, line); // Skip CSV header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);
        if (f.size() < 7) continue;

        Road r;
        r.id = f[0];
        r.source = f[1];
        r.destination = f[2];
        r.capacity = stoi(f[5]);
        r.status = f[6];

        transform(r.status.begin(), r.status.end(),
                  r.status.begin(),
                  [](unsigned char c) {
                      return toupper(c);
                  });

        if (r.status == "OPEN" && r.capacity > 0)
            roads.push_back(r);
    }

    return roads;
}

void addDirectedEdge(
    vector<vector<Edge>>& graph,
    int u, int v, int capacity
) {
    int forwardIndex = (int)graph[u].size();
    int reverseIndex = (int)graph[v].size();

    graph[u].push_back({
        v, reverseIndex, capacity, capacity
    });

    graph[v].push_back({
        u, forwardIndex, 0, 0
    });
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 4) {
            cerr << "Usage: max_flow_api.exe "
                 << "roads.csv SOURCE_ID DESTINATION_ID\n";
            return 1;
        }

        string csvPath = argv[1];
        string sourceId = argv[2];
        string destinationId = argv[3];

        if (sourceId == destinationId)
            throw runtime_error(
                "Source and destination must be different"
            );

        vector<Road> roads = loadRoads(csvPath);

        map<string, int> nodeIndex;
        vector<string> nodeNames;

        auto getNode = [&](const string& id) -> int {
            auto it = nodeIndex.find(id);

            if (it != nodeIndex.end())
                return it->second;

            int index = (int)nodeNames.size();
            nodeIndex[id] = index;
            nodeNames.push_back(id);
            return index;
        };

        for (const Road& road : roads) {
            getNode(road.source);
            getNode(road.destination);
        }

        if (!nodeIndex.count(sourceId))
            throw runtime_error("Source location not found in open-road graph");

        if (!nodeIndex.count(destinationId))
            throw runtime_error(
                "Destination location not found in open-road graph"
            );

        int n = (int)nodeNames.size();

        vector<vector<Edge>> graph(n);
        vector<ArcHandle> originalArcs;

        // CSV roads are treated as bidirectional.
        // Each direction has the road's stated capacity.
        for (const Road& road : roads) {
            int u = nodeIndex.at(road.source);
            int v = nodeIndex.at(road.destination);

            int edgeIndex = (int)graph[u].size();

            addDirectedEdge(graph, u, v, road.capacity);

            originalArcs.push_back({
                u, edgeIndex, road.id,
                road.source, road.destination,
                road.capacity
            });

            edgeIndex = (int)graph[v].size();

            addDirectedEdge(graph, v, u, road.capacity);

            originalArcs.push_back({
                v, edgeIndex, road.id,
                road.destination, road.source,
                road.capacity
            });
        }

        int source = nodeIndex.at(sourceId);
        int sink = nodeIndex.at(destinationId);
        long long maxFlow = 0;

        // Edmonds-Karp: BFS finds an augmenting path.
        while (true) {
            vector<int> parentNode(n, -1);
            vector<int> parentEdge(n, -1);
            queue<int> q;

            q.push(source);
            parentNode[source] = source;

            while (!q.empty() && parentNode[sink] == -1) {
                int u = q.front();
                q.pop();

                for (int i = 0; i < (int)graph[u].size(); i++) {
                    const Edge& e = graph[u][i];

                    if (e.residual > 0 &&
                        parentNode[e.to] == -1) {
                        parentNode[e.to] = u;
                        parentEdge[e.to] = i;
                        q.push(e.to);
                    }
                }
            }

            if (parentNode[sink] == -1)
                break;

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

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":\"Edmonds-Karp\",";
        cout << "\"source\":" << jsonString(sourceId) << ",";
        cout << "\"destination\":"
             << jsonString(destinationId) << ",";
        cout << "\"open_roads\":" << roads.size() << ",";
        cout << "\"node_count\":" << n << ",";
        cout << "\"maximum_flow\":" << maxFlow << ",";
        cout << "\"flow_details\":[";

        bool first = true;

        for (const ArcHandle& a : originalArcs) {
            int used = a.capacity -
                       graph[a.from][a.edgeIndex].residual;

            if (used <= 0) continue;

            if (!first) cout << ",";
            first = false;

            cout << "{";
            cout << "\"road_id\":" << jsonString(a.roadId) << ",";
            cout << "\"from\":" << jsonString(a.source) << ",";
            cout << "\"to\":" << jsonString(a.destination) << ",";
            cout << "\"capacity\":" << a.capacity << ",";
            cout << "\"flow\":" << used;
            cout << "}";
        }

        cout << "],";
        cout << "\"time_complexity\":\"O(V * E^2)\",";
        cout << "\"space_complexity\":\"O(V + E)\"";
        cout << "}\n";

        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
