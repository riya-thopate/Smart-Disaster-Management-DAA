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
// Partition Function
// --------------------------------------------------

int partition(vector<Emergency>& emergencies,
              int low,
              int high) {

    // Select last element as pivot
    int pivot = emergencies[high].affectedPeople;

    int i = low - 1;

    // Arrange elements greater than or equal
    // to pivot on the left side
    for (int j = low; j < high; j++) {

        if (emergencies[j].affectedPeople >= pivot) {

            i++;

            swap(emergencies[i], emergencies[j]);
        }
    }

    // Put pivot in its correct position
    swap(emergencies[i + 1], emergencies[high]);

    return i + 1;
}


// --------------------------------------------------
// Quick Sort
// --------------------------------------------------

void quickSort(vector<Emergency>& emergencies,
               int low,
               int high) {

    if (low < high) {

        // Partition the array
        int pivotIndex =
            partition(emergencies, low, high);

        // Sort left part
        quickSort(emergencies,
                  low,
                  pivotIndex - 1);

        // Sort right part
        quickSort(emergencies,
                  pivotIndex + 1,
                  high);
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

    vector<Emergency> emergencies = {

        {"E1", "FIRE", "Industrial Area", 150},

        {"E2", "FLOOD", "Market Area", 250},

        {"E3", "ACCIDENT", "South Junction", 50}

    };


    cout << "========================================"
         << endl;

    cout << "       DISASTER EMERGENCY QUICK SORT"
         << endl;

    cout << "========================================"
         << endl;


    // Display original data

    cout << "\nBefore Sorting:" << endl;

    cout << "----------------------------------------"
         << endl;

    displayEmergencies(emergencies);


    // Apply Quick Sort

    quickSort(
        emergencies,
        0,
        emergencies.size() - 1
    );


    // Display sorted data

    cout << "\nAfter Quick Sort:" << endl;

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


    cout << "\nQuick Sort completed successfully."
         << endl;

    return 0;
}