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
// Merge two sorted parts
// --------------------------------------------------

void merge(vector<Emergency>& emergencies,
           int left,
           int mid,
           int right) {

    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Emergency> leftPart(n1);
    vector<Emergency> rightPart(n2);

    // Copy data into temporary arrays
    for (int i = 0; i < n1; i++) {
        leftPart[i] = emergencies[left + i];
    }

    for (int j = 0; j < n2; j++) {
        rightPart[j] = emergencies[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    // Merge in descending order
    while (i < n1 && j < n2) {

        if (leftPart[i].affectedPeople >=
            rightPart[j].affectedPeople) {

            emergencies[k] = leftPart[i];
            i++;
        }
        else {

            emergencies[k] = rightPart[j];
            j++;
        }

        k++;
    }

    // Copy remaining left elements
    while (i < n1) {

        emergencies[k] = leftPart[i];

        i++;
        k++;
    }

    // Copy remaining right elements
    while (j < n2) {

        emergencies[k] = rightPart[j];

        j++;
        k++;
    }
}


// --------------------------------------------------
// Merge Sort
// --------------------------------------------------

void mergeSort(vector<Emergency>& emergencies,
               int left,
               int right) {

    // Base case
    if (left >= right) {
        return;
    }

    // Divide
    int mid = left + (right - left) / 2;

    // Conquer left half
    mergeSort(emergencies, left, mid);

    // Conquer right half
    mergeSort(emergencies, mid + 1, right);

    // Combine
    merge(emergencies, left, mid, right);
}


// --------------------------------------------------
// Display emergencies
// --------------------------------------------------

void displayEmergencies(const vector<Emergency>& emergencies) {

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


    cout << "========================================" << endl;
    cout << "       DISASTER EMERGENCY MERGE SORT" << endl;
    cout << "========================================" << endl;


    // Original data

    cout << "\nBefore Sorting:" << endl;
    cout << "----------------------------------------" << endl;

    displayEmergencies(emergencies);


    // Merge Sort

    mergeSort(emergencies, 0, emergencies.size() - 1);


    // Sorted data

    cout << "\nAfter Merge Sort:" << endl;
    cout << "----------------------------------------" << endl;

    cout << "Sorted by affected people "
         << "(Highest to Lowest):" << endl;

    displayEmergencies(emergencies);


    cout << "\n----------------------------------------" << endl;

    cout << "Highest priority emergency:" << endl;

    cout << emergencies[0].id
         << " - "
         << emergencies[0].type
         << " at "
         << emergencies[0].location
         << " -> "
         << emergencies[0].affectedPeople
         << " people affected"
         << endl;


    cout << "\nMerge Sort completed successfully."
         << endl;

    return 0;
}