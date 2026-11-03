#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Emergency {
    string id;
    string locationId;
    string type;
    string severity;
    int affectedPeople;
    string resource;
    string status;
};

// --------------------------------------------------
// JSON escaping
// --------------------------------------------------
string jsonEscape(const string& value) {

    string result;

    for (char c : value) {

        if (c == '\\') {
            result += "\\\\";
        }
        else if (c == '"') {
            result += "\\\"";
        }
        else {
            result += c;
        }
    }

    return result;
}

// --------------------------------------------------
// Split CSV line
// Our current CSV fields do not contain commas,
// so simple splitting is sufficient here.
// --------------------------------------------------
vector<string> splitCSV(const string& line) {

    vector<string> fields;
    string field;

    stringstream ss(line);

    while (getline(ss, field, ',')) {
        fields.push_back(field);
    }

    return fields;
}

// --------------------------------------------------
// Read emergencies.csv
// --------------------------------------------------
bool loadEmergencies(
    const string& csvPath,
    vector<Emergency>& emergencies
) {

    ifstream file(csvPath);

    if (!file.is_open()) {
        return false;
    }

    string line;

    // Skip header
    if (!getline(file, line)) {
        return false;
    }

    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        vector<string> fields =
            splitCSV(line);

        if (fields.size() < 7) {
            continue;
        }

        Emergency emergency;

        emergency.id =
            fields[0];

        emergency.locationId =
            fields[1];

        emergency.type =
            fields[2];

        emergency.severity =
            fields[3];

        emergency.affectedPeople =
            stoi(fields[4]);

        emergency.resource =
            fields[5];

        emergency.status =
            fields[6];

        emergencies.push_back(
            emergency
        );
    }

    return true;
}

