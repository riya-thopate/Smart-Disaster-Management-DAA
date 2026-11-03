#include <iostream>
#include <vector>

using namespace std;


// --------------------------------------------------
// Josephus Problem
// --------------------------------------------------

int josephus(int n, int k) {

    // Base case
    if (n == 1) {
        return 0;
    }

    // Recursive formula
    return (josephus(n - 1, k) + k) % n;
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    int numberOfTeams = 7;
    int eliminationStep = 3;


    cout << "========================================"
         << endl;

    cout << "          JOSEPHUS PROBLEM"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nEmergency Response Teams:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    for (int i = 1; i <= numberOfTeams; i++) {

        cout << "Team " << i << endl;
    }


    cout << "\nElimination step: "
         << eliminationStep
         << endl;


    int survivor =
        josephus(
            numberOfTeams,
            eliminationStep
        );


    cout << "\n----------------------------------------"
         << endl;

    cout << "Surviving Team:"
         << endl;

    cout << "Team "
         << survivor + 1
         << endl;


    cout << "\nInterpretation:"
         << endl;

    cout << "The Josephus algorithm determines the"
         << endl;

    cout << "remaining team in a circular selection"
         << endl;

    cout << "process."
         << endl;


    cout << "\n----------------------------------------"
         << endl;

    cout << "Josephus Problem completed successfully."
         << endl;


    return 0;
}