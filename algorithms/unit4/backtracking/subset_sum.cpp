#include <iostream>
#include <vector>

using namespace std;

bool subsetSum(
    const vector<int>& resources,
    int index,
    int remaining,
    vector<int>& selected
) {
    // Target achieved
    if (remaining == 0) {
        return true;
    }

    // No more resources or target exceeded
    if (index == resources.size() ||
        remaining < 0) {
        return false;
    }

    // Choose current resource
    selected.push_back(resources[index]);

    if (subsetSum(
            resources,
            index + 1,
            remaining - resources[index],
            selected)) {
        return true;
    }

    // Backtrack
    selected.pop_back();

    // Skip current resource
    if (subsetSum(
            resources,
            index + 1,
            remaining,
            selected)) {
        return true;
    }

    return false;
}

int main() {

    // Available emergency resource quantities
    vector<int> resources = {
        10, 15, 20, 25, 30
    };

    int target = 50;

    vector<int> selected;

    cout << "========================================" << endl;
    cout << "             SUBSET SUM" << endl;
    cout << "             BACKTRACKING" << endl;
    cout << "========================================" << endl;

    cout << "\nAvailable emergency resource quantities:"
         << endl;

    for (int value : resources) {
        cout << value << " ";
    }

    cout << "\n\nRequired target quantity = "
         << target
         << endl;

    cout << "\n----------------------------------------" << endl;

    if (subsetSum(
            resources,
            0,
            target,
            selected)) {

        cout << "Subset found:" << endl;

        cout << endl;

        for (int value : selected) {
            cout << value << " ";
        }

        cout << "\n\nSubset sum = "
             << target
             << endl;

        cout << "\n----------------------------------------" << endl;

        cout << "The selected resource quantities"
             << endl;

        cout << "exactly satisfy the required target."
             << endl;
    }
    else {

        cout << "No subset can produce the target."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Subset Sum Backtracking completed successfully."
         << endl;

    return 0;
}