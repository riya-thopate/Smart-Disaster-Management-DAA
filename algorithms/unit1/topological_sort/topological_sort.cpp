#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

class Graph {

private:
    int vertices;
    vector<vector<int>> adjList;
    vector<int> indegree;
    vector<string> tasks;

public:

    Graph(int v) {

        vertices = v;

        adjList.resize(vertices);
        indegree.resize(vertices, 0);
        tasks.resize(vertices);
    }

    void addTask(int index, string task) {

        tasks[index] = task;
    }

    void addDependency(int before, int after) {

        // before must be completed before after
        adjList[before].push_back(after);

        indegree[after]++;
    }

    void topologicalSort() {

        queue<int> q;

        // Add tasks having zero dependencies
        for (int i = 0; i < vertices; i++) {

            if (indegree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> order;

        while (!q.empty()) {

            int current = q.front();
            q.pop();

            order.push_back(current);

            // Remove current task's dependency
            for (int next : adjList[current]) {

                indegree[next]--;

                if (indegree[next] == 0) {
                    q.push(next);
                }
            }
        }

        cout << "\n========================================" << endl;
        cout << "   DISASTER RESPONSE TASK SCHEDULING" << endl;
        cout << "========================================" << endl;

        cout << "\nValid Task Execution Order:" << endl;
        cout << "----------------------------------------" << endl;

        if (order.size() != vertices) {

            cout << "Cycle detected!" << endl;
            cout << "Topological sorting is not possible." << endl;

            return;
        }

        for (int i = 0; i < order.size(); i++) {

            cout << i + 1 << ". "
                 << tasks[order[i]]
                 << endl;
        }

        cout << "\n----------------------------------------" << endl;

        cout << "All disaster-response tasks can be completed "
             << "in the above dependency order." << endl;
    }
};


int main() {

    // ----------------------------------------
    // Create Disaster Response Task Graph
    // ----------------------------------------

    Graph disaster(7);

    // Tasks

    disaster.addTask(0, "Detect Emergency");
    disaster.addTask(1, "Assess Damage");
    disaster.addTask(2, "Dispatch Rescue Team");
    disaster.addTask(3, "Allocate Resources");
    disaster.addTask(4, "Open Evacuation Route");
    disaster.addTask(5, "Evacuate People");
    disaster.addTask(6, "Move People to Shelter");


    // ----------------------------------------
    // Task Dependencies
    // ----------------------------------------

    // Detect Emergency -> Assess Damage
    disaster.addDependency(0, 1);

    // Assess Damage -> Dispatch Rescue Team
    disaster.addDependency(1, 2);

    // Assess Damage -> Allocate Resources
    disaster.addDependency(1, 3);

    // Dispatch Rescue Team -> Open Evacuation Route
    disaster.addDependency(2, 4);

    // Allocate Resources -> Evacuate People
    disaster.addDependency(3, 5);

    // Open Evacuation Route -> Evacuate People
    disaster.addDependency(4, 5);

    // Evacuate People -> Move People to Shelter
    disaster.addDependency(5, 6);


    // ----------------------------------------
    // Perform Topological Sort
    // ----------------------------------------

    disaster.topologicalSort();

    return 0;
}