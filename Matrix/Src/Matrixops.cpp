#include "../Include/Matrix.hpp" // include Matrix header
#include <stdexcept>             // for runtime_error
#include <cmath>                 // for abs()
using namespace std;             // use standard namespace

// ---- ARITHMETIC OPERATORS ----

Matrix Matrix::operator+(const Matrix &m) const           // matrix addition
{
    if (rows != m.rows || cols != m.cols)                 // check dimensions match
        throw runtime_error("operator+: dimension mismatch"); // error if not
    Matrix r(rows, cols);                                 // create result matrix
    for (int i = 0; i < rows; i++)                        // loop rows
        for (int j = 0; j < cols; j++)                   // loop columns
            r.data[i][j] = data[i][j] + m.data[i][j];   // add elements
    return r;                                             // return result
}

Matrix Matrix::operator-(const Matrix &m) const           // matrix subtraction
{
    if (rows != m.rows || cols != m.cols)                 // check dimensions match
        throw runtime_error("operator-: dimension mismatch"); // error if not
    Matrix r(rows, cols);                                 // create result matrix
    for (int i = 0; i < rows; i++)                        // loop rows
        for (int j = 0; j < cols; j++)                   // loop columns
            r.data[i][j] = data[i][j] - m.data[i][j];   // subtract elements
    return r;                                             // return result
}

Matrix Matrix::operator*(const Matrix &m) const           // matrix multiplication
{
    if (cols != m.rows)                                   // check inner dimensions
        throw runtime_error("operator*: dimension mismatch"); // error if not
    Matrix r(rows, m.cols);                               // create result matrix
    for (int i = 0; i < rows; i++)                        // loop rows
        for (int j = 0; j < m.cols; j++)                 // loop columns
            for (int k = 0; k < cols; k++)               // inner dimension loop
                r.data[i][j] += data[i][k] * m.data[k][j]; // accumulate product
    return r;                                             // return result
}

// ---- COMPARISON OPERATOR ----

bool Matrix::operator==(const Matrix &m) const            // equality check
{
    if (rows != m.rows || cols != m.cols) return false;   // dimension mismatch = not equal
    for (int i = 0; i < rows; i++)                        // loop rows
        for (int j = 0; j < cols; j++)                   // loop columns
            if (abs(data[i][j] - m.data[i][j]) > 1e-9)  // compare with tolerance
                return false;                             // not equal
    return true;                                          // all elements matched
}