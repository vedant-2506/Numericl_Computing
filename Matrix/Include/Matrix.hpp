#ifndef MATRIX_HPP                                    // prevent multiple inclusion
#define MATRIX_HPP                                    // define header guard

#include <vector>                                     // for std::vector
#include <fstream>                                    // for std::ifstream, std::ofstream
#include <iostream>                                   // for std::istream, std::ostream

class Matrix                                          // base class for all matrix types
{
protected:                                            // accessible by child classes
    int rows;                                         // number of rows
    int cols;                                         // number of columns
    std::vector<std::vector<double>> data;            // 2D matrix data storage

public:                                               // accessible by everyone

    // constructors
    Matrix();                                         // default constructor
    Matrix(int r, int c);                             // parameterized constructor
    Matrix(const Matrix &m);                          // copy constructor

    // file I/O
    void readFromFile(std::ifstream &fin);            // read matrix from file
    void displayToFile(std::ofstream &fout) const;   // write matrix to file
    void display() const;                             // print matrix to console

    // arithmetic operators
    Matrix operator+(const Matrix &m) const;          // matrix addition
    Matrix operator-(const Matrix &m) const;          // matrix subtraction
    Matrix operator*(const Matrix &m) const;          // matrix multiplication

    // element access operators
    double& operator()(int i, int j);                 // read/write element at (i,j)
    double  operator()(int i, int j) const;           // read-only element at (i,j)

    // comparison operator
    bool operator==(const Matrix &m) const;           // equality check

    // stream friend operators
    friend std::istream& operator>>(std::istream &in,  Matrix &m);       // read from stream
    friend std::ostream& operator<<(std::ostream &out, const Matrix &m); // write to stream

    // getters and setters
    int    getRows() const;                           // return row count
    int    getCols() const;                           // return column count
    double get(int i, int j) const;                   // get element at (i,j)
    void   set(int i, int j, double v);               // set element at (i,j)
    std::vector<std::vector<double>>& getData();      // return data reference

    // matrix property checks
    bool isSquare()             const;                // check if rows == cols
    bool isSymmetric()          const;                // check if A == A^T
    bool isIdentity()           const;                // check if identity matrix
    bool isNull()               const;                // check if all elements zero
    bool isDiagonal()           const;                // check if diagonal matrix
    bool isDiagonallyDominant() const;                // check diagonal dominance
    bool isTranspose(const Matrix &m) const;          // check if this == m^T

    // matrix transformations
    void   makeDiagonallyDominant();                  // reorder rows for dominance
    Matrix transpose()   const;                       // ← added back
    double determinant() const;                       // ← added back
    Matrix inverse()     const;                       // compute inverse matrix
};

#endif                                                // end header guard