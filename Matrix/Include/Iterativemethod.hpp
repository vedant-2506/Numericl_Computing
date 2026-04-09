#ifndef ITERATIVE_METHOD_HPP   // Guard against double inclusion
#define ITERATIVE_METHOD_HPP

#include "LinearOperation.hpp"      // Base class for linear solvers
#include <string>                   // For file path handling
#include <iostream>                 // Standard input/output

class IterativeMethod : public LinearOperation  // Iterative solvers (Jacobi, Seidel)
{
public:
    IterativeMethod();               // Default constructor
    IterativeMethod(int r, int c);   // Constructor with dimensions

    friend std::ostream& operator<<(std::ostream &out, const IterativeMethod &im);  // Output operator

    void gaussJacobi(                // Gauss-Jacobi iteration method
        const std::string &Afile,    // File with coefficient matrix A
        const std::string &bfile,    // File with constant vector b
        int    maxIter,              // Maximum number of iterations
        double tol);                 // Convergence tolerance

    void gaussSeidel(                // Gauss-Seidel iteration method (faster)
        const std::string &Afile,    // File with coefficient matrix A
        const std::string &bfile,    // File with constant vector b
        int    maxIter,              // Maximum number of iterations
        double tol);                 // Convergence tolerance
};

#endif