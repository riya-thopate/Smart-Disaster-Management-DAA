#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// dp[node][0] = maximum independent set
//                when node is NOT selected
//
// dp[node][1] = maximum independent set
//                when node IS selected

vector<vector<int>> tree;
vector<vector<int>> dp;

void calculateDP(int node, int parent) {

    // If node is selected, its children
    // cannot be selected.
    dp[node][1] = 1;

    // If node is not selected, each child
    // can either be selected or not.
    dp[node][0] = 0;

    for (int child : tree[node]) {

        if (child == parent)
            continue;

        calculateDP(child, node);

        // Node is selected
        dp[node][1] += dp[child][0];

        // Node is not selected
        dp[node][0] +=
            max(
                dp[child][0],
                dp[child][1]
            );
    }
}

// Reconstruct selected nodes
void reconstruct(
    int node,
    int parent,
    bool parentSelected,
    vector<bool>& selected
) {

    // If parent is selected,
    // current node cannot be selected.
    if (parentSelected) {
        selected[node] = false;
    }
    else {

        // Choose the better option
        if (dp[node][1] > dp[node][0]) {
            selected[node] = true;
        }
        else {
            selected[node] = false;
        }
    }

    for (int child : tree[node]) {

        if (child == parent)
            continue;

        reconstruct(
            child,
            node,
            selected[node],
            selected
        );
    }
}

int main() {

    // Disaster-response hierarchy tree:
    //
    //                 Emergency Center
    //                  /            \
    //         Damage Assessment   Resource Center
    //            /      \            /        \
    //     Rescue A    Rescue B   Evacuation   Shelter
    //
    // No two directly connected locations
    // can be selected together.

    vector<string> locations = {

        "Emergency Center",
        "Damage Assessment",
        "Resource Center",
        "Rescue Team A",
        "Rescue Team B",
        "Evacuation Point",
        "Shelter"
    };

    int n = locations.size();

    tree.resize(n);
    dp.assign(
        n,
        vector<int>(2, 0)
    );

    // Construct tree

    tree[0].push_back(1);
    tree[1].push_back(0);

    tree[0].push_back(2);
    tree[2].push_back(0);

    tree[1].push_back(3);
    tree[3].push_back(1);

    tree[1].push_back(4);
    tree[4].push_back(1);

    tree[2].push_back(5);
    tree[5].push_back(2);

    tree[2].push_back(6);
    tree[6].push_back(2);

    calculateDP(0, -1);

    int maximumSet =
        max(
            dp[0][0],
            dp[0][1]
        );

    vector<bool> selected(n, false);

    reconstruct(
        0,
        -1,
        false,
        selected
    );

    cout << "========================================" << endl;
    cout << "       LARGEST INDEPENDENT SET" << endl;
    cout << "             FOR TREES" << endl;
    cout << "          DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-response hierarchy:" << endl;

    for (int i = 0; i < n; i++) {

        cout << i
             << " -> "
             << locations[i]
             << endl;
    }

    cout << "\nTree connections:" << endl;

    cout << "Emergency Center"
         << " -> Damage Assessment"
         << endl;

    cout << "Emergency Center"
         << " -> Resource Center"
         << endl;

    cout << "Damage Assessment"
         << " -> Rescue Team A"
         << endl;

    cout << "Damage Assessment"
         << " -> Rescue Team B"
         << endl;

    cout << "Resource Center"
         << " -> Evacuation Point"
         << endl;

    cout << "Resource Center"
         << " -> Shelter"
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Maximum Independent Set Size = "
         << maximumSet
         << endl;

    cout << "\nSelected locations:" << endl;

    for (int i = 0; i < n; i++) {

        if (selected[i]) {
            cout << "- "
                 << locations[i]
                 << endl;
        }
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "The Dynamic Programming algorithm selects"
         << endl;

    cout << "the maximum number of non-adjacent disaster"
         << endl;

    cout << "response locations in the hierarchy." << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Largest Independent Set completed successfully."
         << endl;

    return 0;
}