#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// EMERGENCY DATA
// ==========================================

struct Emergency {

    string id;
    string type;
    string location;

    int severity;
    int affectedPeople;
};

// ==========================================
// DISPLAY EMERGENCIES
// ==========================================

void displayEmergencies(const vector<Emergency>& emergencies) {

    cout << "\n";

    for (const auto& emergency : emergencies) {

        cout << emergency.id
             << " | "
             << emergency.type
             << " | "
             << emergency.location
             << " | Affected People: "
             << emergency.affectedPeople
             << " | Severity: "
             << emergency.severity
             << endl;
    }
}

// ==========================================
// MERGE FUNCTION
// ==========================================

void merge(vector<Emergency>& emergencies,
           int left,
           int middle,
           int right) {

    int n1 = middle - left + 1;
    int n2 = right - middle;

    vector<Emergency> leftArray(n1);
    vector<Emergency> rightArray(n2);

    // Copy left half
    for (int i = 0; i < n1; i++) {
        leftArray[i] = emergencies[left + i];
    }

    // Copy right half
    for (int j = 0; j < n2; j++) {
        rightArray[j] = emergencies[middle + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    // Merge in descending order
    while (i < n1 && j < n2) {

        if (leftArray[i].affectedPeople >=
            rightArray[j].affectedPeople) {

            emergencies[k] = leftArray[i];
            i++;

        } else {

            emergencies[k] = rightArray[j];
            j++;
        }

        k++;
    }

    // Remaining left elements
    while (i < n1) {

        emergencies[k] = leftArray[i];

        i++;
        k++;
    }

    // Remaining right elements
    while (j < n2) {

        emergencies[k] = rightArray[j];

        j++;
        k++;
    }
}

// ==========================================
// MERGE SORT
// ==========================================

void mergeSort(vector<Emergency>& emergencies,
               int left,
               int right) {

    if (left >= right) {
        return;
    }

    int middle = left + (right - left) / 2;

    // Sort left half
    mergeSort(emergencies, left, middle);

    // Sort right half
    mergeSort(emergencies, middle + 1, right);

    // Merge both halves
    merge(emergencies, left, middle, right);
}

// ==========================================
// MAIN
// ==========================================

int main() {

    vector<Emergency> emergencies = {

        {"E1", "FIRE", "Industrial Area", 3, 150},

        {"E2", "FLOOD", "Market Area", 2, 250},

        {"E3", "ACCIDENT", "South Junction", 2, 50}
    };

    cout << "========================================\n";
    cout << "      DISASTER EMERGENCY SORTING\n";
    cout << "========================================\n";

    cout << "\nBefore Sorting:";
    displayEmergencies(emergencies);

    // Apply Merge Sort
    mergeSort(emergencies, 0, emergencies.size() - 1);

    cout << "\n----------------------------------------\n";

    cout << "\nAfter Sorting by Affected People";
    cout << " (Highest to Lowest):";

    displayEmergencies(emergencies);

    cout << "\n----------------------------------------\n";

    cout << "\nMost affected emergency:\n";

    cout << emergencies[0].id
         << " - "
         << emergencies[0].type
         << " at "
         << emergencies[0].location
         << " -> "
         << emergencies[0].affectedPeople
         << " people affected\n";

    return 0;
}