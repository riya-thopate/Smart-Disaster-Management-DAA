#include <iostream>
#include "Graph.h"

using namespace std;

int main() {

    Graph city;

    // ==========================================
    // ADD CITY LOCATIONS
    // ==========================================

    city.addLocation("L1", "Central Junction", "JUNCTION");
    city.addLocation("L2", "North Junction", "JUNCTION");
    city.addLocation("L3", "East Junction", "JUNCTION");
    city.addLocation("L4", "West Junction", "JUNCTION");
    city.addLocation("L5", "South Junction", "JUNCTION");

    city.addLocation("L6", "City Hospital", "HOSPITAL");
    city.addLocation("L7", "Central Shelter", "SHELTER");
    city.addLocation("L8", "Emergency Center", "EMERGENCY_CENTER");
    city.addLocation("L9", "North Shelter", "SHELTER");

    city.addLocation("L10", "Industrial Area", "AREA");
    city.addLocation("L11", "School Ground", "SHELTER");
    city.addLocation("L12", "Market Area", "AREA");


    // ==========================================
    // ADD CITY ROADS
    // ==========================================

    city.addUndirectedEdge("L1", "L2", 10, 5, 100);
    city.addUndirectedEdge("L1", "L3", 10, 5, 120);
    city.addUndirectedEdge("L1", "L4", 10, 5, 80);
    city.addUndirectedEdge("L1", "L5", 10, 5, 100);

    city.addUndirectedEdge("L2", "L12", 14, 7, 80);
    city.addUndirectedEdge("L2", "L7", 10, 5, 70);

    city.addUndirectedEdge("L3", "L10", 10, 5, 100);
    city.addUndirectedEdge("L3", "L6", 14, 7, 90);

    city.addUndirectedEdge("L5", "L8", 10, 5, 100);

    city.addUndirectedEdge("L4", "L7", 10, 5, 60);

    city.addUndirectedEdge("L12", "L9", 10, 5, 70);
    city.addUndirectedEdge("L12", "L6", 14, 7, 80);

    city.addUndirectedEdge("L10", "L11", 10, 5, 100);
    city.addUndirectedEdge("L6", "L11", 10, 5, 90);

    city.addUndirectedEdge("L8", "L10", 14, 7, 80);


    // ==========================================
    // DISPLAY CITY
    // ==========================================

    city.displayGraph();

    return 0;
}