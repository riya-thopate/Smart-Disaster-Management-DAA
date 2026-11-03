#include <iostream>

using namespace std;

// OR gate
bool ORGate(bool a, bool b) {
    return a || b;
}

// AND gate
bool ANDGate(bool a, bool b) {
    return a && b;
}

// Evaluate the complete circuit
bool evaluateCircuit(
    bool A,
    bool B,
    bool C
) {
    // First gate:
    // A OR B
    bool orOutput = ORGate(A, B);

    // Second gate:
    // (A OR B) AND C
    bool finalOutput =
        ANDGate(orOutput, C);

    return finalOutput;
}

int main() {

    cout << "========================================" << endl;
    cout << "            CIRCUIT-SAT" << endl;
    cout << "        BOOLEAN CIRCUIT PROBLEM" << endl;
    cout << "========================================" << endl;

    cout << "\nCircuit inputs:" << endl;

    cout << "A = Water Available" << endl;
    cout << "B = Food Available" << endl;
    cout << "C = Emergency Active" << endl;

    cout << "\nCircuit Formula:" << endl;

    cout << "(A OR B) AND C" << endl;

    cout << "\n----------------------------------------" << endl;

    bool foundSolution = false;

    // Test all 2^3 = 8 input combinations
    for (int a = 0; a <= 1; a++) {

        for (int b = 0; b <= 1; b++) {

            for (int c = 0; c <= 1; c++) {

                bool A = a;
                bool B = b;
                bool C = c;

                bool orOutput =
                    ORGate(A, B);

                bool finalOutput =
                    ANDGate(
                        orOutput,
                        C
                    );

                if (finalOutput) {

                    cout << "Satisfying input found:"
                         << endl;

                    cout << "A = "
                         << (A ? "TRUE" : "FALSE")
                         << endl;

                    cout << "B = "
                         << (B ? "TRUE" : "FALSE")
                         << endl;

                    cout << "C = "
                         << (C ? "TRUE" : "FALSE")
                         << endl;

                    cout << "\nOR Gate Output = "
                         << (orOutput ? "TRUE" : "FALSE")
                         << endl;

                    cout << "Final Circuit Output = "
                         << (finalOutput ? "TRUE" : "FALSE")
                         << endl;

                    foundSolution = true;

                    break;
                }
            }

            if (foundSolution) {
                break;
            }
        }

        if (foundSolution) {
            break;
        }
    }

    cout << "\n----------------------------------------" << endl;

    if (foundSolution) {

        cout << "Result: CIRCUIT SATISFIABLE"
             << endl;

        cout << "\nAt least one input assignment"
             << endl;

        cout << "makes the circuit output TRUE."
             << endl;
    }
    else {

        cout << "Result: CIRCUIT UNSATISFIABLE"
             << endl;

        cout << "\nNo input assignment makes"
             << endl;

        cout << "the circuit output TRUE."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "Circuit-SAT determines whether there"
         << endl;

    cout << "exists an input combination that"
         << endl;

    cout << "produces TRUE at the output of"
         << endl;

    cout << "a Boolean circuit."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Circuit-SAT demonstration completed successfully."
         << endl;

    return 0;
}