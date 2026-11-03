#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct DisasterZone {
    string name;
    string type;
    vector<DisasterZone> subZones;
};

// Recursive function to explore disaster zones
void exploreZone(const DisasterZone& zone, int level) {

    // Indentation according to hierarchy level
    for (int i = 0; i < level; i++) {
        cout << "   ";
    }

    cout << "-> " << zone.name
         << " [" << zone.type << "]" << endl;

    // Base case:
    // If there are no sub-zones, stop recursion
    if (zone.subZones.empty()) {
        return;
    }

    // Recursive case:
    // Explore every sub-zone
    for (const DisasterZone& subZone : zone.subZones) {
        exploreZone(subZone, level + 1);
    }
}

int main() {

    // ----------------------------------------
    // Create Disaster Zone Hierarchy
    // ----------------------------------------

    DisasterZone factoryZone{
        "Factory Zone",
        "HIGH RISK",
        {}
    };

    DisasterZone storageZone{
        "Storage Zone",
        "HIGH RISK",
        {}
    };

    DisasterZone industrialArea{
        "Industrial Area",
        "DISASTER AREA",
        {factoryZone, storageZone}
    };


    DisasterZone mainMarket{
        "Main Market",
        "CROWDED AREA",
        {}
    };

    DisasterZone warehouse{
        "Warehouse",
        "RESOURCE AREA",
        {}
    };

    DisasterZone marketArea{
        "Market Area",
        "FLOOD AREA",
        {mainMarket, warehouse}
    };


    DisasterZone roadBlock{
        "Road Block",
        "TRAFFIC OBSTRUCTION",
        {}
    };

    DisasterZone accidentZone{
        "Accident Zone",
        "EMERGENCY AREA",
        {}
    };

    DisasterZone southJunction{
        "South Junction",
        "ACCIDENT AREA",
        {roadBlock, accidentZone}
    };


    // ----------------------------------------
    // Root Disaster Area
    // ----------------------------------------

    DisasterZone disasterArea{
        "Disaster Management Zone",
        "ROOT",
        {industrialArea, marketArea, southJunction}
    };


    // ----------------------------------------
    // Display
    // ----------------------------------------

    cout << "========================================" << endl;
    cout << "     RECURSIVE DISASTER ZONE EXPLORATION" << endl;
    cout << "========================================" << endl;

    cout << "\nExploring disaster zones recursively:\n" << endl;

    exploreZone(disasterArea, 0);

    cout << "\n----------------------------------------" << endl;
    cout << "Recursive exploration completed." << endl;

    return 0;
}