#ifndef GAUSSIAN_ELIMINATION_HPP  // Guard against double inclusion
#define GAUSSIAN_ELIMINATION_HPP

#include "LinearOperation.hpp"         // Base class for linear solvers
#include <fstream>                     // File operations

class GaussianElimination : public LinearOperation  // Solver using Gaussian elimination
{
public:

    GaussianElimination();             // Default constructor
    GaussianElimination(int r, int c);  // Constructor with dimensions

    friend std::ostream& operator<<(std::ostream &out, const GaussianElimination &ge);  // Output operator

    void gaussianWithPivoting(         // Solve using partial pivoting for stability
        Matrix &matrix,                // Input matrix (will be modified)
        std::ofstream &matrixOut,      // Output for intermediate matrix
        std::ofstream &vectorOut);     // Output for solution vector

    void gaussianWithoutPivoting(      // Solve without pivoting (simpler but less stable)
        Matrix &matrix,                // Input matrix (will be modified)
        std::ofstream &matrixOut,      // Output for intermediate matrix
        std::ofstream &vectorOut);     // Output for solution vector
};

#endif