#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Matrix
{
private:
    int rows, cols;
    vector<vector<double>> mat;

public:
    // Constructors
    Matrix();
    Matrix(int rows, int cols);
    Matrix(const Matrix &m);

    // File handling
    void readFromFile(ifstream &fin);
    void displayToFile(ofstream &fout) const;

    // Matrix operations
    Matrix add(const Matrix &m) const;
    Matrix subtract(const Matrix &m) const;

    // Gaussian elimination with Partial Pivoting
    void gaussianEliminationWithPivoting(ofstream &matrixOut,
                                         ofstream &solutionOut);
};

#endif