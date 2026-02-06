// Include the header file from Include folder
#include "Root.hpp"

// Include iostream for input-output operations
#include <iostream>

// Include cmath for mathematical functions like abs()
#include <cmath>

// Definition of static class variables
double bisect::a;                 // Left endpoint of interval
double bisect::b;                 // Right endpoint of interval
double bisect::tol = 0.0001;      // Tolerance value for accuracy

// Definition of the mathematical function f(x)
// This is the equation whose root we want to find
// f(x) = 4x^3 - 3x
double bisect::f(double x)
{
    // Return the calculated value of the equation
    return 4 * x * x * x - 3 * x;
}

// Function to find a valid interval automatically
// A valid interval is where f(left) * f(right) < 0
int bisect::findInterval()
{
    // Step size used to scan the x-axis
    double step = 0.05;

    // Loop through values from -5 to +5 approximately
    for (int i = -100; i < 100; i++)
    {
        // Calculate left point of interval
        double left = i * step;

        // Calculate right point of interval
        double right = left + step;

        // Check if function values have opposite signs
        if (f(left) * f(right) < 0)
        {
            // Store the valid interval
            a = left;
            b = right;

            // Interval found successfully
            return 1;
        }
    }

    // No valid interval found
    return 0;
}

// Function that applies the Bisection Method
double bisect::solve()
{
    // First, find a valid interval automatically
    if (!findInterval())
    {
        // If interval is not found, print error message
        std::cout << "Could not find a valid interval\n";
        return 0;
    }

    // Display the starting interval
    std::cout << "Starting interval: [" << a << ", " << b << "]\n";

    // Display tolerance value
    std::cout << "Tolerance: " << tol << "\n\n";

    // Print table header for iterations
    std::cout << "Iter\t a\t\t b\t\t m\t\t f(m)\n";

    // Previous midpoint value (used for convergence check)
    double m_prev = a;

    // Current midpoint value
    double m_curr;

    // Iteration counter
    int iter = 1;

    // Infinite loop (will break when tolerance condition is satisfied)
    while (true)
    {
        // Calculate midpoint of interval
        m_curr = (a + b) / 2;

        // Calculate function value at midpoint
        double fm = f(m_curr);

        // Print current iteration values
        std::cout << iter << "\t "
                  << a << "\t "
                  << b << "\t "
                  << m_curr << "\t "
                  << fm << std::endl;

        // Check stopping condition using tolerance
        if (std::abs(m_curr - m_prev) < tol)
            break;

        // Decide the new interval
        if (f(a) * fm < 0)
            b = m_curr;     // Root lies in left sub-interval
        else
            a = m_curr;     // Root lies in right sub-interval

        // Update previous midpoint
        m_prev = m_curr;

        // Increment iteration counter
        iter++;
    }

    // Return the final approximate root
    return m_curr;
}
