#include "../Include/Matrix.hpp"
#include <cmath>
#include <stdexcept>
using namespace std;


bool Matrix::isSquare()    const { return rows == cols; }

bool Matrix::isNull() const
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (abs(data[i][j]) > 1e-9) return false;
    return true;
}

bool Matrix::isDiagonal() const
{
    if (!isSquare()) return false;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (i != j && abs(data[i][j]) > 1e-9) return false;
    return true;
}

bool Matrix::isSymmetric() const
{
    if (!isSquare()) return false;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (abs(data[i][j] - data[j][i]) > 1e-9) return false;
    return true;
}

bool Matrix::isIdentity() const
{
    if (!isSquare()) return false;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            double e = (i == j) ? 1.0 : 0.0;
            if (abs(data[i][j] - e) > 1e-9) return false;
        }
    return true;
}

bool Matrix::isDiagonallyDominant() const
{
    if (!isSquare()) return false;
    for (int i = 0; i < rows; i++)
    {
        double sum = 0;
        for (int j = 0; j < cols; j++)
            if (i != j) sum += abs(data[i][j]);
        if (abs(data[i][i]) < sum) return false;
    }
    return true;
}

bool Matrix::isTranspose(const Matrix &m) const
{
    if (rows != m.cols || cols != m.rows) return false;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (abs(data[i][j] - m.data[j][i]) > 1e-9) return false;
    return true;
}


void Matrix::makeDiagonallyDominant()
{
    if (!isSquare()) throw runtime_error("not square");
    for (int i = 0; i < rows; i++)
    {
        int best = i;
        for (int k = i+1; k < rows; k++)
            if (abs(data[k][i]) > abs(data[best][i])) best = k;
        if (best != i) swap(data[i], data[best]);
    }
}

Matrix Matrix::transpose() const
{
    Matrix r(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            r.data[j][i] = data[i][j];
    return r;
}

double Matrix::determinant() const
{
    if (!isSquare()) throw runtime_error("determinant: not square");
    if (rows == 1) return data[0][0];
    if (rows == 2) return data[0][0]*data[1][1] - data[0][1]*data[1][0];
    double det = 0;
    for (int p = 0; p < cols; p++)
    {
        Matrix sub(rows-1, cols-1);
        for (int i = 1; i < rows; i++)
        {
            int c = 0;
            for (int j = 0; j < cols; j++)
                if (j != p) sub.data[i-1][c++] = data[i][j];
        }
        det += pow(-1.0, p) * data[0][p] * sub.determinant();
    }
    return det;
}

Matrix Matrix::inverse() const
{
    if (!isSquare()) throw runtime_error("inverse: not square");
    if (abs(determinant()) < 1e-9) throw runtime_error("inverse: singular");
    int n = rows;
    Matrix aug(n, 2*n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) aug.data[i][j] = data[i][j];
        aug.data[i][n+i] = 1.0;
    }
    for (int k = 0; k < n; k++)
    {
        int mx = k;
        for (int i = k+1; i < n; i++)
            if (abs(aug.data[i][k]) > abs(aug.data[mx][k])) mx = i;
        swap(aug.data[k], aug.data[mx]);
        double piv = aug.data[k][k];
        if (abs(piv) < 1e-9) throw runtime_error("inverse: zero pivot");
        for (int j = 0; j < 2*n; j++) aug.data[k][j] /= piv;
        for (int i = 0; i < n; i++)
        {
            if (i == k) continue;
            double f = aug.data[i][k];
            for (int j = 0; j < 2*n; j++) aug.data[i][j] -= f * aug.data[k][j];
        }
    }
    Matrix inv(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            inv.data[i][j] = aug.data[i][n+j];
    return inv;
}