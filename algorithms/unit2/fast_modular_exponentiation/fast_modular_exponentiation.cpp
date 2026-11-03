#include <iostream>

using namespace std;


// --------------------------------------------------
// Fast Modular Exponentiation
// Calculates (base ^ exponent) % modulus
// --------------------------------------------------

long long fastModularExponentiation(
    long long base,
    long long exponent,
    long long modulus) {

    long long result = 1;

    base = base % modulus;


    while (exponent > 0) {

        // If exponent is odd
        if (exponent % 2 == 1) {

            result =
                (result * base) % modulus;
        }


        // Square the base
        base =
            (base * base) % modulus;


        // Divide exponent by 2
        exponent =
            exponent / 2;
    }


    return result;
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    long long base = 7;
    long long exponent = 20;
    long long modulus = 1000;


    cout << "========================================"
         << endl;

    cout << "      FAST MODULAR EXPONENTIATION"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nCalculation:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    cout << base
         << "^"
         << exponent
         << " mod "
         << modulus
         << endl;


    long long result =
        fastModularExponentiation(
            base,
            exponent,
            modulus
        );


    cout << "\n----------------------------------------"
         << endl;

    cout << "Result: "
         << result
         << endl;


    cout << "\nInterpretation:"
         << endl;

    cout << "The calculation can be performed"
         << endl;

    cout << "efficiently using repeated squaring."
         << endl;


    cout << "\n----------------------------------------"
         << endl;

    cout << "Fast Modular Exponentiation completed successfully."
         << endl;


    return 0;
}