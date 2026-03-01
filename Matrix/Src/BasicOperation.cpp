#include "../Include/BasicOperation.hpp"

// Default constructor
BasicOperation::BasicOperation() : rows(0), cols(0) {}

// Parameterized constructor
BasicOperation::BasicOperation(int r, int c)
{
    rows = r;                              // assign rows
    cols = c;                              // assign cols
    mat.resize(rows, vector<double>(cols)); // resize matrix
}

// Read matrix values from file
void BasicOperation::readFromFile(ifstream &fin)
{
    for (int i = 0; i < rows; i++)          // loop rows
        for (int j = 0; j < cols; j++)      // loop columns
            fin >> mat[i][j];               // read element
}

// Display matrix to file
void BasicOperation::displayToFile(ofstream &fout) const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            fout << mat[i][j] << " ";       // print element
        fout << endl;                      // next line
    }
}

// Addition
BasicOperation BasicOperation::add(const BasicOperation &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw runtime_error("Dimension mismatch");

    BasicOperation result(rows, cols);      // result matrix

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.mat[i][j] = mat[i][j] + other.mat[i][j];

    return result;
}

// Subtraction
BasicOperation BasicOperation::subtract(const BasicOperation &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw runtime_error("Dimension mismatch");

    BasicOperation result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.mat[i][j] = mat[i][j] - other.mat[i][j];

    return result;
}

// Multiplication
BasicOperation BasicOperation::multiply(const BasicOperation &other) const
{
    if (cols != other.rows)
        throw runtime_error("Invalid multiplication");

    BasicOperation result(rows, other.cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < other.cols; j++)
            for (int k = 0; k < cols; k++)
                result.mat[i][j] += mat[i][k] * other.mat[k][j];

    return result;
}

// Scalar division
BasicOperation BasicOperation::scalarDivide(double value) const
{
    BasicOperation result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.mat[i][j] = mat[i][j] / value;

    return result;
}

// Transpose
BasicOperation BasicOperation::transpose() const
{
    BasicOperation result(cols, rows);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.mat[j][i] = mat[i][j];

    return result;
}

// Determinant (recursive)
double BasicOperation::determinant() const
{
    if (rows != cols)
        throw runtime_error("Not square matrix");

    if (rows == 1)
        return mat[0][0];

    if (rows == 2)
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    double det = 0;

    for (int p = 0; p < cols; p++)
    {
        BasicOperation sub(rows - 1, cols - 1);

        for (int i = 1; i < rows; i++)
        {
            int colIndex = 0;
            for (int j = 0; j < cols; j++)
            {
                if (j == p) continue;
                sub.mat[i - 1][colIndex] = mat[i][j];
                colIndex++;
            }
        }

        det += pow(-1, p) * mat[0][p] * sub.determinant();
    }

    return det;
}

int BasicOperation::getRows() const { return rows; }
int BasicOperation::getCols() const { return cols; }
vector<vector<double>> BasicOperation::getMatrix() const { return mat; }