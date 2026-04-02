#include "../Include/Matrix.hpp" // include Matrix header
#include <cmath>                 // for abs(), pow()
#include <stdexcept>             // for runtime_error
using namespace std;                              // use standard namespace


bool Matrix::isSquare()    const { return rows == cols; }  // define function

bool Matrix::isNull() const                       // define function
{
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            if (abs(data[i][j]) > 1e-9) return false;  // check condition
    return true;                                  // return result from function
}

bool Matrix::isDiagonal() const                   // define function
{
    if (!isSquare()) return false;                // check condition
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            if (i != j && abs(data[i][j]) > 1e-9) return false;  // check condition
    return true;                                  // return result from function
}

bool Matrix::isSymmetric() const                  // define function
{
    if (!isSquare()) return false;                // check condition
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            if (abs(data[i][j] - data[j][i]) > 1e-9) return false;  // check condition
    return true;                                  // return result from function
}

bool Matrix::isIdentity() const                   // define function
{
    if (!isSquare()) return false;                // check condition
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
        {
            double e = (i == j) ? 1.0 : 0.0;      // declare and initialize object
            if (abs(data[i][j] - e) > 1e-9) return false;  // check condition
        }
    return true;                                  // return result from function
}

bool Matrix::isDiagonallyDominant() const         // define function
{
    if (!isSquare()) return false;                // check condition
    for (int i = 0; i < rows; i++)                // iterate over elements
    {
        double sum = 0;                           // declare and assign variable
        for (int j = 0; j < cols; j++)            // iterate over elements
            if (i != j) sum += abs(data[i][j]);   // check condition
        if (abs(data[i][i]) < sum) return false;  // check condition
    }
    return true;                                  // return result from function
}

bool Matrix::isTranspose(const Matrix &m) const   // define function
{
    if (rows != m.cols || cols != m.rows) return false;  // check condition
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            if (abs(data[i][j] - m.data[j][i]) > 1e-9) return false;  // check condition
    return true;                                  // return result from function
}


void Matrix::makeDiagonallyDominant()             // define function
{
    if (!isSquare()) throw runtime_error("not square");  // check condition
    for (int i = 0; i < rows; i++)                // iterate over elements
    {
        int best = i;                             // declare and assign variable
        for (int k = i+1; k < rows; k++)          // iterate over elements
            if (abs(data[k][i]) > abs(data[best][i])) best = k;  // check condition
        if (best != i) swap(data[i], data[best]); // check condition
    }
}

Matrix Matrix::transpose() const                  // define function
{
    Matrix r(cols, rows);                         // call function or method
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            r.data[j][i] = data[i][j];            // assign value to variable
    return r;                                     // return result from function
}

double Matrix::determinant() const                // define function
{
    if (!isSquare()) throw runtime_error("determinant: not square");  // check condition
    if (rows == 1) return data[0][0];             // check condition
    if (rows == 2) return data[0][0]*data[1][1] - data[0][1]*data[1][0];  // check condition
    double det = 0;                               // declare and assign variable
    for (int p = 0; p < cols; p++)                // iterate over elements
    {
        Matrix sub(rows-1, cols-1);               // call function or method
        for (int i = 1; i < rows; i++)            // iterate over elements
        {
            int c = 0;                            // declare and assign variable
            for (int j = 0; j < cols; j++)        // iterate over elements
                if (j != p) sub.data[i-1][c++] = data[i][j];  // check condition
        }
        det += pow(-1.0, p) * data[0][p] * sub.determinant();  // update variable with operation
    }
    return det;                                   // return result from function
}

Matrix Matrix::inverse() const                    // define function
{
    if (!isSquare()) throw runtime_error("inverse: not square");  // check condition
    if (abs(determinant()) < 1e-9) throw runtime_error("inverse: singular");  // check condition
    int n = rows;                                 // declare and assign variable
    Matrix aug(n, 2*n);                           // call function or method
    for (int i = 0; i < n; i++)                   // iterate over elements
    {
        for (int j = 0; j < n; j++) aug.data[i][j] = data[i][j];  // iterate over elements
        aug.data[i][n+i] = 1.0;                   // assign value to variable
    }
    for (int k = 0; k < n; k++)                   // iterate over elements
    {
        int mx = k;                               // declare and assign variable
        for (int i = k+1; i < n; i++)             // iterate over elements
            if (abs(aug.data[i][k]) > abs(aug.data[mx][k])) mx = i;  // check condition
        swap(aug.data[k], aug.data[mx]);          // call function or method
        double piv = aug.data[k][k];              // declare and assign variable
        if (abs(piv) < 1e-9) throw runtime_error("inverse: zero pivot");  // check condition
        for (int j = 0; j < 2*n; j++) aug.data[k][j] /= piv;  // iterate over elements
        for (int i = 0; i < n; i++)               // iterate over elements
        {
            if (i == k) continue;                 // check condition
            double f = aug.data[i][k];            // declare and assign variable
            for (int j = 0; j < 2*n; j++) aug.data[i][j] -= f * aug.data[k][j];  // iterate over elements
        }
    }
    Matrix inv(n, n);                             // call function or method
    for (int i = 0; i < n; i++)                   // iterate over elements
        for (int j = 0; j < n; j++)               // iterate over elements
            inv.data[i][j] = aug.data[i][n+j];    // assign value to variable
    return inv;                                   // return result from function
}