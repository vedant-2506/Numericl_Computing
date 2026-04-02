#ifndef BASIC_OPERATION_HPP                       // prevent multiple inclusion
#define BASIC_OPERATION_HPP                       // define header guard

#include "Matrix.hpp"                    // include base Matrix class
#include <iostream>                      // for std::ostream

class BasicOperation : public Matrix              // declare class
{
public:

    BasicOperation();                             // call function or method
    BasicOperation(int r, int c);                 // declare and initialize object

    BasicOperation add(const BasicOperation &o)      const;  // call function or method
    BasicOperation subtract(const BasicOperation &o) const;  // call function or method
    BasicOperation multiply(const BasicOperation &o) const;  // call function or method
    BasicOperation scalarDivide(double v)            const;  // declare and initialize object
    BasicOperation scalarMultiply(double v)          const;  // declare and initialize object

    BasicOperation transpose()   const;           // call function or method
    double         determinant() const;           // declare and initialize object

    friend std::ostream& operator<<(std::ostream &out, const BasicOperation &b);  // declare function
};

#endif                                            // end header guard