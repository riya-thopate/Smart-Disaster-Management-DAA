#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Try to find an emergency for a team.
bool tryMatch(
    int team,
    vector<vector<int>>& graph,
    vector<bool>& visited,
    vector<int>& emergencyMatch
) {
    for (int emergency : graph[team]) {

        // Avoid visiting the same emergency
        // repeatedly in one search.
        if (visited[emergency]) {
            continue;
        }

        visited[emergency] = true;

        // Emergency is free OR the currently assigned
        // team can be moved to another emergency.
        if (emergencyMatch[emergency] == -1 ||
            tryMatch(
                emergencyMatch[emergency],
                graph,
                visited,
                emergencyMatch
            )) {

            emergencyMatch[emergency] = team;
            return true;
        }
    }

    return false;
}

int main() {

    vector<string> teams = {
        "T1 - Alpha Team",
        "T2 - Bravo Team",
        "T3 - Charlie Team",
        "T4 - Delta Team"
    };

    vector<string> emergencies = {
        "E1 - Fire",
        "E2 - Flood",
        "E3 - Accident"
    };

    int teamCount = teams.size();
    int emergencyCount = emergencies.size();

    // Bipartite graph:
    // teams -> emergencies
    vector<vector<int>> graph(teamCount);

    // Team compatibility
    //
    // T1 can handle Fire and Accident
    // T2 can handle Flood and Accident
    // T3 can handle Accident
    // T4 can handle Fire and Flood

    graph[0] = {0, 2};
    graph[1] = {1, 2};
    graph[2] = {2};
    graph[3] = {0, 1};

    // emergencyMatch[e] = team assigned to emergency e
    vector<int> emergencyMatch(
        emergencyCount,
        -1
    );

    int maximumMatching = 0;

    // Find a match for each team.
    for (int team = 0; team < teamCount; team++) {

        vector<bool> visited(
            emergencyCount,
            false
        );

        if (tryMatch(
                team,
                graph,
                visited,
                emergencyMatch
            )) {

            maximumMatching++;
        }
    }

    cout << "========================================" << endl;
    cout << "          BIPARTITE MATCHING" << endl;
    cout << "           RESCUE ASSIGNMENT" << endl;
    cout << "========================================" << endl;

    cout << "\nRescue Teams:" << endl;

    for (const string& team : teams) {
        cout << "- " << team << endl;
    }

    cout << "\nEmergencies:" << endl;

    for (const string& emergency : emergencies) {
        cout << "- " << emergency << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Possible assignments:" << endl;

    cout << "Alpha Team   -> Fire, Accident" << endl;
    cout << "Bravo Team   -> Flood, Accident" << endl;
    cout << "Charlie Team -> Accident" << endl;
    cout << "Delta Team   -> Fire, Flood" << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Final Rescue-Team Assignments:"
         << endl;

    for (int emergency = 0;
         emergency < emergencyCount;
         emergency++) {

        if (emergencyMatch[emergency] != -1) {

            cout << emergencies[emergency]
                 << " <- "
                 << teams[
                     emergencyMatch[emergency]
                 ]
                 << endl;
        }
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Maximum matching = "
         << maximumMatching
         << " assignments"
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "Bipartite matching assigns rescue teams"
         << endl;

    cout << "to compatible emergencies so that"
         << endl;

    cout << "each team and emergency participates"
         << endl;

    cout << "in at most one assignment."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Bipartite Matching completed successfully."
         << endl;

    return 0;
}