#ifndef LINEAR_OPERATION_HPP   // Guard against double inclusion
#define LINEAR_OPERATION_HPP

#include "Matrix.hpp"                // Base matrix class
#include <string>                   // For file path handling
#include <iostream>                 // Standard input/output

class LinearOperation : public Matrix  // Base class for linear system solvers
{
public:

    LinearOperation();               // Default constructor
    LinearOperation(int r, int c);   // Constructor with dimensions

    void generateAugmentedMatrixFile(  // Create augmented matrix [A|b] from separate files
        const std::string &leftFile,   // File with coefficient matrix A
        const std::string &rightFile,  // File with constant vector b
        const std::string &outputFile); // Output file for augmented matrix

    friend std::ostream& operator<<(std::ostream &out, const LinearOperation &lo);  // Output operator
};

#endif