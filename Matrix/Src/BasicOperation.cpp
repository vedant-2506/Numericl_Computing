#include "../Include/BasicOperation.hpp" // MUST include own header, not Matrix.hpp directly
#include <stdexcept>                     // for runtime_error
#include <cmath>                         // for abs(), pow()
using namespace std;                              // use standard namespace

BasicOperation::BasicOperation() : Matrix() {}    // define function
BasicOperation::BasicOperation(int r, int c) : Matrix(r, c) {}  // define function

BasicOperation BasicOperation::add(const BasicOperation &o) const  // define function
{
    Matrix res = static_cast<const Matrix&>(*this) + static_cast<const Matrix&>(o);  // assign value to variable
    BasicOperation r(res.getRows(), res.getCols());  // call function or method
    for (int i = 0; i < res.getRows(); i++)       // iterate over elements
        for (int j = 0; j < res.getCols(); j++)   // iterate over elements
            r.set(i, j, res.get(i, j));           // call function or method
    return r;                                     // return result from function
}

BasicOperation BasicOperation::subtract(const BasicOperation &o) const  // define function
{
    Matrix res = static_cast<const Matrix&>(*this) - static_cast<const Matrix&>(o);  // assign value to variable
    BasicOperation r(res.getRows(), res.getCols());  // call function or method
    for (int i = 0; i < res.getRows(); i++)       // iterate over elements
        for (int j = 0; j < res.getCols(); j++)   // iterate over elements
            r.set(i, j, res.get(i, j));           // call function or method
    return r;                                     // return result from function
}

BasicOperation BasicOperation::multiply(const BasicOperation &o) const  // define function
{
    Matrix res = static_cast<const Matrix&>(*this) * static_cast<const Matrix&>(o);  // assign value to variable
    BasicOperation r(res.getRows(), res.getCols());  // call function or method
    for (int i = 0; i < res.getRows(); i++)       // iterate over elements
        for (int j = 0; j < res.getCols(); j++)   // iterate over elements
            r.set(i, j, res.get(i, j));           // call function or method
    return r;                                     // return result from function
}

BasicOperation BasicOperation::scalarDivide(double v) const  // define function
{
    if (abs(v) < 1e-9)                            // check condition
        throw runtime_error("scalarDivide: division by zero");  // raise exception with error
    BasicOperation r(rows, cols);                 // call function or method
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            r.set(i, j, data[i][j] / v);          // call function or method
    return r;                                     // return result from function
}

BasicOperation BasicOperation::scalarMultiply(double v) const  // define function
{
    BasicOperation r(rows, cols);                 // call function or method
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            r.set(i, j, data[i][j] * v);          // call function or method
    return r;                                     // return result from function
}

BasicOperation BasicOperation::transpose() const  // define function
{
    BasicOperation r(cols, rows);                 // call function or method
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            r.set(j, i, data[i][j]);              // call function or method
    return r;                                     // return result from function
}

double BasicOperation::determinant() const        // define function
{
    if (rows != cols)                             // check condition
        throw runtime_error("determinant: not square");  // raise exception with error
    if (rows == 1) return data[0][0];             // check condition
    if (rows == 2)                                // check condition
        return data[0][0]*data[1][1] - data[0][1]*data[1][0];  // return result from function
    double det = 0;                               // declare and assign variable
    for (int p = 0; p < cols; p++)                // iterate over elements
    {
        BasicOperation sub(rows-1, cols-1);       // call function or method
        for (int i = 1; i < rows; i++)            // iterate over elements
        {
            int c = 0;                            // declare and assign variable
            for (int j = 0; j < cols; j++)        // iterate over elements
                if (j != p) { sub.set(i-1, c, data[i][j]); c++; }  // check condition
        }
        det += pow(-1.0, p) * data[0][p] * sub.determinant();  // update variable with operation
    }
    return det;                                   // return result from function
}

ostream& operator<<(ostream &out, const BasicOperation &b)  // stream input/output operation
{
    out << "BasicOperation(" << b.rows << "x" << b.cols << "):\n";  // check equality or comparison
    for (int i = 0; i < b.rows; i++)              // iterate over elements
    {
        for (int j = 0; j < b.cols; j++)          // iterate over elements
            out << b.data[i][j] << "\t";          // check equality or comparison
        out << "\n";                              // check equality or comparison
    }
    return out;                                   // return result from function
}