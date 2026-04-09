#ifndef EIGEN_VALUE_HPP         // Guard against double inclusion
#define EIGEN_VALUE_HPP

#include "Matrix.hpp"               // Base matrix class
#include <string>                  // For file path handling
#include <vector>                  // For storing eigenvalue bounds
#include <iostream>                // Standard input/output

class EigenValue : public Matrix   // Class for eigenvalue estimation
{
public:

    EigenValue();                  // Default constructor
    EigenValue(int r, int c);      // Constructor with dimensions

    friend std::ostream& operator<<(std::ostream &out, const EigenValue &ev);  // Output operator

    void gerschgorin(const std::string &Afile);  // Estimate eigenvalues using Gerschgorin circles
};

#endif