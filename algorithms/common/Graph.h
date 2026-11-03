#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Represents one road
struct Edge {
    int destination;
    int distance;
    int time;
    int capacity;
};

// Represents one city location
struct Location {
    string id;
    string name;
    string type;
};

class Graph {

private:
    int vertices;

    // Converts L1, L2, L3... into 0, 1, 2...
    unordered_map<string, int> idToIndex;

public:

    vector<vector<Edge>> adjList;
    vector<Location> locations;

    Graph() {
        vertices = 0;
    }

    // Add a city location
    void addLocation(string id, string name, string type) {

        if (idToIndex.find(id) != idToIndex.end()) {
            return;
        }

        int index = vertices++;

        idToIndex[id] = index;

        Location location;
        location.id = id;
        location.name = name;
        location.type = type;

        locations.push_back(location);
        adjList.push_back(vector<Edge>());
    }

    // Add road using location IDs such as L1 -> L2
    void addEdge(string source, string destination,
                 int distance, int time, int capacity) {

        if (idToIndex.find(source) == idToIndex.end() ||
            idToIndex.find(destination) == idToIndex.end()) {

            cout << "Invalid location ID!" << endl;
            return;
        }

        int sourceIndex = idToIndex[source];
        int destinationIndex = idToIndex[destination];

        Edge edge;

        edge.destination = destinationIndex;
        edge.distance = distance;
        edge.time = time;
        edge.capacity = capacity;

        adjList[sourceIndex].push_back(edge);
    }

    // Add a two-way road
    void addUndirectedEdge(string source, string destination,
                           int distance, int time, int capacity) {

        addEdge(source, destination, distance, time, capacity);

        addEdge(destination, source, distance, time, capacity);
    }

    // Get numeric index of location
    int getIndex(string id) {

        if (idToIndex.find(id) == idToIndex.end()) {
            return -1;
        }

        return idToIndex[id];
    }

    // Get location ID
    string getLocationId(int index) {
        return locations[index].id;
    }

    // Get location name
    string getLocationName(int index) {
        return locations[index].name;
    }

    // Display city graph
    void displayGraph() {

        cout << "\n============================================\n";
        cout << "       DISASTER MANAGEMENT CITY GRAPH\n";
        cout << "============================================\n";

        for (int i = 0; i < vertices; i++) {

            cout << "\n"
                 << locations[i].id
                 << " - "
                 << locations[i].name
                 << " [" << locations[i].type << "]";

            cout << "\n   Roads: ";

            if (adjList[i].empty()) {
                cout << "No outgoing roads";
            }

            for (auto edge : adjList[i]) {

                cout << "\n      -> "
                     << locations[edge.destination].id
                     << " - "
                     << locations[edge.destination].name
                     << " | Distance: "
                     << edge.distance
                     << " km"
                     << " | Time: "
                     << edge.time
                     << " min"
                     << " | Capacity: "
                     << edge.capacity;
            }

            cout << "\n";
        }

        cout << "\n============================================\n";
    }

    int getVertices() {
        return vertices;
    }
};

#endif