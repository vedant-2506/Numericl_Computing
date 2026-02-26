#include "../Include/Gaussian.hpp"
#include <cmath>
#include <stdexcept>
#include <algorithm>

// Default Constructor
Matrix::Matrix()
{
    rows = 0;
    cols = 0;
}

// Parameterized Constructor
Matrix::Matrix(int rows, int cols)
{
    if (rows <= 0 || cols <= 0)
        throw invalid_argument("Matrix size must be positive.");

    this->rows = rows;
    this->cols = cols;

    mat.resize(rows, vector<double>(cols, 0));
}

// Copy Constructor
Matrix::Matrix(const Matrix &m)
{
    rows = m.rows;
    cols = m.cols;
    mat = m.mat;
}

// Read matrix from file
void Matrix::readFromFile(ifstream &fin)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            fin >> mat[i][j];
}

// Write matrix to file
void Matrix::displayToFile(ofstream &fout) const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            fout << mat[i][j] << " ";
        fout << endl;
    }
}

// Matrix Addition
Matrix Matrix::add(const Matrix &m) const
{
    if (rows != m.rows || cols != m.cols)
        throw logic_error("Addition not possible (size mismatch)");

    Matrix result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.mat[i][j] = mat[i][j] + m.mat[i][j];

    return result;
}

// Matrix Subtraction
Matrix Matrix::subtract(const Matrix &m) const
{
    if (rows != m.rows || cols != m.cols)
        throw logic_error("Subtraction not possible (size mismatch)");

    Matrix result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.mat[i][j] = mat[i][j] - m.mat[i][j];

    return result;
}

// Gaussian Elimination with Partial Pivoting
void Matrix::gaussianEliminationWithPivoting(ofstream &matrixOut,
                                            ofstream &solutionOut)
{
    const double EPS = 1e-12;

    if (cols != rows + 1)
        throw logic_error("Matrix must be augmented (n x n+1)");

    // Forward Elimination with Partial Pivoting
    for (int k = 0; k < rows - 1; k++)
    {
        int maxRow = k;
        double maxVal = fabs(mat[k][k]);

        for (int i = k + 1; i < rows; i++)
        {
            if (fabs(mat[i][k]) > maxVal)
            {
                maxVal = fabs(mat[i][k]);
                maxRow = i;
            }
        }

        if (maxVal < EPS)
            throw runtime_error("Matrix is singular → No unique solution.");

        if (maxRow != k)
            swap(mat[k], mat[maxRow]);

        for (int i = k + 1; i < rows; i++)
        {
            double factor = mat[i][k] / mat[k][k];

            for (int j = k; j < cols; j++)
                mat[i][j] -= factor * mat[k][j];
        }
    }

    // Save final upper triangular matrix
    matrixOut << "Upper Triangular Matrix:\n";
    displayToFile(matrixOut);

    // Back Substitution
    vector<double> x(rows);

    for (int i = rows - 1; i >= 0; i--)
    {
        if (fabs(mat[i][i]) < EPS)
            throw runtime_error("No unique solution.");

        x[i] = mat[i][cols - 1];

        for (int j = i + 1; j < rows; j++)
            x[i] -= mat[i][j] * x[j];

        x[i] /= mat[i][i];
    }

    // Save solution in required format
    solutionOut << "Solution Vector:\n";
    for (int i = 0; i < rows; i++)
        solutionOut << "x" << i + 1 << " = " << x[i] << endl;
}