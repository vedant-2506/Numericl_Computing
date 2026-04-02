#ifndef MATRIX_HPP                                // prevent multiple inclusion
#define MATRIX_HPP                                // define header guard

#include <vector>                                     // for std::vector
#include <fstream>                                    // for std::ifstream, std::ofstream
#include <iostream>                                   // for std::istream, std::ostream

class Matrix                                      // declare class
{
protected:
    int rows;                                     // declare variable
    int cols;                                     // declare variable
    std::vector<std::vector<double>> data;        // check equality or comparison

public:

    Matrix();                                     // call function or method
    Matrix(int r, int c);                         // declare and initialize object
    Matrix(const Matrix &m);                      // call function or method

    void readFromFile(std::ifstream &fin);        // handle file stream operation
    void displayToFile(std::ofstream &fout) const;  // declare function
    void display() const;                         // call function or method

    Matrix operator+(const Matrix &m) const;      // call function or method
    Matrix operator-(const Matrix &m) const;      // call function or method
    Matrix operator*(const Matrix &m) const;      // call function or method

    double& operator()(int i, int j);             // declare and initialize object
    double  operator()(int i, int j) const;       // declare and initialize object

    bool operator==(const Matrix &m) const;       // declare and initialize object

    friend std::istream& operator>>(std::istream &in,  Matrix &m);  // declare function
    friend std::ostream& operator<<(std::ostream &out, const Matrix &m);  // declare function

    int    getRows() const;                       // declare and initialize object
    int    getCols() const;                       // declare and initialize object
    double get(int i, int j) const;               // declare and initialize object
    void   set(int i, int j, double v);           // declare and initialize object
    std::vector<std::vector<double>>& getData();  // declare function

    bool isSquare()             const;            // declare and initialize object
    bool isSymmetric()          const;            // declare and initialize object
    bool isIdentity()           const;            // declare and initialize object
    bool isNull()               const;            // declare and initialize object
    bool isDiagonal()           const;            // declare and initialize object
    bool isDiagonallyDominant() const;            // declare and initialize object
    bool isTranspose(const Matrix &m) const;      // declare and initialize object

    void   makeDiagonallyDominant();              // call function or method
    Matrix transpose()   const;                   // call function or method
    double determinant() const;                   // declare and initialize object
    Matrix inverse()     const;                   // call function or method
};

#endif                                            // end header guard