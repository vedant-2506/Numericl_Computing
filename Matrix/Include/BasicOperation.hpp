#ifndef BASIC_OPERATION_HPP // prevent multiple include
#define BASIC_OPERATION_HPP // start header guard

#include "Matrix.hpp" // include base matrix class
#include <cmath> // math functions

class BasicOperation : public Matrix // inherit Matrix class
{
public: // public members

    BasicOperation(); // default constructor

    BasicOperation(int r, int c); // parameter constructor

    BasicOperation add(const BasicOperation &other) const; // matrix addition

    BasicOperation subtract(const BasicOperation &other) const; // matrix subtraction

    BasicOperation multiply(const BasicOperation &other) const; // matrix multiplication

    BasicOperation scalarDivide(double value) const; // divide by scalar

    BasicOperation transpose() const; // transpose matrix

    double determinant() const; // calculate determinant

    void displayToFile(ofstream &fout) const; // write matrix to file
};

#endif // end header guard