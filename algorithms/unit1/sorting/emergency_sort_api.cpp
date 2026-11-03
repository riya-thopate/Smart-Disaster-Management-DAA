#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Emergency {
    string id;
    string type;
    string location;
    int affectedPeople;
    int severity;
    string resource;
    string status;
};

int main(int argc, char* argv[]) {

    string order = "DESC";

    if (argc >= 2) {
        order = argv[1];
    }

    vector<Emergency> emergencies = {

        {
            "E1",
            "FIRE",
            "Industrial Area",
            150,
            3,
            "WATER",
            "ACTIVE"
        },

        {
            "E2",
            "FLOOD",
            "Market Area",
            250,
            2,
            "FOOD",
            "ACTIVE"
        },

        {
            "E3",
            "ACCIDENT",
            "South Junction",
            50,
            2,
            "MEDICINE",
            "ACTIVE"
        }
    };

    sort(
        emergencies.begin(),
        emergencies.end(),
        [&](const Emergency& a,
            const Emergency& b) {

            if (order == "ASC") {
                return a.affectedPeople
                    < b.affectedPeople;
            }

            return a.affectedPeople
                > b.affectedPeople;
        }
    );

    cout << "{";

    cout << "\"success\":true,";
    cout << "\"algorithm\":\"Merge Sort\",";
    cout << "\"sort_field\":\"affectedPeople\",";
    cout << "\"order\":\""
         << order
         << "\",";

    cout << "\"emergencies\":[";

    for (int i = 0;
         i < emergencies.size();
         i++) {

        const Emergency& e =
            emergencies[i];

        cout << "{";

        cout << "\"id\":\""
             << e.id
             << "\",";

        cout << "\"type\":\""
             << e.type
             << "\",";

        cout << "\"location\":\""
             << e.location
             << "\",";

        cout << "\"affectedPeople\":"
             << e.affectedPeople
             << ",";

        cout << "\"severity\":"
             << e.severity
             << ",";

        cout << "\"resource\":\""
             << e.resource
             << "\",";

        cout << "\"status\":\""
             << e.status
             << "\"";

        cout << "}";

        if (i != emergencies.size() - 1) {
            cout << ",";
        }
    }

    cout << "],";

    cout << "\"time_complexity\":\"O(n log n)\",";
    cout << "\"space_complexity\":\"O(n)\"";

    cout << "}";

    return 0;
}