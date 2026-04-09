#include "../Include/Matrix.hpp"
#include <stdexcept>
#include <cmath>
using namespace std;


Matrix Matrix::operator+(const Matrix &m) const
{
    if (rows != m.rows || cols != m.cols)
        throw runtime_error("operator+: dimension mismatch");
    Matrix r(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            r.data[i][j] = data[i][j] + m.data[i][j];
    return r;
}

Matrix Matrix::operator-(const Matrix &m) const
{
    if (rows != m.rows || cols != m.cols)
        throw runtime_error("operator-: dimension mismatch");
    Matrix r(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            r.data[i][j] = data[i][j] - m.data[i][j];
    return r;
}

Matrix Matrix::operator*(const Matrix &m) const
{
    if (cols != m.rows)
        throw runtime_error("operator*: dimension mismatch");
    Matrix r(rows, m.cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < m.cols; j++)
            for (int k = 0; k < cols; k++)
                r.data[i][j] += data[i][k] * m.data[k][j];
    return r;
}


bool Matrix::operator==(const Matrix &m) const
{
    if (rows != m.rows || cols != m.cols) return false;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (abs(data[i][j] - m.data[i][j]) > 1e-9)
                return false;
    return true;
}