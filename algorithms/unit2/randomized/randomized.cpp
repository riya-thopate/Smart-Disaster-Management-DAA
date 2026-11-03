#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Emergency {

    string id;
    string type;
    string location;
    int affectedPeople;
};


// --------------------------------------------------
// Partition
// --------------------------------------------------

int partition(
    vector<Emergency>& emergencies,
    int low,
    int high) {

    int pivot =
        emergencies[high].affectedPeople;

    int i = low - 1;

    for (int j = low; j < high; j++) {

        // Descending order
        if (emergencies[j].affectedPeople >= pivot) {

            i++;

            swap(
                emergencies[i],
                emergencies[j]
            );
        }
    }

    swap(
        emergencies[i + 1],
        emergencies[high]
    );

    return i + 1;
}


// --------------------------------------------------
// Randomized Partition
// --------------------------------------------------

int randomizedPartition(
    vector<Emergency>& emergencies,
    int low,
    int high) {

    // Generate random pivot index
    int randomIndex =
        low + rand() % (high - low + 1);


    // Move random pivot to the end
    swap(
        emergencies[randomIndex],
        emergencies[high]
    );


    return partition(
        emergencies,
        low,
        high
    );
}


// --------------------------------------------------
// Randomized Quick Sort
// --------------------------------------------------

void randomizedQuickSort(
    vector<Emergency>& emergencies,
    int low,
    int high) {

    if (low < high) {

        int pivotIndex =
            randomizedPartition(
                emergencies,
                low,
                high
            );


        randomizedQuickSort(
            emergencies,
            low,
            pivotIndex - 1
        );


        randomizedQuickSort(
            emergencies,
            pivotIndex + 1,
            high
        );
    }
}


// --------------------------------------------------
// Display Emergencies
// --------------------------------------------------

void displayEmergencies(
    const vector<Emergency>& emergencies) {

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
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    // Seed random number generator
    srand(time(0));


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

    cout << "       RANDOMIZED QUICK SORT"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nBefore Randomized Sorting:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    displayEmergencies(emergencies);


    randomizedQuickSort(
        emergencies,
        0,
        emergencies.size() - 1
    );


    cout << "\n----------------------------------------"
         << endl;

    cout << "After Randomized Quick Sort:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    cout << "Sorted by affected people "
         << "(Highest to Lowest):"
         << endl;

    displayEmergencies(emergencies);


    cout << "\n----------------------------------------"
         << endl;

    cout << "Highest priority emergency:"
         << endl;

    cout << emergencies[0].id
         << " - "
         << emergencies[0].type
         << " at "
         << emergencies[0].location
         << " -> "
         << emergencies[0].affectedPeople
         << " people affected"
         << endl;


    cout << "\n----------------------------------------"
         << endl;

    cout << "Randomized Quick Sort completed successfully."
         << endl;


    return 0;
}