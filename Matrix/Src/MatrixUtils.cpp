#include "../Include/Matrix.hpp" // include Matrix header
#include <cmath>                 // for abs(), pow()
#include <stdexcept>             // for runtime_error
using namespace std;             // use standard namespace

// ---- PROPERTY CHECKS ----

bool Matrix::isSquare()    const { return rows == cols; }        // square if rows == cols

bool Matrix::isNull() const                                      // all elements zero
{
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
            if (abs(data[i][j]) > 1e-9) return false;           // non-zero found
    return true;                                                 // all zero
}

bool Matrix::isDiagonal() const                                  // off-diagonal = 0
{
    if (!isSquare()) return false;                               // must be square
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
            if (i != j && abs(data[i][j]) > 1e-9) return false; // off-diag non-zero
    return true;                                                 // is diagonal
}

bool Matrix::isSymmetric() const                                 // A == A^T
{
    if (!isSquare()) return false;                               // must be square
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
            if (abs(data[i][j] - data[j][i]) > 1e-9) return false; // check symmetry
    return true;                                                 // is symmetric
}

bool Matrix::isIdentity() const                                  // diagonal=1, rest=0
{
    if (!isSquare()) return false;                               // must be square
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
        {
            double e = (i == j) ? 1.0 : 0.0;                    // expected value
            if (abs(data[i][j] - e) > 1e-9) return false;       // check element
        }
    return true;                                                 // is identity
}

bool Matrix::isDiagonallyDominant() const                        // diagonal dominance
{
    if (!isSquare()) return false;                               // must be square
    for (int i = 0; i < rows; i++)                               // loop rows
    {
        double sum = 0;                                          // off-diagonal sum
        for (int j = 0; j < cols; j++)                          // loop columns
            if (i != j) sum += abs(data[i][j]);                 // accumulate
        if (abs(data[i][i]) < sum) return false;                 // check dominance
    }
    return true;                                                 // is dominant
}

bool Matrix::isTranspose(const Matrix &m) const                  // check this == m^T
{
    if (rows != m.cols || cols != m.rows) return false;          // dimensions must swap
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
            if (abs(data[i][j] - m.data[j][i]) > 1e-9) return false; // compare
    return true;                                                 // is transpose
}

// ---- TRANSFORMATIONS ----

void Matrix::makeDiagonallyDominant()                            // reorder rows
{
    if (!isSquare()) throw runtime_error("not square");          // must be square
    for (int i = 0; i < rows; i++)                               // loop rows
    {
        int best = i;                                            // best row index
        for (int k = i+1; k < rows; k++)                        // search below
            if (abs(data[k][i]) > abs(data[best][i])) best = k; // find largest
        if (best != i) swap(data[i], data[best]);                // swap rows
    }
}

Matrix Matrix::transpose() const                                 // return transposed
{
    Matrix r(cols, rows);                                        // swapped dimensions
    for (int i = 0; i < rows; i++)                               // loop rows
        for (int j = 0; j < cols; j++)                          // loop columns
            r.data[j][i] = data[i][j];                         // swap position
    return r;                                                    // return result
}

double Matrix::determinant() const                               // cofactor expansion
{
    if (!isSquare()) throw runtime_error("determinant: not square"); // must be square
    if (rows == 1) return data[0][0];                            // 1x1 base case
    if (rows == 2) return data[0][0]*data[1][1] - data[0][1]*data[1][0]; // 2x2
    double det = 0;                                              // result
    for (int p = 0; p < cols; p++)                               // expand row 0
    {
        Matrix sub(rows-1, cols-1);                              // submatrix
        for (int i = 1; i < rows; i++)                           // loop rows
        {
            int c = 0;                                           // sub col index
            for (int j = 0; j < cols; j++)                      // loop columns
                if (j != p) sub.data[i-1][c++] = data[i][j];   // skip col p
        }
        det += pow(-1.0, p) * data[0][p] * sub.determinant();  // cofactor
    }
    return det;                                                  // return result
}

Matrix Matrix::inverse() const                                   // Gauss-Jordan
{
    if (!isSquare()) throw runtime_error("inverse: not square"); // must be square
    if (abs(determinant()) < 1e-9) throw runtime_error("inverse: singular"); // check
    int n = rows;                                                // size
    Matrix aug(n, 2*n);                                         // [A | I]
    for (int i = 0; i < n; i++)                                  // build augmented
    {
        for (int j = 0; j < n; j++) aug.data[i][j] = data[i][j]; // copy A
        aug.data[i][n+i] = 1.0;                                 // identity
    }
    for (int k = 0; k < n; k++)                                  // pivot loop
    {
        int mx = k;
        for (int i = k+1; i < n; i++)                           // find max pivot
            if (abs(aug.data[i][k]) > abs(aug.data[mx][k])) mx = i;
        swap(aug.data[k], aug.data[mx]);                         // swap rows
        double piv = aug.data[k][k];                             // pivot value
        if (abs(piv) < 1e-9) throw runtime_error("inverse: zero pivot");
        for (int j = 0; j < 2*n; j++) aug.data[k][j] /= piv;  // normalize
        for (int i = 0; i < n; i++)                              // eliminate
        {
            if (i == k) continue;
            double f = aug.data[i][k];
            for (int j = 0; j < 2*n; j++) aug.data[i][j] -= f * aug.data[k][j];
        }
    }
    Matrix inv(n, n);                                            // result
    for (int i = 0; i < n; i++)                                  // extract right half
        for (int j = 0; j < n; j++)
            inv.data[i][j] = aug.data[i][n+j];
    return inv;                                                  // return inverse
}