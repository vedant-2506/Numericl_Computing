// Header guard start
// Prevents multiple inclusion of this header file
#ifndef ROOT_HPP
#define ROOT_HPP

// Class definition for Bisection Method
class bisect
{
public:
    // Static variables are shared among all calls
    // 'a' and 'b' store the left and right interval values
    static double a, b;

    // Tolerance value for stopping condition
    static double tol;

    // Function representing the mathematical equation f(x)
    // Example: f(x) = 4x^3 - 3x
    static double f(double x);

    // Function to automatically find an interval [a, b]
    // where f(a) and f(b) have opposite signs
    static int findInterval();

    // Function that applies the bisection method
    // and returns the approximate root
    static double solve();
};

// End of header guard
#endif
