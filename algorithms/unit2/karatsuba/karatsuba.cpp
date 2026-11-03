#include <iostream>
#include <string>
#include <cmath>

using namespace std;


// --------------------------------------------------
// Karatsuba Multiplication
// --------------------------------------------------

long long karatsuba(long long x, long long y) {

    // Base case
    if (x < 10 || y < 10) {
        return x * y;
    }


    // Find number of digits
    long long maxNumber = max(x, y);

    int digits = to_string(maxNumber).length();

    int half = digits / 2;


    long long power = pow(10, half);


    // Split x into two parts
    long long highX = x / power;
    long long lowX = x % power;


    // Split y into two parts
    long long highY = y / power;
    long long lowY = y % power;


    // Three recursive multiplications
    long long z0 = karatsuba(lowX, lowY);

    long long z1 =
        karatsuba(
            lowX + highX,
            lowY + highY
        );

    long long z2 =
        karatsuba(highX, highY);


    // Combine the results
    return
        z2 * power * power
        + (z1 - z2 - z0) * power
        + z0;
}


// --------------------------------------------------
// Main
// --------------------------------------------------

int main() {

    long long number1 = 1234;
    long long number2 = 5678;


    cout << "========================================"
         << endl;

    cout << "       KARATSUBA MULTIPLICATION"
         << endl;

    cout << "========================================"
         << endl;


    cout << "\nLarge Resource Identifier Values:"
         << endl;

    cout << "----------------------------------------"
         << endl;

    cout << "Number 1: "
         << number1
         << endl;

    cout << "Number 2: "
         << number2
         << endl;


    long long result =
        karatsuba(number1, number2);


    cout << "\n----------------------------------------"
         << endl;

    cout << "Karatsuba Result:"
         << endl;

    cout << result
         << endl;


    cout << "\n----------------------------------------"
         << endl;

    cout << "Karatsuba Multiplication completed successfully."
         << endl;


    return 0;
}