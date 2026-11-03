#include <iostream>

using namespace std;


// --------------------------------------------------
// Tower of Hanoi using Recursion
// --------------------------------------------------

void towerOfHanoi(int n, char source,
                  char auxiliary, char destination) {

    // Base case: only one disk
    if (n == 1) {
        cout << "Move disk 1 from "
             << source << " to "
             << destination << endl;
        return;
    }

    // Move n-1 disks from source to auxiliary
    towerOfHanoi(n - 1, source, destination, auxiliary);

    // Move the largest disk to destination
    cout << "Move disk " << n
         << " from " << source
         << " to " << destination << endl;

    // Move n-1 disks from auxiliary to destination
    towerOfHanoi(n - 1, auxiliary, source, destination);
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    int numberOfDisks = 3;

    cout << "========================================" << endl;
    cout << "          TOWER OF HANOI" << endl;
    cout << "========================================" << endl;

    cout << "\nNumber of disks: "
         << numberOfDisks << endl;

    cout << "\nRod A: Source"
         << "\nRod B: Auxiliary"
         << "\nRod C: Destination" << endl;

    cout << "\nSteps to move all disks:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    towerOfHanoi(numberOfDisks, 'A', 'B', 'C');

    cout << "\n----------------------------------------"
         << endl;

    cout << "Total moves required: "
         << (1 << numberOfDisks) - 1 << endl;

    cout << "Tower of Hanoi completed successfully."
         << endl;

    return 0;
}