#ifndef SOLUTION_PRINTER_HPP                      // prevent multiple inclusion
#define SOLUTION_PRINTER_HPP                      // define header guard

#include <vector> // include vector
#include <string> // include string
#include <fstream> // include fstream
#include <iostream> // include iostream
#include <iomanip> // include iomanip
#include <cmath> // include cmath
#include <sstream> // include sstream


class SolutionPrinter                             // declare class
{
public:

    void printIterative(                          // execute statement
        const std::vector<double> &x,             // execute statement
        const std::string         &methodName,    // declare variable
        bool                       converged,     // declare variable
        int                        iterations,    // declare variable
        double                     finalError,    // declare variable
        std::ofstream             &vecOut)        // handle file stream operation
    {
        std::string header;                       // declare variable
        std::ostringstream oss;                   // execute statement

        if (converged)                            // check condition
        {
            oss << methodName << " converged in " // stream input/output operation
                << iterations << " iterations"    // stream input/output operation
                << " (error = " << std::scientific  // stream input/output operation
                << std::setprecision(5) << finalError << ")";  // declare function
        }
        else                                      // fallback case
        {
            oss << methodName << " did NOT converge after "  // stream input/output operation
                << iterations << " iterations"    // stream input/output operation
                << " (error = " << std::scientific  // stream input/output operation
                << std::setprecision(5) << finalError << ")";  // declare function
        }
        header = oss.str();                       // assign value to variable

        std::cout << "\n" << header << "\n";      // check equality or comparison
        vecOut    << header << "\n";              // check equality or comparison

        bool diverged = false;                    // declare and assign variable
        for (double xi : x)                       // iterate over elements
            if (std::fabs(xi) > 1e9) { diverged = true; break; }  // check condition

        if (diverged)                             // check condition
        {
            std::string warn = "[WARNING] Values diverged — Gauss-Seidel recommended for this matrix.";  // declare and assign variable
            std::cout << warn << "\n";            // check equality or comparison
            vecOut    << warn << "\n";            // check equality or comparison
        }

        printSolution(x, vecOut, converged && !diverged);  // call function or method
    }

    void printDirect(                             // execute statement
        const std::vector<double> &x,             // execute statement
        const std::string         &methodName,    // declare variable
        std::ofstream             &vecOut)        // handle file stream operation
    {
        std::string header = methodName + " \xe2\x80\x94 Solution Vector";  // declare and assign variable
        std::cout << "\n" << header << "\n";      // check equality or comparison
        vecOut    << header << "\n";              // check equality or comparison
        printSolution(x, vecOut, true);           // call function or method
    }

private:

    void printSolution(                           // execute statement
        const std::vector<double> &x,             // execute statement
        std::ofstream             &vecOut,        // handle file stream operation
        bool                       safe_approx)   // declare variable
    {
        int n = (int)x.size();                    // declare and initialize object
        for (int i = 0; i < n; i++)               // iterate over elements
        {
            std::ostringstream oss;               // execute statement
            oss << "x" << (i+1)                   // stream input/output operation
                << " = " << std::fixed << std::setprecision(5) << x[i];  // declare function

            if (!safe_approx && std::fabs(x[i]) > 1e9)  // check condition
            {
                oss << " \xe2\x89\x88 [DIVERGED]";  // check equality or comparison
            }
            else                                  // fallback case
            {
                long long approx = (long long)std::round(x[i]);  // declare function
                oss << " \xe2\x89\x88 " << approx;  // check equality or comparison
            }

            std::cout << oss.str() << "\n";       // declare function
            vecOut    << oss.str() << "\n";       // check equality or comparison
        }
    }
};

#endif                                            // end header guard