// --------------------------------------------------
// Genuine Merge Sort
// Sort by affected people
// --------------------------------------------------
void merge(
    vector<Emergency>& emergencies,
    int left,
    int middle,
    int right,
    bool ascending
) {

    int leftSize =
        middle - left + 1;

    int rightSize =
        right - middle;

    vector<Emergency> leftPart(
        leftSize
    );

    vector<Emergency> rightPart(
        rightSize
    );

    for (int i = 0; i < leftSize; i++) {
        leftPart[i] =
            emergencies[left + i];
    }

    for (int j = 0; j < rightSize; j++) {
        rightPart[j] =
            emergencies[middle + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (
        i < leftSize &&
        j < rightSize
    ) {

        bool takeLeft;

        if (ascending) {

            takeLeft =
                leftPart[i].affectedPeople
                <= rightPart[j].affectedPeople;

        }
        else {

            takeLeft =
                leftPart[i].affectedPeople
                >= rightPart[j].affectedPeople;
        }

        if (takeLeft) {

            emergencies[k] =
                leftPart[i];

            i++;
        }
        else {

            emergencies[k] =
                rightPart[j];

            j++;
        }

        k++;
    }

    while (i < leftSize) {

        emergencies[k] =
            leftPart[i];

        i++;
        k++;
    }

    while (j < rightSize) {

        emergencies[k] =
            rightPart[j];

        j++;
        k++;
    }
}

void mergeSort(
    vector<Emergency>& emergencies,
    int left,
    int right,
    bool ascending
) {

    if (left >= right) {
        return;
    }

    int middle =
        left + (right - left) / 2;

    mergeSort(
        emergencies,
        left,
        middle,
        ascending
    );

    mergeSort(
        emergencies,
        middle + 1,
        right,
        ascending
    );

    merge(
        emergencies,
        left,
        middle,
        right,
        ascending
    );
}

// --------------------------------------------------
// Linear Search
// Searches ID, type, location ID,
// severity and status.
// --------------------------------------------------
vector<Emergency> linearSearch(
    const vector<Emergency>& emergencies,
    const string& query
) {

    vector<Emergency> results;

    string searchValue =
        query;

    transform(
        searchValue.begin(),
        searchValue.end(),
        searchValue.begin(),
        ::toupper
    );

    for (const Emergency& emergency :
         emergencies) {

        string id =
            emergency.id;

        string locationId =
            emergency.locationId;

        string type =
            emergency.type;

        string severity =
            emergency.severity;

        string resource =
            emergency.resource;

        string status =
            emergency.status;

        transform(
            id.begin(),
            id.end(),
            id.begin(),
            ::toupper
        );

        transform(
            locationId.begin(),
            locationId.end(),
            locationId.begin(),
            ::toupper
        );

        transform(
            type.begin(),
            type.end(),
            type.begin(),
            ::toupper
        );

        transform(
            severity.begin(),
            severity.end(),
            severity.begin(),
            ::toupper
        );

        transform(
            resource.begin(),
            resource.end(),
            resource.begin(),
            ::toupper
        );

        transform(
            status.begin(),
            status.end(),
            status.begin(),
            ::toupper
        );

        if (
            id.find(searchValue) != string::npos ||
            locationId.find(searchValue) != string::npos ||
            type.find(searchValue) != string::npos ||
            severity.find(searchValue) != string::npos ||
            resource.find(searchValue) != string::npos ||
            status.find(searchValue) != string::npos
        ) {

            results.push_back(
                emergency
            );
        }
    }

    return results;
}

// --------------------------------------------------
// Print one emergency as JSON
// --------------------------------------------------
void printEmergencyJSON(
    const Emergency& emergency
) {

    cout << "{";

    cout << "\"id\":\""
         << jsonEscape(emergency.id)
         << "\",";

    cout << "\"location_id\":\""
         << jsonEscape(emergency.locationId)
         << "\",";

    cout << "\"type\":\""
         << jsonEscape(emergency.type)
         << "\",";

    cout << "\"severity\":\""
         << jsonEscape(emergency.severity)
         << "\",";

    cout << "\"affected_people\":"
         << emergency.affectedPeople
         << ",";

    cout << "\"required_resource\":\""
         << jsonEscape(emergency.resource)
         << "\",";

    cout << "\"status\":\""
         << jsonEscape(emergency.status)
         << "\"";

    cout << "}";
}

// --------------------------------------------------
// Print emergency list
// --------------------------------------------------
void printEmergencyList(
    const vector<Emergency>& emergencies
) {

    cout << "[";

    for (
        size_t i = 0;
        i < emergencies.size();
        i++
    ) {

        printEmergencyJSON(
            emergencies[i]
        );

        if (
            i != emergencies.size() - 1
        ) {
            cout << ",";
        }
    }

    cout << "]";
}

// --------------------------------------------------
// Main
//
// Usage:
//
// emergency_api.exe SORT DESC csvPath
// emergency_api.exe SORT ASC  csvPath
// emergency_api.exe SEARCH FIRE csvPath
// --------------------------------------------------
int main(int argc, char* argv[]) {

    if (argc != 4) {

        cout << "{";
        cout << "\"success\":false,";
        cout << "\"message\":\"Usage: emergency_api.exe SORT|SEARCH VALUE csvPath\"";
        cout << "}";

        return 1;
    }

    string operation =
        argv[1];

    string value =
        argv[2];

    string csvPath =
        argv[3];

    transform(
        operation.begin(),
        operation.end(),
        operation.begin(),
        ::toupper
    );

    vector<Emergency> emergencies;

    if (!loadEmergencies(
            csvPath,
            emergencies
        )) {

        cout << "{";

        cout << "\"success\":false,";

        cout << "\"message\":\"Unable to read emergencies.csv\",";

        cout << "\"csv_path\":\""
             << jsonEscape(csvPath)
             << "\"";

        cout << "}";

        return 1;
    }

    // --------------------------------------------------
    // SORT
    // --------------------------------------------------
    if (operation == "SORT") {

        string order =
            value;

        transform(
            order.begin(),
            order.end(),
            order.begin(),
            ::toupper
        );

        if (
            order != "ASC" &&
            order != "DESC"
        ) {

            cout << "{";

            cout << "\"success\":false,";
            cout << "\"message\":\"Sort order must be ASC or DESC\"";

            cout << "}";

            return 1;
        }

        bool ascending =
            order == "ASC";

        if (!emergencies.empty()) {

            mergeSort(
                emergencies,
                0,
                emergencies.size() - 1,
                ascending
            );
        }

        cout << "{";

        cout << "\"success\":true,";

        cout << "\"algorithm\":\"Merge Sort\",";

        cout << "\"operation\":\"SORT\",";

        cout << "\"sort_field\":\"affected_people\",";

        cout << "\"order\":\""
             << order
             << "\",";

        cout << "\"count\":"
             << emergencies.size()
             << ",";

        cout << "\"emergencies\":";

        printEmergencyList(
            emergencies
        );

        cout << ",";

        cout << "\"time_complexity\":\"O(n log n)\",";

        cout << "\"space_complexity\":\"O(n)\"";

        cout << "}";

        return 0;
    }

    // --------------------------------------------------
    // SEARCH
    // --------------------------------------------------
    if (operation == "SEARCH") {

        vector<Emergency> results =
            linearSearch(
                emergencies,
                value
            );

        cout << "{";

        cout << "\"success\":true,";

        cout << "\"algorithm\":\"Linear Search\",";

        cout << "\"operation\":\"SEARCH\",";

        cout << "\"query\":\""
             << jsonEscape(value)
             << "\",";

        cout << "\"count\":"
             << results.size()
             << ",";

        cout << "\"emergencies\":";

        printEmergencyList(
            results
        );

        cout << ",";

        cout << "\"time_complexity\":\"O(n)\",";

        cout << "\"space_complexity\":\"O(n)\"";

        cout << "}";

        return 0;
    }

    cout << "{";

    cout << "\"success\":false,";

    cout << "\"message\":\"Unknown operation\"";

    cout << "}";

    return 1;
}