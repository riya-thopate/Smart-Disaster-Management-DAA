#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// DISASTER LOCATION
// ==========================================

struct Location {
    string id;
    string name;
    string type;
};

// ==========================================
// LINEAR SEARCH BY LOCATION ID
// ==========================================

int linearSearchById(const vector<Location>& locations, string targetId) {

    for (int i = 0; i < locations.size(); i++) {

        if (locations[i].id == targetId) {
            return i;
        }
    }

    return -1;
}

// ==========================================
// LINEAR SEARCH BY LOCATION NAME
// ==========================================

int linearSearchByName(const vector<Location>& locations,
                       string targetName) {

    for (int i = 0; i < locations.size(); i++) {

        if (locations[i].name == targetName) {
            return i;
        }
    }

    return -1;
}

// ==========================================
// DISPLAY LOCATION
// ==========================================

void displayLocation(const Location& location) {

    cout << "\nLocation Found!\n";

    cout << "ID   : " << location.id << endl;
    cout << "Name : " << location.name << endl;
    cout << "Type : " << location.type << endl;
}

// ==========================================
// MAIN
// ==========================================

int main() {

    // Our actual disaster-management locations

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
    // SEARCH BY ID
    // ==========================================

    string targetId = "L10";

    int result = linearSearchById(locations, targetId);

    cout << "========================================\n";
    cout << "       DISASTER LOCATION SEARCH\n";
    cout << "========================================\n";

    cout << "\nSearching for Location ID: "
         << targetId << endl;


    if (result != -1) {

        displayLocation(locations[result]);

    } else {

        cout << "\nLocation not found.\n";
    }


    // ==========================================
    // SEARCH BY NAME
    // ==========================================

    string targetName = "City Hospital";

    result = linearSearchByName(locations, targetName);

    cout << "\n----------------------------------------\n";

    cout << "\nSearching for Location Name: "
         << targetName << endl;


    if (result != -1) {

        displayLocation(locations[result]);

    } else {

        cout << "\nLocation not found.\n";
    }


    return 0;
}