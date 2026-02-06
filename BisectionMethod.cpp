#include <iostream>
#include <cmath>
using namespace std;

// Define the function
double f(double x) {
    return x * x - 2;
}

int main() {
    double a = 1.0, b = 2.0;
    double m_prev = 0.0, m_curr;
    int iteration = 0;
    int k;

    cout << "Enter value of k (tolerance = 10^-k): ";
    cin >> k;

    double tolerance = pow(10, -k);

    // Check initial condition
    if (f(a) * f(b) >= 0) {
        cout << "Bisection method not applicable!" << endl;
        return 0;
    }

    do {
        m_curr = (a + b) / 2.0;
        iteration++;

        if (f(a) * f(m_curr) < 0)
            b = m_curr;
        else
            a = m_curr;

        // Print iteration details
        cout << "Iteration " << iteration
             << " : m = " << m_curr << endl;

        // Stop condition
        if (iteration > 1 && fabs(m_curr - m_prev) < tolerance)
            break;

        m_prev = m_curr;

    } while (true);

    cout << "\nApproximate Root = " << m_curr << endl;
    cout << "Total Iterations = " << iteration << endl;

    return 0;
}
