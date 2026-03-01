#ifndef BASIC_OPERATION_HPP
#define BASIC_OPERATION_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cmath>

using namespace std;

class BasicOperation
{
private:
    int rows, cols;                         // store matrix dimensions
    vector<vector<double>> mat;            // 2D vector to store matrix data

public:
    BasicOperation();                       // default constructor
    BasicOperation(int r, int c);           // parameterized constructor

    void readFromFile(ifstream &fin);       // read matrix from file
    void displayToFile(ofstream &fout) const; // display matrix to file

    // Basic Matrix Operations
    BasicOperation add(const BasicOperation &other) const;      // addition
    BasicOperation subtract(const BasicOperation &other) const; // subtraction
    BasicOperation multiply(const BasicOperation &other) const; // multiplication
    BasicOperation scalarDivide(double value) const;            // scalar division
    BasicOperation transpose() const;                           // transpose
    double determinant() const;                                 // determinant

    int getRows() const;   // getter
    int getCols() const;   // getter
    vector<vector<double>> getMatrix() const; // getter
};

#endif