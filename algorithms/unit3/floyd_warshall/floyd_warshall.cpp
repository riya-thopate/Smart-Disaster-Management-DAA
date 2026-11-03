#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int INF = 999999;

int main() {

    vector<string> locations = {
        "Emergency Center",
        "Industrial Area",
        "Market Area",
        "City Hospital",
        "Central Shelter"
    };

    int n = locations.size();

    // Distance matrix
    // INF means there is no direct road.
    vector<vector<int>> dist = {

        {0,  14, INF, INF, INF},
        {14, 0,  34, 20,  INF},
        {INF, 34, 0,  14,  24},
        {INF, 20, 14, 0,   24},
        {INF, INF, 24, 24, 0}
    };

    cout << "========================================" << endl;
    cout << "           FLOYD-WARSHALL" << endl;
    cout << "           DYNAMIC PROGRAMMING" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster-management locations:" << endl;

    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". "
             << locations[i] << endl;
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < n; k++) {

        for (int i = 0; i < n; i++) {

            for (int j = 0; j < n; j++) {

                if (dist[i][k] != INF &&
                    dist[k][j] != INF) {

                    if (dist[i][j] >
                        dist[i][k] + dist[k][j]) {

                        dist[i][j] =
                            dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    cout << "\n----------------------------------------" << endl;
    cout << "All-Pairs Shortest Distance Matrix:" << endl;
    cout << "----------------------------------------" << endl;

    for (int i = 0; i < n; i++) {

        cout << "\nFrom "
             << locations[i]
             << ":" << endl;

        for (int j = 0; j < n; j++) {

            cout << "  To "
                 << locations[j]
                 << " = ";

            if (dist[i][j] == INF) {
                cout << "Unreachable";
            }
            else {
                cout << dist[i][j]
                     << " km";
            }

            cout << endl;
        }
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Example:" << endl;

    cout << "Shortest distance from Emergency Center"
         << endl;

    cout << "to City Hospital = "
         << dist[0][3]
         << " km"
         << endl;

    cout << "\nShortest distance from Industrial Area"
         << endl;

    cout << "to Central Shelter = "
         << dist[1][4]
         << " km"
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "Floyd-Warshall calculates the shortest"
         << endl;

    cout << "path between every pair of disaster"
         << endl;

    cout << "management locations." << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Floyd-Warshall completed successfully."
         << endl;

    return 0;
}