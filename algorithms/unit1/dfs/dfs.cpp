#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Location {
    string id;
    string name;
    string type;
};

class Graph {
private:
    int vertices;

public:
    vector<vector<int>> adjList;
    vector<Location> locations;

    Graph(int v) {
        vertices = v;
        adjList.resize(v);
        locations.resize(v);
    }

    void addLocation(int index, string id, string name, string type) {
        locations[index] = {id, name, type};
    }

    void addUndirectedEdge(int source, int destination) {
        adjList[source].push_back(destination);
        adjList[destination].push_back(source);
    }

    void DFS(int current, vector<bool>& visited, int& step) {

        visited[current] = true;

        step++;

        cout << step << ". "
             << locations[current].id << " - "
             << locations[current].name
             << " [" << locations[current].type << "]"
             << endl;

        for (int next : adjList[current]) {

            if (!visited[next]) {
                DFS(next, visited, step);
            }
        }
    }
};

int main() {

    Graph city(12);

    // ----------------------------------------
    // Adding City Locations
    // ----------------------------------------

    city.addLocation(0, "L1", "Central Junction", "JUNCTION");
    city.addLocation(1, "L2", "North Junction", "JUNCTION");
    city.addLocation(2, "L3", "East Junction", "JUNCTION");
    city.addLocation(3, "L4", "West Junction", "JUNCTION");
    city.addLocation(4, "L5", "South Junction", "JUNCTION");
    city.addLocation(5, "L6", "City Hospital", "HOSPITAL");
    city.addLocation(6, "L7", "Central Shelter", "SHELTER");
    city.addLocation(7, "L8", "Emergency Center", "EMERGENCY_CENTER");
    city.addLocation(8, "L9", "North Shelter", "SHELTER");
    city.addLocation(9, "L10", "Industrial Area", "AREA");
    city.addLocation(10, "L11", "School Ground", "SHELTER");
    city.addLocation(11, "L12", "Market Area", "AREA");

    // ----------------------------------------
    // Adding Roads
    // ----------------------------------------

    city.addUndirectedEdge(0, 1);   // L1 - L2
    city.addUndirectedEdge(0, 2);   // L1 - L3
    city.addUndirectedEdge(0, 3);   // L1 - L4
    city.addUndirectedEdge(0, 4);   // L1 - L5

    city.addUndirectedEdge(1, 11);  // L2 - L12
    city.addUndirectedEdge(1, 6);   // L2 - L7

    city.addUndirectedEdge(2, 9);   // L3 - L10
    city.addUndirectedEdge(2, 5);   // L3 - L6

    city.addUndirectedEdge(4, 7);   // L5 - L8

    city.addUndirectedEdge(3, 6);   // L4 - L7

    city.addUndirectedEdge(11, 8);  // L12 - L9
    city.addUndirectedEdge(11, 5);  // L12 - L6

    city.addUndirectedEdge(9, 10);  // L10 - L11
    city.addUndirectedEdge(5, 10);  // L6 - L11

    city.addUndirectedEdge(7, 9);   // L8 - L10

    // ----------------------------------------
    // DFS Starting from Industrial Area
    // ----------------------------------------

    int start = 9;  // L10 - Industrial Area

    vector<bool> visited(12, false);

    int step = 0;

    cout << "========================================" << endl;
    cout << "        DFS CITY NETWORK EXPLORATION" << endl;
    cout << "========================================" << endl;

    cout << "\nStarting Location:" << endl;
    cout << "L10 - Industrial Area" << endl;

    cout << "\nDFS Traversal:" << endl;
    cout << "----------------------------------------" << endl;

    city.DFS(start, visited, step);

    cout << "\n----------------------------------------" << endl;

    cout << "Total locations visited: " << step << endl;

    cout << "\nDFS completed successfully." << endl;

    return 0;
}