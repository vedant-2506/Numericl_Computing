#ifndef MATRIX_HPP // prevent multiple include
#define MATRIX_HPP // start header guard

#include <vector> // vector container
#include <fstream> // file stream
#include <iostream> // input output

using namespace std; // use standard namespace

class Matrix // base matrix class
{
protected: // protected members

    int rows; // number of rows

    int cols; // number of columns

    vector<vector<double>> data; // matrix data storage

public: // public members

    Matrix(); // default constructor

    Matrix(int r,int c); // parameter constructor

    void set(int i,int j,double value); // set matrix value

    double get(int i,int j) const; // get matrix value

    int getRows() const; // return rows

    int getCols() const; // return columns

    vector<vector<double>>& getData(); // return matrix data

    void readFromFile(ifstream &fin); // read matrix from file

    void display() const; // display matrix
};

#endif // end header guard