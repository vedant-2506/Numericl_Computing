#ifndef LU_DECOMPOSITION_HPP   // Guard against double inclusion
#define LU_DECOMPOSITION_HPP

#include "LinearOperation.hpp"        // Base linear operation class
#include <string>                     // For file path handling
#include <iostream>                   // Standard input/output

class LUDecomposition : public LinearOperation  // Decompose matrix into L and U
{
public:

    LUDecomposition();                // Default constructor
    LUDecomposition(int r, int c);    // Constructor with dimensions

    void doolittleLU(                 // Doolittle method: L has 1s on diagonal
        const std::string &Afile,     // File containing coefficient matrix
        const std::string &bfile);    // File containing constant vector

    void croutLU(                     // Crout method: U has 1s on diagonal
        const std::string &Afile,     // File containing coefficient matrix
        const std::string &bfile);    // File containing constant vector

    void choleskyDecomposition(       // For symmetric positive-definite matrices
        const std::string &Afile,     // File containing coefficient matrix
        const std::string &bfile);    // File containing constant vector

    friend std::ostream& operator<<(std::ostream &out, const LUDecomposition &lu);  // Output operator
};

#endif