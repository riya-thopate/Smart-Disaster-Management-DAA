
#include <bits/stdc++.h>
using namespace std;

struct Resource {
    string id;
    string type;
    string locationId;
    double quantity;
    string unit;
    double priority;
};

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string upper(string s) {
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) {
            return static_cast<char>(toupper(c));
        });
    return s;
}

vector<string> splitCSV(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);

    while (getline(ss, field, ',')) {
        fields.push_back(trim(field));
    }

    return fields;
}

string jsonString(const string& s) {
    string result = "\"";

    for (unsigned char c : s) {
        if (c == '"' || c == '\\') {
            result += '\\';
            result += c;
        } else if (c == '\n') {
            result += "\\n";
        } else if (c == '\r') {
            result += "\\r";
        } else if (c == '\t') {
            result += "\\t";
        } else {
            result += c;
        }
    }

    result += '"';
    return result;
}

vector<Resource> loadResources(
    const string& path,
    const string& requestedType
) {
    ifstream file(path);

    if (!file) {
        throw runtime_error("Cannot open resources.csv");
    }

    vector<Resource> resources;
    string line;
    string commonUnit;

    getline(file, line); // Skip CSV header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> f = splitCSV(line);

        if (f.size() < 6) continue;

        Resource r;
        r.id = f[0];
        r.type = upper(f[1]);
        r.locationId = f[2];
        r.quantity = stod(f[3]);
        r.unit = f[4];
        r.priority = stod(f[5]);

        // Filter first so quantities from different
        // measurement units are never mixed.
        if (r.type != requestedType) continue;

        if (r.quantity < 0 || r.priority < 0) {
            throw runtime_error(
                "Resource quantity and priority must be non-negative"
            );
        }

        if (commonUnit.empty()) {
            commonUnit = r.unit;
        } else if (upper(commonUnit) != upper(r.unit)) {
            throw runtime_error(
                "Selected resource type contains incompatible units"
            );
        }

        if (r.quantity > 0) {
            resources.push_back(r);
        }
    }

    return resources;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 4) {
            cerr << "Usage: fractional_knapsack_api.exe "
                 << "resources.csv RESOURCE_TYPE CAPACITY\n";
            return 1;
        }

        string csvPath = argv[1];
        string resourceType = upper(argv[2]);
        double capacity = stod(argv[3]);

        if (!isfinite(capacity) || capacity <= 0) {
            throw runtime_error(
                "Capacity must be a positive finite number"
            );
        }

        vector<Resource> resources =
            loadResources(csvPath, resourceType);

        // Greedy strategy: highest priority per unit first.
        // All candidates have the same measurement unit.
        sort(resources.begin(), resources.end(),
            [](const Resource& a, const Resource& b) {
                if (a.priority != b.priority)
                    return a.priority > b.priority;

                return a.id < b.id;
            });

        double remaining = capacity;
        double totalAllocated = 0;
        double totalValue = 0;

        struct Allocation {
            Resource resource;
            double allocated;
            double value;
        };

        vector<Allocation> allocations;

        for (const Resource& r : resources) {
            if (remaining <= 1e-9) break;

            double take = min(r.quantity, remaining);

            if (take <= 0) continue;

            double value = take * r.priority;

            allocations.push_back({
                r, take, value
            });

            totalAllocated += take;
            totalValue += value;
            remaining -= take;
        }

        cout << fixed << setprecision(2);

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":\"Fractional Knapsack\",";
        cout << "\"resource_type\":" << jsonString(resourceType) << ",";
        cout << "\"unit\":";

        if (allocations.empty()) {
            cout << "null,";
        } else {
            cout << jsonString(allocations[0].resource.unit) << ",";
        }

        cout << "\"capacity\":" << capacity << ",";
        cout << "\"allocated_quantity\":" << totalAllocated << ",";
        cout << "\"remaining_capacity\":"
             << max(0.0, remaining) << ",";
        cout << "\"total_priority_value\":" << totalValue << ",";
        cout << "\"selected_resources\":[";

        for (size_t i = 0; i < allocations.size(); i++) {
            if (i > 0) cout << ",";

            const Allocation& a = allocations[i];

            cout << "{";
            cout << "\"resource_id\":"
                 << jsonString(a.resource.id) << ",";
            cout << "\"type\":"
                 << jsonString(a.resource.type) << ",";
            cout << "\"location_id\":"
                 << jsonString(a.resource.locationId) << ",";
            cout << "\"available_quantity\":"
                 << a.resource.quantity << ",";
            cout << "\"allocated_quantity\":"
                 << a.allocated << ",";
            cout << "\"unit\":"
                 << jsonString(a.resource.unit) << ",";
            cout << "\"priority\":"
                 << a.resource.priority << ",";
            cout << "\"allocated_value\":"
                 << a.value;
            cout << "}";
        }

        cout << "],";
        cout << "\"time_complexity\":\"O(n log n)\",";
        cout << "\"space_complexity\":\"O(n)\"";
        cout << "}\n";

        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
