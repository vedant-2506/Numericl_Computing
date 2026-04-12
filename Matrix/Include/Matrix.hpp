#ifndef MATRIX_HPP        // Guard to prevent double inclusion
#define MATRIX_HPP

#include <vector>           // For 2D vector storage
#include <fstream>          // For file input/output
#include <iostream>         // For standard input/output
#include <string>           // For file paths and error messages

class Matrix            // Base class for all matrix operations
{
protected:
    int rows;                                    // Number of rows in matrix
    int cols;                                    // Number of columns in matrix
    std::vector<std::vector<double>> data;       // 2D data storage with dynamic allocation
    bool loaded;                                 // Tracks whether matrix data is available

public:

    Matrix();                                    // Default constructor, empty matrix
    Matrix(int r, int c);                        // Constructor with dimensions
    Matrix(const Matrix &m);                     // Copy constructor

    void readFromFile(std::ifstream &fin);       // Read matrix values from file
    bool isLoadFile(const std::string &path, std::string *errorMessage = nullptr);  // Validate/load file for user workflow
    bool loadFromFile(const std::string &path, std::string *errorMessage = nullptr);  // Safe file loader
    bool isLoaded() const;                       // Check if matrix has loaded/initialized data
    void displayToFile(std::ofstream &fout) const;  // Write matrix to file
    void display() const;                        // Print matrix to console

    Matrix operator+(const Matrix &m) const;     // Overload + for matrix addition
    Matrix operator-(const Matrix &m) const;     // Overload - for matrix subtraction
    Matrix operator*(const Matrix &m) const;     // Overload * for matrix multiplication

    double& operator()(int i, int j);            // Access element for modification
    double  operator()(int i, int j) const;      // Access element for reading

    bool operator==(const Matrix &m) const;      // Check if two matrices are equal

    friend std::istream& operator>>(std::istream &in,  Matrix &m);  // Read matrix from input stream
    friend std::ostream& operator<<(std::ostream &out, const Matrix &m);  // Write matrix to output stream

    int    getRows() const;                      // Get number of rows
    int    getCols() const;                      // Get number of columns
    double get(int i, int j) const;              // Get element at position (i,j)
    void   set(int i, int j, double v);          // Set element at position (i,j)
    std::vector<std::vector<double>>& getData(); // Get raw data reference

    bool isSquare()             const;           // Check if matrix is square
    bool isSymmetric()          const;           // Check if matrix is symmetric
    bool isIdentity()           const;           // Check if matrix is identity
    bool isNull()               const;           // Check if matrix is null (all zeros)
    bool isDiagonal()           const;           // Check if matrix is diagonal
    bool isDiagonallyDominant() const;           // Check diagonal dominance property
    bool isTranspose(const Matrix &m) const;     // Check if m is transpose of this matrix

    void   makeDiagonallyDominant();             // Rearrange rows to achieve diagonal dominance
    Matrix transpose()   const;                  // Return transposed matrix
    double determinant() const;                  // Calculate matrix determinant
    Matrix inverse()     const;                  // Calculate matrix inverse
};

#endif