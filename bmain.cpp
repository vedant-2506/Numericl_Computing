// Include the header file where the bisect class is declared
// The path must exactly match the folder name (Linux is case-sensitive)
#include "Root.hpp"

// Include iostream to use cout and endl
#include <iostream>

int main()
{
    // Call the static solve() function of bisect class
    // This function will:
    // 1) Automatically find a valid interval
    // 2) Apply the bisection method
    // 3) Return the approximate root
    double root = bisect::solve();

    // Print the final approximate root on the screen
    std::cout << "\nApproximate Root = " << root << std::endl;

    // Return 0 to indicate successful program execution
    return 0;
}
