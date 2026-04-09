#ifndef BASIC_OPERATION_HPP    // Guard against double inclusion
#define BASIC_OPERATION_HPP

#include "Matrix.hpp"                 // Base matrix class
#include <iostream>                   // Input/output stream

class BasicOperation : public Matrix  // Derived class for basic matrix operations
{
public:

    BasicOperation();                 // Default constructor
    BasicOperation(int r, int c);     // Constructor with dimensions

    BasicOperation add(const BasicOperation &o)      const;  // Add two matrices
    BasicOperation subtract(const BasicOperation &o) const;  // Subtract two matrices
    BasicOperation multiply(const BasicOperation &o) const;  // Multiply two matrices
    BasicOperation scalarDivide(double v)            const;  // Divide all elements by scalar
    BasicOperation scalarMultiply(double v)          const;  // Multiply all elements by scalar

    BasicOperation transpose()   const;  // Flip matrix rows and columns
    double         determinant() const;  // Calculate determinant value

    friend std::ostream& operator<<(std::ostream &out, const BasicOperation &b);  // Overload output operator
};

#endif