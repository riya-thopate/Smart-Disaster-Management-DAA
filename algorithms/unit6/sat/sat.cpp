#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool evaluateFormula(
    bool A,
    bool B,
    bool C
) {
    // Formula:
    //
    // (A OR B)
    // AND
    // (NOT A OR C)
    // AND
    // (B OR C)

    bool clause1 = A || B;

    bool clause2 = (!A) || C;

    bool clause3 = B || C;

    return clause1 &&
           clause2 &&
           clause3;
}

int main() {

    vector<string> variables = {
        "Water Available",
        "Food Available",
        "Medicine Available"
    };

    cout << "========================================" << endl;
    cout << "              SAT PROBLEM" << endl;
    cout << "       BOOLEAN SATISFIABILITY" << endl;
    cout << "========================================" << endl;

    cout << "\nBoolean variables:" << endl;

    cout << "A = Water Available" << endl;
    cout << "B = Food Available" << endl;
    cout << "C = Medicine Available" << endl;

    cout << "\nBoolean Formula:" << endl;

    cout << "(A OR B) AND (!A OR C) AND (B OR C)"
         << endl;

    cout << "\n----------------------------------------" << endl;

    bool foundSolution = false;

    // Try all possible assignments
    // for A, B and C.
    for (int a = 0; a <= 1; a++) {

        for (int b = 0; b <= 1; b++) {

            for (int c = 0; c <= 1; c++) {

                bool A = a;
                bool B = b;
                bool C = c;

                if (evaluateFormula(
                        A,
                        B,
                        C)) {

                    cout << "Satisfying assignment found:"
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

        cout << "Result: SATISFIABLE"
             << endl;

        cout << "\nThe Boolean formula has at least"
             << endl;

        cout << "one assignment that makes the"
             << endl;

        cout << "entire formula TRUE."
             << endl;
    }
    else {

        cout << "Result: UNSATISFIABLE"
             << endl;

        cout << "\nNo Boolean assignment satisfies"
             << endl;

        cout << "the entire formula."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "SAT determines whether there exists"
         << endl;

    cout << "an assignment of Boolean values that"
         << endl;

    cout << "makes a given logical formula TRUE."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "SAT demonstration completed successfully."
         << endl;

    return 0;
}