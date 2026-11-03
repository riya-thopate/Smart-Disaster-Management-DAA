#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Emergency {
    string id;
    string type;
    string location;
    int affectedPeople;
};


// --------------------------------------------------
// Partition for Quickselect
// Sort in descending order
// --------------------------------------------------

int partition(vector<Emergency>& emergencies,
              int low,
              int high) {

    int pivot = emergencies[high].affectedPeople;

    int i = low - 1;

    for (int j = low; j < high; j++) {

        // Larger values come first
        if (emergencies[j].affectedPeople >= pivot) {

            i++;

            swap(emergencies[i], emergencies[j]);
        }
    }

    swap(emergencies[i + 1], emergencies[high]);

    return i + 1;
}


// --------------------------------------------------
// Quickselect
// k = 1 means highest affected emergency
// --------------------------------------------------

Emergency quickSelect(vector<Emergency>& emergencies,
                      int low,
                      int high,
                      int k) {

    int pivotIndex =
        partition(emergencies, low, high);

    // Number of elements on the left including pivot
    int position = pivotIndex - low + 1;


    if (position == k) {

        return emergencies[pivotIndex];
    }


    if (k < position) {

        return quickSelect(
            emergencies,
            low,
            pivotIndex - 1,
            k
        );
    }


    return quickSelect(
        emergencies,
        pivotIndex + 1,
        high,
        k - position
    );
}


// --------------------------------------------------
// Display Emergency
// --------------------------------------------------

void displayEmergency(const Emergency& emergency) {

    cout << "Emergency ID   : "
         << emergency.id << endl;

    cout << "Type           : "
         << emergency.type << endl;

    cout << "Location       : "
         << emergency.location << endl;

    cout << "Affected People: "
         << emergency.affectedPeople << endl;
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    vector<Emergency> emergencies = {

        {"E1", "FIRE", "Industrial Area", 150},

        {"E2", "FLOOD", "Market Area", 250},

        {"E3", "ACCIDENT", "South Junction", 50},

        {"E4", "FIRE", "West Junction", 180},

        {"E5", "FLOOD", "North Area", 320},

        {"E6", "ACCIDENT", "Central Junction", 100},

        {"E7", "FIRE", "East Junction", 220}

    };


    cout << "========================================"
         << endl;

    cout << "       DISASTER ORDER STATISTICS"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nEmergency Data:"
         << endl;

    cout << "----------------------------------------"
         << endl;


    for (const Emergency& emergency : emergencies) {

        cout << emergency.id
             << " | "
             << emergency.type
             << " | "
             << emergency.location
             << " | Affected People: "
             << emergency.affectedPeople
             << endl;
    }


    // Find 3rd most affected emergency
    int k = 3;


    Emergency result =
        quickSelect(
            emergencies,
            0,
            emergencies.size() - 1,
            k
        );


    cout << "\n----------------------------------------"
         << endl;

    cout << k
         << "rd Most Affected Emergency:"
         << endl;

    cout << "----------------------------------------"
         << endl;


    displayEmergency(result);


    cout << "\n----------------------------------------"
         << endl;

    cout << "Order Statistics algorithm completed successfully."
         << endl;


    return 0;
}