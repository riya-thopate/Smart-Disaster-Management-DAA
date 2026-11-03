#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

struct Resource {
    string name;
    double value;
    double weight;
    double ratio;
};

int main() {

    vector<Resource> resources = {

        {"Water", 60, 10, 0},
        {"Medicine", 100, 20, 0},
        {"Food", 120, 30, 0},
        {"Blankets", 80, 40, 0},
        {"First Aid Kits", 30, 10, 0}
    };

    double capacity = 65;

    // Calculate value/weight ratio
    for (Resource& resource : resources) {

        resource.ratio =
            resource.value / resource.weight;
    }

    // Sort by highest value/weight ratio
    sort(
        resources.begin(),
        resources.end(),
        [](const Resource& a, const Resource& b) {

            return a.ratio > b.ratio;
        }
    );

    double totalValue = 0;
    double remainingCapacity = capacity;

    cout << "========================================" << endl;
    cout << "         FRACTIONAL KNAPSACK" << endl;
    cout << "            GREEDY ALGORITHM" << endl;
    cout << "========================================" << endl;

    cout << "\nAvailable emergency resources:" << endl;

    cout << fixed << setprecision(2);

    for (const Resource& resource : resources) {

        cout << resource.name
             << " | Value: "
             << resource.value
             << " | Weight: "
             << resource.weight
             << " | Ratio: "
             << resource.ratio
             << endl;
    }

    cout << "\nEmergency vehicle capacity = "
         << capacity
         << " kg"
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Selected resources:" << endl;

    for (const Resource& resource : resources) {

        if (remainingCapacity <= 0) {
            break;
        }

        if (resource.weight <= remainingCapacity) {

            // Take complete resource
            remainingCapacity -=
                resource.weight;

            totalValue +=
                resource.value;

            cout << resource.name
                 << " -> 100%"
                 << " | Value added: "
                 << resource.value
                 << endl;
        }
        else {

            // Take fraction of resource
            double fraction =
                remainingCapacity /
                resource.weight;

            double valueAdded =
                resource.value * fraction;

            totalValue += valueAdded;

            cout << resource.name
                 << " -> "
                 << fraction * 100
                 << "%"
                 << " | Value added: "
                 << valueAdded
                 << endl;

            remainingCapacity = 0;
        }
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Maximum resource value = "
         << totalValue
         << endl;

    cout << "Remaining capacity = "
         << remainingCapacity
         << " kg"
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "The greedy algorithm selects resources"
         << endl;

    cout << "with the highest value-to-weight ratio"
         << endl;

    cout << "to maximize the total emergency resource"
         << endl;

    cout << "value within the available capacity."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Fractional Knapsack completed successfully."
         << endl;

    return 0;
}