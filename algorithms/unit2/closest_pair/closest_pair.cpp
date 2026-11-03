#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

struct Point {
    string name;
    double x;
    double y;
};


// Calculate Euclidean distance
double distanceBetween(const Point& p1, const Point& p2) {

    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;

    return sqrt(dx * dx + dy * dy);
}


// --------------------------------------------------
// Brute Force for small number of points
// --------------------------------------------------

double bruteForce(const vector<Point>& points,
                  int left,
                  int right,
                  pair<Point, Point>& closestPair) {

    double minimumDistance = 1e9;

    for (int i = left; i <= right; i++) {

        for (int j = i + 1; j <= right; j++) {

            double currentDistance =
                distanceBetween(points[i], points[j]);

            if (currentDistance < minimumDistance) {

                minimumDistance = currentDistance;

                closestPair =
                    {points[i], points[j]};
            }
        }
    }

    return minimumDistance;
}


// --------------------------------------------------
// Divide and Conquer Closest Pair
// --------------------------------------------------

double closestPairRec(vector<Point>& points,
                      int left,
                      int right,
                      pair<Point, Point>& closestPair) {

    // For 2 or 3 points, use brute force
    if (right - left <= 2) {

        return bruteForce(
            points,
            left,
            right,
            closestPair
        );
    }


    // Find middle
    int middle = (left + right) / 2;

    double leftDistance;
    double rightDistance;

    pair<Point, Point> leftPair;
    pair<Point, Point> rightPair;


    // Solve left half
    leftDistance =
        closestPairRec(
            points,
            left,
            middle,
            leftPair
        );


    // Solve right half
    rightDistance =
        closestPairRec(
            points,
            middle + 1,
            right,
            rightPair
        );


    // Select smaller distance
    double minimumDistance;

    if (leftDistance < rightDistance) {

        minimumDistance = leftDistance;
        closestPair = leftPair;

    }
    else {

        minimumDistance = rightDistance;
        closestPair = rightPair;
    }


    // Create strip around dividing line
    double middleX = points[middle].x;

    vector<Point> strip;


    for (int i = left; i <= right; i++) {

        if (abs(points[i].x - middleX)
            < minimumDistance) {

            strip.push_back(points[i]);
        }
    }


    // Sort strip by Y coordinate
    sort(
        strip.begin(),
        strip.end(),
        [](const Point& a, const Point& b) {

            return a.y < b.y;
        }
    );


    // Check points in strip
    for (int i = 0; i < (int)strip.size(); i++) {

        for (int j = i + 1;
             j < (int)strip.size() &&
             (strip[j].y - strip[i].y) < minimumDistance;
             j++) {

            double currentDistance =
                distanceBetween(
                    strip[i],
                    strip[j]
                );


            if (currentDistance < minimumDistance) {

                minimumDistance = currentDistance;

                closestPair =
                    {strip[i], strip[j]};
            }
        }
    }


    return minimumDistance;
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    vector<Point> emergencyLocations = {

        {"Industrial Area", 30, 10},

        {"Market Area", 20, 30},

        {"South Junction", 10, 0},

        {"West Junction", 0, 10},

        {"North Junction", 10, 20}

    };


    // Sort points by X coordinate

    sort(
        emergencyLocations.begin(),
        emergencyLocations.end(),
        [](const Point& a, const Point& b) {

            return a.x < b.x;
        }
    );


    cout << "========================================"
         << endl;

    cout << "       CLOSEST EMERGENCY LOCATIONS"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nEmergency Location Coordinates:"
         << endl;

    cout << "----------------------------------------"
         << endl;


    for (const Point& point : emergencyLocations) {

        cout << point.name
             << " -> ("
             << point.x
             << ", "
             << point.y
             << ")"
             << endl;
    }


    pair<Point, Point> closestPair;


    double minimumDistance =
        closestPairRec(
            emergencyLocations,
            0,
            emergencyLocations.size() - 1,
            closestPair
        );


    cout << "\n----------------------------------------"
         << endl;

    cout << "Closest Pair:"
         << endl;

    cout << "----------------------------------------"
         << endl;


    cout << closestPair.first.name
         << " <-> "
         << closestPair.second.name
         << endl;


    cout << fixed
         << setprecision(2);


    cout << "Distance: "
         << minimumDistance
         << " units"
         << endl;


    cout << "\n----------------------------------------"
         << endl;

    cout << "Closest Pair algorithm completed successfully."
         << endl;


    return 0;
}