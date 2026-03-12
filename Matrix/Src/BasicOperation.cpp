#include "../Include/BasicOperation.hpp" // MUST include own header, not Matrix.hpp directly
#include <stdexcept>                     // for runtime_error
#include <cmath>                         // for abs(), pow()
using namespace std;                     // use standard namespace

// ---- CONSTRUCTORS ----
BasicOperation::BasicOperation() : Matrix() {}                  // default constructor
BasicOperation::BasicOperation(int r, int c) : Matrix(r, c) {} // parameterized constructor

// ---- ADD ----
BasicOperation BasicOperation::add(const BasicOperation &o) const
{
    Matrix res = static_cast<const Matrix&>(*this) + static_cast<const Matrix&>(o); // operator+
    BasicOperation r(res.getRows(), res.getCols());              // create result
    for (int i = 0; i < res.getRows(); i++)                      // loop rows
        for (int j = 0; j < res.getCols(); j++)                  // loop columns
            r.set(i, j, res.get(i, j));                         // copy element
    return r;                                                    // return result
}

// ---- SUBTRACT ----
BasicOperation BasicOperation::subtract(const BasicOperation &o) const
{
    Matrix res = static_cast<const Matrix&>(*this) - static_cast<const Matrix&>(o); // operator-
    BasicOperation r(res.getRows(), res.getCols());              // create result
    for (int i = 0; i < res.getRows(); i++)                      // loop rows
        for (int j = 0; j < res.getCols(); j++)                  // loop columns
            r.set(i, j, res.get(i, j));                         // copy element
    return r;                                                    // return result
}

// ---- MULTIPLY ----
BasicOperation BasicOperation::multiply(const BasicOperation &o) const
{
    Matrix res = static_cast<const Matrix&>(*this) * static_cast<const Matrix&>(o); // operator*
    BasicOperation r(res.getRows(), res.getCols());              // create result
    for (int i = 0; i < res.getRows(); i++)                      // loop rows
        for (int j = 0; j < res.getCols(); j++)                  // loop columns
            r.set(i, j, res.get(i, j));                         // copy element
    return r;                                                    // return result
}

// ---- SCALAR DIVIDE ----
BasicOperation BasicOperation::scalarDivide(double v) const
{
    if (abs(v) < 1e-9)                                           // check zero
        throw runtime_error("scalarDivide: division by zero");
    BasicOperation r(rows, cols);                                // result matrix
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
            r.set(i, j, data[i][j] / v);                       // divide element
    return r;                                                    // return result
}

// ---- SCALAR MULTIPLY ----
BasicOperation BasicOperation::scalarMultiply(double v) const
{
    BasicOperation r(rows, cols);                                // result matrix
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
            r.set(i, j, data[i][j] * v);                       // multiply element
    return r;                                                    // return result
}

// ---- TRANSPOSE ----
BasicOperation BasicOperation::transpose() const
{
    BasicOperation r(cols, rows);                                // swapped dimensions
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
            r.set(j, i, data[i][j]);                            // swap position
    return r;                                                    // return result
}

// ---- DETERMINANT ----
double BasicOperation::determinant() const
{
    if (rows != cols)                                            // must be square
        throw runtime_error("determinant: not square");
    if (rows == 1) return data[0][0];                            // 1x1 base case
    if (rows == 2)                                               // 2x2 formula
        return data[0][0]*data[1][1] - data[0][1]*data[1][0];
    double det = 0;                                              // result
    for (int p = 0; p < cols; p++)                               // expand along row 0
    {
        BasicOperation sub(rows-1, cols-1);                      // submatrix
        for (int i = 1; i < rows; i++)                           // loop rows (skip 0)
        {
            int c = 0;                                           // sub column index
            for (int j = 0; j < cols; j++)                      // loop all columns
                if (j != p) { sub.set(i-1, c, data[i][j]); c++; } // skip col p
        }
        det += pow(-1.0, p) * data[0][p] * sub.determinant();   // cofactor term
    }
    return det;                                                  // return result
}

// ---- FRIEND OPERATOR<< ----
ostream& operator<<(ostream &out, const BasicOperation &b)
{
    out << "BasicOperation(" << b.rows << "x" << b.cols << "):\n"; // header line
    for (int i = 0; i < b.rows; i++)                             // loop rows
    {
        for (int j = 0; j < b.cols; j++)                        // loop columns
            out << b.data[i][j] << "\t";                        // print element
        out << "\n";                                             // newline
    }
    return out;                                                  // return stream
}