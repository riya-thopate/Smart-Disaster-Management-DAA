#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Emergency {
    string id;
    string type;
    string location;
};


// --------------------------------------------------
// Find Majority Element using Boyer-Moore Algorithm
// --------------------------------------------------

string findMajorityType(const vector<Emergency>& emergencies) {

    string candidate = "";
    int count = 0;

    // Phase 1: Find a possible candidate
    for (const Emergency& emergency : emergencies) {

        if (count == 0) {
            candidate = emergency.type;
            count = 1;
        }
        else if (emergency.type == candidate) {
            count++;
        }
        else {
            count--;
        }
    }

    // Phase 2: Verify the candidate
    int frequency = 0;

    for (const Emergency& emergency : emergencies) {

        if (emergency.type == candidate) {
            frequency++;
        }
    }

    // Majority means more than n/2
    if (frequency > emergencies.size() / 2) {
        return candidate;
    }

    return "NONE";
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    vector<Emergency> emergencies = {

        {"E1", "FIRE", "Industrial Area"},

        {"E2", "FIRE", "Market Area"},

        {"E3", "FLOOD", "North Area"},

        {"E4", "FIRE", "South Junction"},

        {"E5", "ACCIDENT", "Central Junction"},

        {"E6", "FIRE", "West Junction"},

        {"E7", "FLOOD", "East Junction"}

    };


    cout << "========================================"
         << endl;

    cout << "      MAJORITY EMERGENCY ANALYSIS"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nEmergency Types:" << endl;

    cout << "----------------------------------------"
         << endl;


    for (const Emergency& emergency : emergencies) {

        cout << emergency.id
             << " | "
             << emergency.type
             << " | "
             << emergency.location
             << endl;
    }


    string majorityType =
        findMajorityType(emergencies);


    cout << "\n----------------------------------------"
         << endl;


    if (majorityType != "NONE") {

        cout << "Majority emergency type found:"
             << endl;

        cout << majorityType
             << endl;

        cout << "\nInterpretation:"
             << endl;

        cout << "More than half of the recorded"
             << endl;

        cout << "emergencies belong to the "
             << majorityType
             << " category."
             << endl;

    }
    else {

        cout << "No majority emergency type exists."
             << endl;
    }


    cout << "\n----------------------------------------"
         << endl;

    cout << "Majority Element algorithm completed successfully."
         << endl;


    return 0;
}