#include <iostream>

using namespace std;


// --------------------------------------------------
// Euclid's Algorithm
// --------------------------------------------------

int gcd(int a, int b) {

    if (b == 0) {
        return a;
    }

    return gcd(b, a % b);
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    // Example disaster resource quantities
    int waterPackets = 1000;
    int foodPackets = 500;


    cout << "========================================"
         << endl;

    cout << "       DISASTER RESOURCE GCD"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nResource Quantities:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    cout << "Water  : "
         << waterPackets
         << " units"
         << endl;

    cout << "Food   : "
         << foodPackets
         << " units"
         << endl;


    int result =
        gcd(waterPackets, foodPackets);


    cout << "\n----------------------------------------"
         << endl;

    cout << "GCD of resource quantities: "
         << result
         << endl;


    cout << "\nInterpretation:"
         << endl;

    cout << "The resources can be divided into equal"
         << endl;

    cout << "groups of "
         << result
         << " units."
         << endl;


    cout << "\n----------------------------------------"
         << endl;

    cout << "Euclid's Algorithm completed successfully."
         << endl;


    return 0;
}