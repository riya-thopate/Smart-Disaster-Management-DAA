#include <iostream>
#include "Graph.h"

using namespace std;

int main() {

    Graph city(6);

    city.addLocation(0, "Central Junction", "JUNCTION");
    city.addLocation(1, "North Junction", "JUNCTION");
    city.addLocation(2, "East Junction", "JUNCTION");
    city.addLocation(3, "West Junction", "JUNCTION");
    city.addLocation(4, "South Junction", "JUNCTION");
    city.addLocation(5, "Hospital", "HOSPITAL");

    city.addEdge(0, 1, 10, 5, 100);
    city.addEdge(0, 2, 10, 5, 120);
    city.addEdge(0, 3, 10, 5, 80);
    city.addEdge(0, 4, 10, 5, 100);
    city.addEdge(2, 5, 14, 7, 90);

    city.displayGraph();

    return 0;
}