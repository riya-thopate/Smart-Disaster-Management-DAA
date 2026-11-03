
#include <bits/stdc++.h>
using namespace std;

struct Road {
    string id;
    string source;
    string destination;
    int distance;
    int capacity;
    string status;
};

struct DSU {
    vector<int> parent, rankValue;

    explicit DSU(int n) {
        parent.resize(n);
        rankValue.assign(n, 0);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) return false;

        if (rankValue[a] < rankValue[b])
            swap(a, b);

        parent[b] = a;

        if (rankValue[a] == rankValue[b])
            rankValue[a]++;

        return true;
    }
};

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string upper(string s) {
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) {
            return static_cast<char>(toupper(c));
        });
    return s;
}

vector<string> splitCSV(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);

    while (getline(ss, field, ','))
        fields.push_back(trim(field));

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

    getline(file, line); // Skip header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);
        if (f.size() < 7) continue;

        Road r;
        r.id = f[0];
        r.source = f[1];
        r.destination = f[2];
        r.distance = stoi(f[3]);
        r.capacity = stoi(f[5]);
        r.status = upper(f[6]);

        if (r.distance < 0)
            throw runtime_error("Road distance cannot be negative");

        if (r.status == "OPEN")
            roads.push_back(r);
    }

    return roads;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            cerr << "Usage: mst_api.exe roads.csv\n";
            return 1;
        }

        vector<Road> roads = loadRoads(argv[1]);

        if (roads.empty())
            throw runtime_error("No open roads available");

        // Build a unique list of graph vertices.
        map<string, int> nodeIndex;
        vector<string> nodeNames;

        auto getNode = [&](const string& id) -> int {
            auto it = nodeIndex.find(id);

            if (it != nodeIndex.end())
                return it->second;

            int index = static_cast<int>(nodeNames.size());
            nodeIndex[id] = index;
            nodeNames.push_back(id);
            return index;
        };

        for (const Road& r : roads) {
            getNode(r.source);
            getNode(r.destination);
        }

        // Kruskal: process roads from smallest distance to largest.
        sort(roads.begin(), roads.end(),
            [](const Road& a, const Road& b) {
                if (a.distance != b.distance)
                    return a.distance < b.distance;

                return a.id < b.id;
            });

        int n = static_cast<int>(nodeNames.size());
        DSU dsu(n);

        vector<Road> mst;
        long long totalDistance = 0;

        for (const Road& road : roads) {
            int u = nodeIndex.at(road.source);
            int v = nodeIndex.at(road.destination);

            if (dsu.unite(u, v)) {
                mst.push_back(road);
                totalDistance += road.distance;

                if (static_cast<int>(mst.size()) == n - 1)
                    break;
            }
        }

        if (static_cast<int>(mst.size()) != n - 1) {
            throw runtime_error(
                "The open-road graph is disconnected; "
                "a spanning tree does not exist"
            );
        }

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":\"Kruskal's MST\",";
        cout << "\"node_count\":" << n << ",";
        cout << "\"open_road_count\":" << roads.size() << ",";
        cout << "\"mst_edge_count\":" << mst.size() << ",";
        cout << "\"total_distance\":" << totalDistance << ",";
        cout << "\"edges\":[";

        for (size_t i = 0; i < mst.size(); i++) {
            if (i > 0) cout << ",";

            const Road& r = mst[i];

            cout << "{";
            cout << "\"road_id\":" << jsonString(r.id) << ",";
            cout << "\"source\":" << jsonString(r.source) << ",";
            cout << "\"destination\":"
                 << jsonString(r.destination) << ",";
            cout << "\"distance\":" << r.distance;
            cout << "}";
        }

        cout << "],";
        cout << "\"time_complexity\":\"O(E log E)\",";
        cout << "\"space_complexity\":\"O(V + E)\"";
        cout << "}\n";

        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
