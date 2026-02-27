#ifndef GAUSSIAN_HPP
#define GAUSSIAN_HPP

#include <iostream>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

class Matrix {
public:
    int rows, cols;                        
    vector<vector<double>> mat;   

    Matrix();                    // Default constructor
    Matrix(int r, int c);        // Parameterized constructor
    Matrix(const Matrix &m);     // Copy constructor

    void readFromFile(ifstream &fin);          
    void displayToFile(ofstream &fout) const;  

    // Matrix Operations
    Matrix add(const Matrix &other) const;         // Addition
    Matrix subtract(const Matrix &other) const;    // Subtraction

    void gaussianEliminationWithPivoting(ofstream &matrixOut, 
                                         ofstream &vectorOut);    // Gaussian elimination with pivoting

    static void generateAugmentedMatrixFile(const string &leftFile,
                                            const string &rightFile,
                                            const string &outputFile);   // Generate augmented matrix 
};

#endif