
#include <bits/stdc++.h>
using namespace std;

struct Task {
    string id;
    string name;
    string dependencies;
};

string trim(string s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
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

vector<string> splitDependencies(const string& input) {
    vector<string> dependencies;
    string token;
    stringstream ss(input);

    while (getline(ss, token, ';')) {
        token = trim(token);

        if (!token.empty() && token != "-")
            dependencies.push_back(token);
    }

    return dependencies;
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

vector<Task> loadTasks(const string& path) {
    ifstream file(path);

    if (!file)
        throw runtime_error("Cannot open tasks.csv");

    vector<Task> tasks;
    string line;

    getline(file, line); // Skip header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> fields = splitCSV(line);

        if (fields.size() < 3)
            throw runtime_error(
                "Each task row needs task_id, task_name, depends_on"
            );

        Task task;
        task.id = fields[0];
        task.name = fields[1];
        task.dependencies = fields[2];

        if (task.id.empty() || task.name.empty())
            throw runtime_error(
                "Task ID and task name cannot be empty"
            );

        tasks.push_back(task);
    }

    if (tasks.empty())
        throw runtime_error("No tasks found in tasks.csv");

    return tasks;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            cerr << "Usage: topological_sort_api.exe tasks.csv\n";
            return 1;
        }

        vector<Task> tasks = loadTasks(argv[1]);
        int n = static_cast<int>(tasks.size());

        map<string, int> taskIndex;

        for (int i = 0; i < n; i++) {
            if (taskIndex.count(tasks[i].id))
                throw runtime_error(
                    "Duplicate task ID: " + tasks[i].id
                );

            taskIndex[tasks[i].id] = i;
        }

        vector<vector<int>> graph(n);
        vector<int> indegree(n, 0);

        // An edge prerequisite -> task represents a dependency.
        set<pair<int, int>> uniqueEdges;

        for (int i = 0; i < n; i++) {
            vector<string> dependencies =
                splitDependencies(tasks[i].dependencies);

            for (const string& prerequisite : dependencies) {
                auto it = taskIndex.find(prerequisite);

                if (it == taskIndex.end()) {
                    throw runtime_error(
                        "Unknown prerequisite '" + prerequisite +
                        "' for task " + tasks[i].id
                    );
                }

                int u = it->second;
                int v = i;

                if (u == v)
                    throw runtime_error(
                        "A task cannot depend on itself: " + tasks[i].id
                    );

                if (uniqueEdges.insert({u, v}).second) {
                    graph[u].push_back(v);
                    indegree[v]++;
                }
            }
        }

        // Kahn's Algorithm: begin with tasks having
        // no outstanding prerequisites.
        queue<int> ready;

        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0)
                ready.push(i);
        }

        vector<int> order;
        order.reserve(n);

        while (!ready.empty()) {
            int u = ready.front();
            ready.pop();

            order.push_back(u);

            for (int v : graph[u]) {
                indegree[v]--;

                if (indegree[v] == 0)
                    ready.push(v);
            }
        }

        if (static_cast<int>(order.size()) != n) {
            cout << "{";
            cout << "\"success\":false,";
            cout << "\"algorithm\":\"Kahn's Topological Sort\",";
            cout << "\"cycle_detected\":true,";
            cout << "\"message\":\"Task dependencies contain a cycle\"";
            cout << "}\n";
            return 0;
        }

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":\"Kahn's Topological Sort\",";
        cout << "\"task_count\":" << n << ",";
        cout << "\"dependency_count\":" << uniqueEdges.size() << ",";
        cout << "\"cycle_detected\":false,";
        cout << "\"task_order\":[";

        for (size_t i = 0; i < order.size(); i++) {
            if (i > 0) cout << ",";

            const Task& task = tasks[order[i]];

            cout << "{";
            cout << "\"task_id\":" << jsonString(task.id) << ",";
            cout << "\"task_name\":" << jsonString(task.name);
            cout << "}";
        }

        cout << "],";
        cout << "\"time_complexity\":\"O(V + E)\",";
        cout << "\"space_complexity\":\"O(V + E)\"";
        cout << "}\n";

        return 0;
    }
    catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
