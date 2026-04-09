#include "../Include/BasicOperation.hpp"  // Include basic operations class
#include <stdexcept>                      // For exception handling
#include <cmath>                          // For math functions
using namespace std;

BasicOperation::BasicOperation() : Matrix() {}  // Default constructor
BasicOperation::BasicOperation(int r, int c) : Matrix(r, c) {}  // Constructor with dimensions

BasicOperation BasicOperation::add(const BasicOperation &o) const  // Add two matrices
{
    Matrix res = static_cast<const Matrix&>(*this) + static_cast<const Matrix&>(o);  // Call parent addition
    BasicOperation r(res.getRows(), res.getCols());  // Create result matrix
    for (int i = 0; i < res.getRows(); i++)  // Loop through rows
        for (int j = 0; j < res.getCols(); j++)  // Loop through columns
            r.set(i, j, res.get(i, j));  // Copy values to result
    return r;  // Return result
}

BasicOperation BasicOperation::subtract(const BasicOperation &o) const
{
    Matrix res = static_cast<const Matrix&>(*this) - static_cast<const Matrix&>(o);
    BasicOperation r(res.getRows(), res.getCols());
    for (int i = 0; i < res.getRows(); i++)
        for (int j = 0; j < res.getCols(); j++)
            r.set(i, j, res.get(i, j));
    return r;
}

BasicOperation BasicOperation::multiply(const BasicOperation &o) const
{
    Matrix res = static_cast<const Matrix&>(*this) * static_cast<const Matrix&>(o);
    BasicOperation r(res.getRows(), res.getCols());
    for (int i = 0; i < res.getRows(); i++)
        for (int j = 0; j < res.getCols(); j++)
            r.set(i, j, res.get(i, j));
    return r;
}

BasicOperation BasicOperation::scalarDivide(double v) const
{
    if (abs(v) < 1e-9)
        throw runtime_error("scalarDivide: division by zero");
    BasicOperation r(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            r.set(i, j, data[i][j] / v);
    return r;
}

BasicOperation BasicOperation::scalarMultiply(double v) const
{
    BasicOperation r(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            r.set(i, j, data[i][j] * v);
    return r;
}

BasicOperation BasicOperation::transpose() const
{
    BasicOperation r(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            r.set(j, i, data[i][j]);
    return r;
}

double BasicOperation::determinant() const
{
    if (rows != cols)
        throw runtime_error("determinant: not square");
    if (rows == 1) return data[0][0];
    if (rows == 2)
        return data[0][0]*data[1][1] - data[0][1]*data[1][0];
    double det = 0;
    for (int p = 0; p < cols; p++)
    {
        BasicOperation sub(rows-1, cols-1);
        for (int i = 1; i < rows; i++)
        {
            int c = 0;
            for (int j = 0; j < cols; j++)
                if (j != p) { sub.set(i-1, c, data[i][j]); c++; }
        }
        det += pow(-1.0, p) * data[0][p] * sub.determinant();
    }
    return det;
}

ostream& operator<<(ostream &out, const BasicOperation &b)
{
    out << "BasicOperation(" << b.rows << "x" << b.cols << "):\n";
    for (int i = 0; i < b.rows; i++)
    {
        for (int j = 0; j < b.cols; j++)
            out << b.data[i][j] << "\t";
        out << "\n";
    }
    return out;
}