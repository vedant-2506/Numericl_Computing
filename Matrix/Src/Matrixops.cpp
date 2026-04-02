#include "../Include/Matrix.hpp" // include Matrix header
#include <stdexcept>             // for runtime_error
#include <cmath>                 // for abs()
using namespace std;                              // use standard namespace


Matrix Matrix::operator+(const Matrix &m) const   // define function
{
    if (rows != m.rows || cols != m.cols)         // check condition
        throw runtime_error("operator+: dimension mismatch");  // raise exception with error
    Matrix r(rows, cols);                         // call function or method
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            r.data[i][j] = data[i][j] + m.data[i][j];  // assign value to variable
    return r;                                     // return result from function
}

Matrix Matrix::operator-(const Matrix &m) const   // define function
{
    if (rows != m.rows || cols != m.cols)         // check condition
        throw runtime_error("operator-: dimension mismatch");  // raise exception with error
    Matrix r(rows, cols);                         // call function or method
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            r.data[i][j] = data[i][j] - m.data[i][j];  // assign value to variable
    return r;                                     // return result from function
}

Matrix Matrix::operator*(const Matrix &m) const   // define function
{
    if (cols != m.rows)                           // check condition
        throw runtime_error("operator*: dimension mismatch");  // raise exception with error
    Matrix r(rows, m.cols);                       // call function or method
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < m.cols; j++)          // iterate over elements
            for (int k = 0; k < cols; k++)        // iterate over elements
                r.data[i][j] += data[i][k] * m.data[k][j];  // update variable with operation
    return r;                                     // return result from function
}


bool Matrix::operator==(const Matrix &m) const    // define function
{
    if (rows != m.rows || cols != m.cols) return false;  // check condition
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            if (abs(data[i][j] - m.data[i][j]) > 1e-9)  // check condition
                return false;                     // return result from function
    return true;                                  // return result from function
}