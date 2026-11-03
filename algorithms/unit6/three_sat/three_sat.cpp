#include <iostream>

using namespace std;

// Evaluate the 3-CNF formula:
//
// (A OR B OR C)
// AND
// (!A OR B OR !C)
// AND
// (A OR !B OR C)

bool evaluateFormula(
    bool A,
    bool B,
    bool C
) {
    bool clause1 =
        A || B || C;

    bool clause2 =
        (!A) || B || (!C);

    bool clause3 =
        A || (!B) || C;

    return clause1 &&
           clause2 &&
           clause3;
}

int main() {

    cout << "========================================" << endl;
    cout << "             3-CNF SAT / 3-SAT" << endl;
    cout << "         BOOLEAN SATISFIABILITY" << endl;
    cout << "========================================" << endl;

    cout << "\nBoolean variables:" << endl;

    cout << "A = Water Available" << endl;
    cout << "B = Food Available" << endl;
    cout << "C = Medicine Available" << endl;

    cout << "\n3-CNF Formula:" << endl;

    cout << "(A OR B OR C)" << endl;
    cout << "AND (!A OR B OR !C)" << endl;
    cout << "AND (A OR !B OR C)" << endl;

    cout << "\n----------------------------------------" << endl;

    bool foundSolution = false;

    // Test all 2^3 = 8 assignments
    for (int a = 0; a <= 1; a++) {

        for (int b = 0; b <= 1; b++) {

            for (int c = 0; c <= 1; c++) {

                bool A = a;
                bool B = b;
                bool C = c;

                bool clause1 =
                    A || B || C;

                bool clause2 =
                    (!A) || B || (!C);

                bool clause3 =
                    A || (!B) || C;

                bool result =
                    clause1 &&
                    clause2 &&
                    clause3;

                if (result) {

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

                    cout << "\nClause 1 = "
                         << (clause1 ? "TRUE" : "FALSE")
                         << endl;

                    cout << "Clause 2 = "
                         << (clause2 ? "TRUE" : "FALSE")
                         << endl;

                    cout << "Clause 3 = "
                         << (clause3 ? "TRUE" : "FALSE")
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

        cout << "Result: 3-CNF FORMULA IS SATISFIABLE"
             << endl;

        cout << "\nAll three clauses evaluate to TRUE"
             << endl;

        cout << "for at least one Boolean assignment."
             << endl;
    }
    else {

        cout << "Result: 3-CNF FORMULA IS UNSATISFIABLE"
             << endl;

        cout << "\nNo Boolean assignment satisfies"
             << endl;

        cout << "all three clauses."
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "3-SAT determines whether a Boolean"
         << endl;

    cout << "formula containing exactly three"
         << endl;

    cout << "literals per clause has a satisfying"
         << endl;

    cout << "assignment."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "3-CNF SAT / 3-SAT demonstration completed successfully."
         << endl;

    return 0;
}