
#include <bits/stdc++.h>
using namespace std;

struct Emergency {
    string id, locationId, type, severity;
    int affectedPeople;
    string resource, status;
};

struct Team {
    string id, name, type, locationId;
    int capacity;
    string status;
};

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string upper(string s) {
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return toupper(c); });
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
    string out = "\"";

    for (unsigned char c : s) {
        if (c == '"' || c == '\\') {
            out += '\\';
            out += c;
        } else if (c == '\n') {
            out += "\\n";
        } else if (c == '\r') {
            out += "\\r";
        } else if (c == '\t') {
            out += "\\t";
        } else {
            out += c;
        }
    }

    out += '"';
    return out;
}

vector<Emergency> loadEmergencies(const string& path) {
    ifstream file(path);

    if (!file)
        throw runtime_error("Cannot open emergencies.csv");

    vector<Emergency> emergencies;
    string line;

    getline(file, line); // Skip header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);
        if (f.size() < 7) continue;

        Emergency e;
        e.id = f[0];
        e.locationId = f[1];
        e.type = upper(f[2]);
        e.severity = f[3];
        e.affectedPeople = stoi(f[4]);
        e.resource = f[5];
        e.status = upper(f[6]);

        if (e.status == "ACTIVE")
            emergencies.push_back(e);
    }

    return emergencies;
}

vector<Team> loadTeams(const string& path) {
    ifstream file(path);

    if (!file)
        throw runtime_error("Cannot open rescue_teams.csv");

    vector<Team> teams;
    string line;

    getline(file, line); // Skip header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);
        if (f.size() < 6) continue;

        Team t;
        t.id = f[0];
        t.name = f[1];
        t.type = upper(f[2]);
        t.locationId = f[3];
        t.capacity = stoi(f[4]);
        t.status = upper(f[5]);

        teams.push_back(t);
    }

    return teams;
}

bool compatible(const Emergency& e, const Team& t) {
    if (t.status != "AVAILABLE") return false;

    string type = upper(e.type);

    if (type == "FIRE")
        return t.type == "FIRE" || t.type == "GENERAL";

    if (type == "FLOOD")
        return t.type == "FLOOD" || t.type == "GENERAL";

    if (type == "ACCIDENT")
        return t.type == "MEDICAL" || t.type == "GENERAL";

    return t.type == "GENERAL";
}

// DFS augmenting-path algorithm for bipartite matching.
bool augment(
    int emergencyIndex,
    const vector<vector<int>>& graph,
    vector<int>& teamOwner,
    vector<bool>& visited
) {
    for (int teamIndex : graph[emergencyIndex]) {
        if (visited[teamIndex]) continue;

        visited[teamIndex] = true;

        if (teamOwner[teamIndex] == -1 ||
            augment(teamOwner[teamIndex], graph,
                    teamOwner, visited)) {
            teamOwner[teamIndex] = emergencyIndex;
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            cerr << "Usage: rescue_matching_api.exe "
                 << "emergencies.csv rescue_teams.csv\n";
            return 1;
        }

        vector<Emergency> emergencies =
            loadEmergencies(argv[1]);

        vector<Team> teams = loadTeams(argv[2]);

        vector<int> teamOwner(teams.size(), -1);
        vector<vector<int>> graph(emergencies.size());

        int availableTeams = 0;

        for (const Team& t : teams) {
            if (t.status == "AVAILABLE")
                availableTeams++;
        }

        // Construct the bipartite graph.
        for (int e = 0; e < (int)emergencies.size(); e++) {
            for (int t = 0; t < (int)teams.size(); t++) {
                if (compatible(emergencies[e], teams[t])) {
                    graph[e].push_back(t);
                }
            }
        }

        // Find the maximum matching.
        int matched = 0;

        for (int e = 0; e < (int)emergencies.size(); e++) {
            vector<bool> visited(teams.size(), false);

            if (augment(e, graph, teamOwner, visited))
                matched++;
        }

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":"
             << jsonString("Bipartite Matching") << ",";
        cout << "\"emergency_count\":" << emergencies.size() << ",";
        cout << "\"available_teams\":" << availableTeams << ",";
        cout << "\"matched_count\":" << matched << ",";
        cout << "\"assignments\":[";

        bool first = true;

        for (int t = 0; t < (int)teams.size(); t++) {
            if (teamOwner[t] == -1) continue;

            const Emergency& e = emergencies[teamOwner[t]];
            const Team& team = teams[t];

            if (!first) cout << ",";
            first = false;

            cout << "{";
            cout << "\"emergency_id\":" << jsonString(e.id) << ",";
            cout << "\"emergency_type\":" << jsonString(e.type) << ",";
            cout << "\"affected_people\":" << e.affectedPeople << ",";
            cout << "\"team_id\":" << jsonString(team.id) << ",";
            cout << "\"team_name\":" << jsonString(team.name) << ",";
            cout << "\"team_type\":" << jsonString(team.type) << ",";
            cout << "\"team_capacity\":" << team.capacity << ",";
            cout << "\"team_location_id\":"
                 << jsonString(team.locationId);
            cout << "}";
        }

        cout << "],\"unmatched_emergencies\":[";

        first = true;

        vector<bool> isMatched(emergencies.size(), false);

        for (int owner : teamOwner) {
            if (owner != -1)
                isMatched[owner] = true;
        }

        for (int e = 0; e < (int)emergencies.size(); e++) {
            if (isMatched[e]) continue;

            if (!first) cout << ",";
            first = false;

            cout << "{";
            cout << "\"id\":" << jsonString(emergencies[e].id) << ",";
            cout << "\"type\":" << jsonString(emergencies[e].type);
            cout << "}";
        }

        cout << "],";
        cout << "\"time_complexity\":\"O(V * E)\",";
        cout << "\"space_complexity\":\"O(V + E)\"";
        cout << "}\n";

        return 0;
    }
    catch (const exception& e) {
        cerr << e.what() << '\n';
        return 1;
    }
}
