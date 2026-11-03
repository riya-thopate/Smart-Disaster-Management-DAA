#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

// ==========================================
// LOCATION
// ==========================================

struct Location {

    string id;
    string name;
    string type;
};


// ==========================================
// DISPLAY BFS RESULT
// ==========================================

void displayLocation(const Location& location, int level) {

    cout << "Level " << level
         << " -> "
         << location.id
         << " - "
         << location.name
         << " ["
         << location.type
         << "]"
         << endl;
}


// ==========================================
// BFS ALGORITHM
// ==========================================

void BFS(const vector<Location>& locations,
         const vector<vector<int>>& graph,
         int start) {

    int n = locations.size();

    vector<bool> visited(n, false);

    queue<int> q;

    // Start from emergency location
    visited[start] = true;

    q.push(start);

    int currentLevel = 0;

    cout << "\n========================================\n";
    cout << "        BFS CITY NETWORK EXPLORATION\n";
    cout << "========================================\n";

    cout << "\nStarting Location:\n";

    cout << locations[start].id
         << " - "
         << locations[start].name
         << endl;


    cout << "\nReachable locations:\n";


    while (!q.empty()) {

        int nodesAtCurrentLevel = q.size();

        cout << "\n--- Level "
             << currentLevel
             << " ---\n";


        for (int i = 0; i < nodesAtCurrentLevel; i++) {

            int current = q.front();

            q.pop();


            cout << locations[current].id
                 << " - "
                 << locations[current].name
                 << " ["
                 << locations[current].type
                 << "]"
                 << endl;


            // Explore neighbouring locations

            for (int neighbour : graph[current]) {

                if (!visited[neighbour]) {

                    visited[neighbour] = true;

                    q.push(neighbour);
                }
            }
        }

        currentLevel++;
    }
}


// ==========================================
// MAIN
// ==========================================

int main() {

    // ==========================================
    // CITY LOCATIONS
    // ==========================================

    vector<Location> locations = {

        {"L1", "Central Junction", "JUNCTION"},
        {"L2", "North Junction", "JUNCTION"},
        {"L3", "East Junction", "JUNCTION"},
        {"L4", "West Junction", "JUNCTION"},
        {"L5", "South Junction", "JUNCTION"},

        {"L6", "City Hospital", "HOSPITAL"},
        {"L7", "Central Shelter", "SHELTER"},
        {"L8", "Emergency Center", "EMERGENCY_CENTER"},
        {"L9", "North Shelter", "SHELTER"},

        {"L10", "Industrial Area", "AREA"},
        {"L11", "School Ground", "SHELTER"},
        {"L12", "Market Area", "AREA"}
    };


    // ==========================================
    // CITY GRAPH
    // ==========================================

    int n = locations.size();

    vector<vector<int>> graph(n);


    // L1 - L2
    graph[0].push_back(1);
    graph[1].push_back(0);

    // L1 - L3
    graph[0].push_back(2);
    graph[2].push_back(0);

    // L1 - L4
    graph[0].push_back(3);
    graph[3].push_back(0);

    // L1 - L5
    graph[0].push_back(4);
    graph[4].push_back(0);

    // L2 - L12
    graph[1].push_back(11);
    graph[11].push_back(1);

    // L2 - L7
    graph[1].push_back(6);
    graph[6].push_back(1);

    // L3 - L10
    graph[2].push_back(9);
    graph[9].push_back(2);

    // L3 - L6
    graph[2].push_back(5);
    graph[5].push_back(2);

    // L5 - L8
    graph[4].push_back(7);
    graph[7].push_back(4);

    // L4 - L7
    graph[3].push_back(6);
    graph[6].push_back(3);

    // L12 - L9
    graph[11].push_back(8);
    graph[8].push_back(11);

    // L12 - L6
    graph[11].push_back(5);
    graph[5].push_back(11);

    // L10 - L11
    graph[9].push_back(10);
    graph[10].push_back(9);

    // L6 - L11
    graph[5].push_back(10);
    graph[10].push_back(5);

    // L8 - L10
    graph[7].push_back(9);
    graph[9].push_back(7);


    // ==========================================
    // EMERGENCY START LOCATION
    // ==========================================

    // L10 = Industrial Area
    int start = 9;


    // ==========================================
    // RUN BFS
    // ==========================================

    BFS(locations, graph, start);


    return 0;
}