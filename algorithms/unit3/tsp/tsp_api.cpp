
#include <bits/stdc++.h>
using namespace std;

const long long INF = LLONG_MAX / 4;

struct Road {
    string source, destination, status;
    int distance;
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
    getline(file, line); // Skip header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);
        if (f.size() < 7) continue;

        Road r;
        r.source = f[1];
        r.destination = f[2];
        r.distance = stoi(f[3]);
        r.status = upper(f[6]);

        if (r.distance < 0)
            throw runtime_error("Road distances cannot be negative");

        if (r.status == "OPEN")
            roads.push_back(r);
    }

    return roads;
}

vector<string> loadActiveEmergencyLocations(
    const string& path
) {
    ifstream file(path);

    if (!file)
        throw runtime_error("Cannot open emergencies.csv");

    vector<string> locations;
    set<string> seen;
    string line;

    getline(file, line); // Skip header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);
        if (f.size() < 7) continue;

        if (upper(f[6]) != "ACTIVE") continue;

        string locationId = f[1];

        if (seen.insert(locationId).second)
            locations.push_back(locationId);
    }

    return locations;
}

map<string, string> loadLocationNames(
    const string& path
) {
    ifstream file(path);

    if (!file)
        throw runtime_error("Cannot open locations.csv");

    map<string, string> names;
    string line;

    getline(file, line); // Skip header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);
        if (f.size() < 2) continue;

        names[f[0]] = f[1];
    }

    return names;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 5) {
            cerr << "Usage: tsp_api.exe "
                 << "roads.csv emergencies.csv locations.csv START_ID\n";
            return 1;
        }

        vector<Road> roads = loadRoads(argv[1]);
        vector<string> emergencies =
            loadActiveEmergencyLocations(argv[2]);
        map<string, string> locationNames =
            loadLocationNames(argv[3]);

        string startId = argv[4];

        map<string, int> index;
        vector<string> nodeNames;

        auto getNode = [&](const string& id) -> int {
            auto it = index.find(id);

            if (it != index.end())
                return it->second;

            int i = static_cast<int>(nodeNames.size());
            index[id] = i;
            nodeNames.push_back(id);
            return i;
        };

        for (const Road& r : roads) {
            getNode(r.source);
            getNode(r.destination);
        }

        if (!index.count(startId))
            throw runtime_error("Start location not found in open-road graph");

        int V = static_cast<int>(nodeNames.size());

        // Build all-pairs shortest distances.
        vector<vector<long long>> dist(
            V, vector<long long>(V, INF)
        );

        for (int i = 0; i < V; i++)
            dist[i][i] = 0;

        // Each open road is treated as bidirectional.
        for (const Road& r : roads) {
            int u = index.at(r.source);
            int v = index.at(r.destination);

            dist[u][v] = min(dist[u][v], (long long)r.distance);
            dist[v][u] = min(dist[v][u], (long long)r.distance);
        }

        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                if (dist[i][k] == INF) continue;

                for (int j = 0; j < V; j++) {
                    if (dist[k][j] == INF) continue;

                    dist[i][j] = min(
                        dist[i][j],
                        dist[i][k] + dist[k][j]
                    );
                }
            }
        }

        int start = index.at(startId);

        // Keep only unique emergency locations other than the start.
        vector<string> stops;

        for (const string& id : emergencies) {
            if (id == startId) continue;

            if (!index.count(id))
                throw runtime_error(
                    "An active emergency location is not in the road graph: "
                    + id
                );

            stops.push_back(id);
        }

        // Held-Karp uses O(k * 2^k) memory.
        // Limit k to avoid excessive memory allocation.
        int k = static_cast<int>(stops.size());

        if (k > 20)
            throw runtime_error(
                "Too many emergency locations for this exact TSP solver "
                "(maximum supported: 20)"
            );

        long long bestDistance = 0;
        vector<string> bestRoute;

        if (k == 0) {
            bestRoute.push_back(startId);
        } else {
            int states = 1 << k;

            vector<vector<long long>> dp(
                states, vector<long long>(k, INF)
            );

            vector<vector<int>> parent(
                states, vector<int>(k, -1)
            );

            // Start at the Emergency Center, then visit one stop.
            for (int i = 0; i < k; i++) {
                long long d = dist[start][index.at(stops[i])];

                if (d != INF)
                    dp[1 << i][i] = d;
            }

            // Held-Karp Dynamic Programming.
            for (int mask = 1; mask < states; mask++) {
                for (int last = 0; last < k; last++) {
                    if (!(mask & (1 << last))) continue;
                    if (dp[mask][last] == INF) continue;

                    for (int next = 0; next < k; next++) {
                        if (mask & (1 << next)) continue;

                        long long leg =
                            dist[index.at(stops[last])]
                                [index.at(stops[next])];

                        if (leg == INF) continue;

                        int nextMask = mask | (1 << next);
                        long long candidate = dp[mask][last] + leg;

                        if (candidate < dp[nextMask][next]) {
                            dp[nextMask][next] = candidate;
                            parent[nextMask][next] = last;
                        }
                    }
                }
            }

            int fullMask = states - 1;
            int bestLast = -1;
            bestDistance = INF;

            // Return to the start after visiting every stop.
            for (int last = 0; last < k; last++) {
                long long back =
                    dist[index.at(stops[last])][start];

                if (dp[fullMask][last] == INF || back == INF)
                    continue;

                long long candidate = dp[fullMask][last] + back;

                if (candidate < bestDistance) {
                    bestDistance = candidate;
                    bestLast = last;
                }
            }

            if (bestLast == -1)
                throw runtime_error(
                    "No complete tour can visit all emergency locations"
                );

            // Reconstruct the optimal stop order.
            vector<int> reversedStops;
            int mask = fullMask;
            int current = bestLast;

            while (current != -1) {
                reversedStops.push_back(current);

                int previous = parent[mask][current];
                mask ^= (1 << current);
                current = previous;
            }

            reverse(reversedStops.begin(), reversedStops.end());

            bestRoute.push_back(startId);

            for (int stopIndex : reversedStops)
                bestRoute.push_back(stops[stopIndex]);

            bestRoute.push_back(startId);
        }

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":\"Held-Karp TSP\",";
        cout << "\"start\":" << jsonString(startId) << ",";
        cout << "\"start_name\":"
             << jsonString(
                    locationNames.count(startId)
                        ? locationNames.at(startId) : startId
                ) << ",";
        cout << "\"active_emergency_locations\":" << k << ",";
        cout << "\"total_distance\":" << bestDistance << ",";
        cout << "\"route\":[";

        for (size_t i = 0; i < bestRoute.size(); i++) {
            if (i > 0) cout << ",";
            cout << jsonString(bestRoute[i]);
        }

        cout << "],\"route_names\":[";

        for (size_t i = 0; i < bestRoute.size(); i++) {
            if (i > 0) cout << ",";

            string id = bestRoute[i];

            cout << jsonString(
                locationNames.count(id)
                    ? locationNames.at(id) : id
            );
        }

        cout << "],";
        cout << "\"time_complexity\":\"O(V^3 + k^2 * 2^k)\",";
        cout << "\"space_complexity\":\"O(V^2 + k * 2^k)\"";
        cout << "}\n";

        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
