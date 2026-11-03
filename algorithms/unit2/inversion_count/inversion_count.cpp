#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Emergency {
    string id;
    int affectedPeople;
};


// --------------------------------------------------
// Merge two sorted parts and count inversions
// --------------------------------------------------

long long mergeAndCount(
    vector<Emergency>& emergencies,
    int left,
    int middle,
    int right) {

    vector<Emergency> leftPart;
    vector<Emergency> rightPart;

    for (int i = left; i <= middle; i++) {
        leftPart.push_back(emergencies[i]);
    }

    for (int i = middle + 1; i <= right; i++) {
        rightPart.push_back(emergencies[i]);
    }

    int i = 0;
    int j = 0;
    int k = left;

    long long inversions = 0;


    while (i < (int)leftPart.size() &&
           j < (int)rightPart.size()) {

        // Ascending order
        if (leftPart[i].affectedPeople
            <= rightPart[j].affectedPeople) {

            emergencies[k] = leftPart[i];
            i++;
        }
        else {

            emergencies[k] = rightPart[j];
            j++;

            // Every remaining element in leftPart
            // forms an inversion
            inversions +=
                leftPart.size() - i;
        }

        k++;
    }


    while (i < (int)leftPart.size()) {

        emergencies[k] = leftPart[i];

        i++;
        k++;
    }


    while (j < (int)rightPart.size()) {

        emergencies[k] = rightPart[j];

        j++;
        k++;
    }


    return inversions;
}


// --------------------------------------------------
// Merge Sort + Inversion Count
// --------------------------------------------------

long long mergeSortAndCount(
    vector<Emergency>& emergencies,
    int left,
    int right) {

    if (left >= right) {
        return 0;
    }

    int middle =
        left + (right - left) / 2;


    long long inversions = 0;


    inversions +=
        mergeSortAndCount(
            emergencies,
            left,
            middle
        );


    inversions +=
        mergeSortAndCount(
            emergencies,
            middle + 1,
            right
        );


    inversions +=
        mergeAndCount(
            emergencies,
            left,
            middle,
            right
        );


    return inversions;
}


// --------------------------------------------------
// Display emergencies
// --------------------------------------------------

void displayEmergencies(
    const vector<Emergency>& emergencies) {

    for (const Emergency& emergency : emergencies) {

        cout << emergency.id
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

        {"E1", 150},
        {"E2", 50},
        {"E3", 250},
        {"E4", 100},
        {"E5", 200}
    };


    cout << "========================================"
         << endl;

    cout << "       DISASTER PRIORITY INVERSION"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nOriginal Emergency Priority List:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    displayEmergencies(emergencies);


    long long inversionCount =
        mergeSortAndCount(
            emergencies,
            0,
            emergencies.size() - 1
        );


    cout << "\n----------------------------------------"
         << endl;

    cout << "Sorted Emergency List:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    displayEmergencies(emergencies);


    cout << "\n----------------------------------------"
         << endl;

    cout << "Total Inversions: "
         << inversionCount
         << endl;


    cout << "\nInterpretation:"
         << endl;

    cout << "The inversion count represents the number"
         << endl;

    cout << "of emergency pairs that were out of order."
         << endl;


    cout << "\n----------------------------------------"
         << endl;

    cout << "Inversion Count algorithm completed successfully."
         << endl;


    return 0;
}