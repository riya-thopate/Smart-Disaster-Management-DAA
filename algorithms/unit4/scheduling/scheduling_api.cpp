
#include <bits/stdc++.h>
using namespace std;

struct Job {
    string id;
    string name;
    int deadline;
    int priority;
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

vector<Job> loadJobs(const string& path) {
    ifstream file(path);

    if (!file)
        throw runtime_error("Cannot open scheduling_jobs.csv");

    vector<Job> jobs;
    string line;

    getline(file, line); // Skip CSV header.

    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        vector<string> fields = splitCSV(line);

        if (fields.size() < 4)
            throw runtime_error("Invalid job row in CSV");

        Job job;
        job.id = fields[0];
        job.name = fields[1];

        size_t usedDeadline = 0;
        size_t usedPriority = 0;

        job.deadline = stoi(fields[2], &usedDeadline);
        job.priority = stoi(fields[3], &usedPriority);

        if (usedDeadline != fields[2].size() ||
            usedPriority != fields[3].size()) {
            throw runtime_error(
                "Deadline and priority must be integers"
            );
        }

        if (job.id.empty() || job.name.empty() ||
            job.deadline <= 0 || job.priority < 0) {
            throw runtime_error(
                "Invalid job: deadline must be positive "
                "and priority must be non-negative"
            );
        }

        jobs.push_back(job);
    }

    return jobs;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            cerr << "Usage: scheduling_api.exe scheduling_jobs.csv\n";
            return 1;
        }

        vector<Job> jobs = loadJobs(argv[1]);

        if (jobs.empty())
            throw runtime_error("No jobs found");

        set<string> ids;

        for (const Job& job : jobs) {
            if (!ids.insert(job.id).second)
                throw runtime_error("Duplicate job ID: " + job.id);
        }

        // Greedy choice: consider highest priority first.
        sort(jobs.begin(), jobs.end(),
            [](const Job& a, const Job& b) {
                if (a.priority != b.priority)
                    return a.priority > b.priority;

                if (a.deadline != b.deadline)
                    return a.deadline < b.deadline;

                return a.id < b.id;
            });

        // At most n jobs can be scheduled, so no need for
        // more than n slots even if a deadline is very large.
        int n = static_cast<int>(jobs.size());
        int maxDeadline = 0;

        for (const Job& job : jobs)
            maxDeadline = max(
                maxDeadline, min(job.deadline, n)
            );

        vector<int> slot(maxDeadline + 1, -1);

        int scheduledCount = 0;
        long long totalPriority = 0;

        // Place each job in the latest free slot
        // no later than its deadline.
        for (int i = 0; i < n; i++) {
            int latest = min(jobs[i].deadline, maxDeadline);

            for (int s = latest; s >= 1; s--) {
                if (slot[s] == -1) {
                    slot[s] = i;
                    scheduledCount++;
                    totalPriority += jobs[i].priority;
                    break;
                }
            }
        }

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":\"Job Sequencing with Deadlines\",";
        cout << "\"job_count\":" << n << ",";
        cout << "\"scheduled_count\":" << scheduledCount << ",";
        cout << "\"unscheduled_count\":"
             << n - scheduledCount << ",";
        cout << "\"total_priority_value\":" << totalPriority << ",";
        cout << "\"schedule\":[";

        bool first = true;

        for (int s = 1; s <= maxDeadline; s++) {
            if (slot[s] == -1) continue;

            if (!first) cout << ",";
            first = false;

            const Job& job = jobs[slot[s]];

            cout << "{";
            cout << "\"slot\":" << s << ",";
            cout << "\"job_id\":" << jsonString(job.id) << ",";
            cout << "\"job_name\":" << jsonString(job.name) << ",";
            cout << "\"deadline\":" << job.deadline << ",";
            cout << "\"priority_value\":" << job.priority;
            cout << "}";
        }

        cout << "],\"unscheduled_jobs\":[";

        first = true;

        set<string> scheduledIds;

        for (int s = 1; s <= maxDeadline; s++) {
            if (slot[s] != -1)
                scheduledIds.insert(jobs[slot[s]].id);
        }

        for (const Job& job : jobs) {
            if (scheduledIds.count(job.id)) continue;

            if (!first) cout << ",";
            first = false;

            cout << "{";
            cout << "\"job_id\":" << jsonString(job.id) << ",";
            cout << "\"job_name\":" << jsonString(job.name);
            cout << "}";
        }

        cout << "],";
        cout << "\"time_complexity\":\"O(n log n + nD)\",";
        cout << "\"space_complexity\":\"O(n + D)\"";
        cout << "}\n";

        return 0;
    }
    catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
