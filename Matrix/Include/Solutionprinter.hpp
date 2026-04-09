#ifndef SOLUTION_PRINTER_HPP
#define SOLUTION_PRINTER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>


class SolutionPrinter
{
public:

    void printIterative(
        const std::vector<double> &x,
        const std::string         &methodName,
        bool                       converged,
        int                        iterations,
        double                     finalError,
        std::ofstream             &vecOut)
    {
        std::string header;
        std::ostringstream oss;

        if (converged)
        {
            oss << methodName << " converged in "
                << iterations << " iterations"
                << " (error = " << std::scientific
                << std::setprecision(5) << finalError << ")";
        }
        else
        {
            oss << methodName << " did NOT converge after "
                << iterations << " iterations"
                << " (error = " << std::scientific
                << std::setprecision(5) << finalError << ")";
        }
        header = oss.str();

        std::cout << "\n" << header << "\n";
        vecOut    << header << "\n";

        bool diverged = false;
        for (double xi : x)
            if (std::fabs(xi) > 1e9) { diverged = true; break; }

        if (diverged)
        {
            std::string warn = "[WARNING] Values diverged — Gauss-Seidel recommended for this matrix.";
            std::cout << warn << "\n";
            vecOut    << warn << "\n";
        }

        printSolution(x, vecOut, converged && !diverged);
    }

    void printDirect(
        const std::vector<double> &x,
        const std::string         &methodName,
        std::ofstream             &vecOut)
    {
        std::string header = methodName + " \xe2\x80\x94 Solution Vector";
        std::cout << "\n" << header << "\n";
        vecOut    << header << "\n";
        printSolution(x, vecOut, true);
    }

private:

    void printSolution(
        const std::vector<double> &x,
        std::ofstream             &vecOut,
        bool                       safe_approx)
    {
        int n = (int)x.size();
        for (int i = 0; i < n; i++)
        {
            std::ostringstream oss;
            oss << "x" << (i+1)
                << " = " << std::fixed << std::setprecision(5) << x[i];

            if (!safe_approx && std::fabs(x[i]) > 1e9)
            {
                oss << " \xe2\x89\x88 [DIVERGED]";
            }
            else
            {
                long long approx = (long long)std::round(x[i]);
                oss << " \xe2\x89\x88 " << approx;
            }

            std::cout << oss.str() << "\n";
            vecOut    << oss.str() << "\n";
        }
    }
};

#endif