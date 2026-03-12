#ifndef BASIC_OPERATION_HPP              // prevent multiple inclusion
#define BASIC_OPERATION_HPP              // define header guard

#include "Matrix.hpp"                    // include base Matrix class
#include <iostream>                      // for std::ostream

class BasicOperation : public Matrix     // inherits all Matrix methods and operators
{
public:                                  // accessible by everyone

    // constructors
    BasicOperation();                    // default constructor
    BasicOperation(int r, int c);        // parameterized constructor

    // named operation methods
    BasicOperation add(const BasicOperation &o)      const; // named addition
    BasicOperation subtract(const BasicOperation &o) const; // named subtraction
    BasicOperation multiply(const BasicOperation &o) const; // named multiplication
    BasicOperation scalarDivide(double v)            const; // divide all by scalar
    BasicOperation scalarMultiply(double v)          const; // multiply all by scalar

    // basic operation specific methods
    BasicOperation transpose()   const;  // transpose -> returns BasicOperation
    double         determinant() const;  // determinant -> returns double

    // friend stream operator
    friend std::ostream& operator<<(std::ostream &out, const BasicOperation &b);
};

#endif                                   // end header guard