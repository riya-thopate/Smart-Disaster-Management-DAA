#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Task {
    string id;
    string name;
    int deadline;
    int profit;
};

int main() {

    vector<Task> tasks = {

        {"T1", "Rescue Critical Zone", 2, 100},
        {"T2", "Deliver Medicine", 1, 80},
        {"T3", "Evacuate Market Area", 2, 60},
        {"T4", "Supply Food", 3, 40},
        {"T5", "Set Up Shelter", 3, 70},
        {"T6", "Distribute Water", 4, 50}
    };

    // Sort tasks by decreasing profit
    sort(
        tasks.begin(),
        tasks.end(),
        [](const Task& a, const Task& b) {
            return a.profit > b.profit;
        }
    );

    int maxDeadline = 0;

    for (const Task& task : tasks) {
        maxDeadline =
            max(maxDeadline, task.deadline);
    }

    // Each slot can contain one task
    vector<int> slot(maxDeadline + 1, -1);

    int totalProfit = 0;
    int scheduledTasks = 0;

    // Greedy scheduling
    for (int i = 0; i < tasks.size(); i++) {

        // Find the latest free slot
        // before or on the deadline
        for (int j = tasks[i].deadline;
             j >= 1;
             j--) {

            if (slot[j] == -1) {

                slot[j] = i;

                totalProfit +=
                    tasks[i].profit;

                scheduledTasks++;

                break;
            }
        }
    }

    cout << "========================================" << endl;
    cout << "        EMERGENCY TASK SCHEDULING" << endl;
    cout << "            GREEDY ALGORITHM" << endl;
    cout << "========================================" << endl;

    cout << "\nEmergency-response tasks:" << endl;

    for (const Task& task : tasks) {

        cout << task.id
             << " | "
             << task.name
             << " | Deadline: "
             << task.deadline
             << " | Priority: "
             << task.profit
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Scheduled task order:" << endl;

    for (int i = 1; i <= maxDeadline; i++) {

        cout << "Slot "
             << i
             << ": ";

        if (slot[i] == -1) {
            cout << "Free";
        }
        else {
            cout << tasks[slot[i]].id
                 << " - "
                 << tasks[slot[i]].name;
        }

        cout << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Total scheduled tasks = "
         << scheduledTasks
         << endl;

    cout << "Total priority value = "
         << totalProfit
         << endl;

    cout << "\nInterpretation:" << endl;

    cout << "The greedy scheduling algorithm selects"
         << endl;

    cout << "high-priority emergency tasks while"
         << endl;

    cout << "respecting their deadlines."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Emergency Task Scheduling completed successfully."
         << endl;

    return 0;
}