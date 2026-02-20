#ifndef GAUSSIAN_HPP
#define GAUSSIAN_HPP

#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

class Matrix
{
private:
    int rowCount;     // number of rows
    int colCount;     // number of columns
    vector<vector<double>> data;   // 2D dynamic storage

public:
    Matrix();                      // default constructor
    Matrix(int r, int c);          // parameterized constructor
    Matrix(const Matrix &obj);     // copy constructor

    void inputData();              // read matrix
    void printData();              // display matrix

    Matrix addMatrix(const Matrix &obj);     // addition
    Matrix subMatrix(const Matrix &obj);     // subtraction

    void performGaussian();        // Gaussian elimination
    void solveBack();              // back substitution to
};

#endif
