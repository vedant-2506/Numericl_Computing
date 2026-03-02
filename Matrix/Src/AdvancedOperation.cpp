#include "../Include/AdvancedOperation.hpp"

using namespace std;

// Default constructor
AdvancedOperation::AdvancedOperation() : rows(0), cols(0) {}

// Parameterized constructor
AdvancedOperation::AdvancedOperation(int r, int c)
{
    rows = r;
    cols = c;
    mat.resize(rows, vector<double>(cols));
}

// Static wrapper to match legacy main.cpp API
void AdvancedOperation::generateAugmentedMatrixFile(const string &leftFile,
                                                    const string &rightFile,
                                                    const string &outputFile)
{
    generateAugmentedAdvancedOperationFile(leftFile, rightFile, outputFile);
}

// Static wrapper that accepts BasicOperation and forwards to instance method
void AdvancedOperation::gaussianEliminationWithPivoting(BasicOperation &matrix,
                                                       ofstream &matrixOut,
                                                       ofstream &vectorOut)
{
    int r = matrix.getRows();
    int c = matrix.getCols();
    AdvancedOperation C(r, c);
    auto data = matrix.getMatrix();
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j)
            C.mat[i][j] = data[i][j];

    C.gaussianEliminationWithPivoting(matrixOut, vectorOut);
}

// Copy constructor
AdvancedOperation::AdvancedOperation(const AdvancedOperation &m)
{
    rows = m.rows;
    cols = m.cols;
    mat = m.mat;
}

// Read AdvancedOperation from file
void AdvancedOperation::readFromFile(ifstream &fin)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            fin >> mat[i][j];
}

// Display AdvancedOperation to file
void AdvancedOperation::displayToFile(ofstream &fout) const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            fout << mat[i][j] << " ";
        fout << endl;
    }
}

// Addition
AdvancedOperation AdvancedOperation::add(const AdvancedOperation &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw runtime_error("Dimension mismatch for addition");

    AdvancedOperation result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.mat[i][j] = mat[i][j] + other.mat[i][j];

    return result;
}

// subtraction
AdvancedOperation AdvancedOperation::subtract(const AdvancedOperation &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw runtime_error("Dimension mismatch for subtraction");

    AdvancedOperation result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.mat[i][j] = mat[i][j] - other.mat[i][j];

    return result;
}

// Generate augmented AdvancedOperation file from left and right matrices
void AdvancedOperation::generateAugmentedAdvancedOperationFile(const string &leftFile,
                                         const string &rightFile,
                                         const string &outputFile)
{
    ifstream finL(leftFile);
    ifstream finR(rightFile);
    ofstream fout(outputFile);

    if (!finL || !finR || !fout)
        throw runtime_error("Error opening input files");

    int rL, cL, rR, cR;

    finL >> rL >> cL;
    finR >> rR >> cR;

    if (rL != rR || cR != 1)
        throw runtime_error("Dimension mismatch");

    fout << rL << " " << cL + 1 << endl;

    for (int i = 0; i < rL; i++)
    {
        for (int j = 0; j < cL; j++)
        {
            double val;
            finL >> val;
            fout << val << " ";
        }

        double rhs;
        finR >> rhs;
        fout << rhs << endl;
    }
}

// Gaussian elimination with pivoting
void AdvancedOperation::gaussianEliminationWithPivoting(ofstream &AdvancedOperationOut,
                                             ofstream &vectorOut)
{
    AdvancedOperationOut << "Augmented AdvancedOperation:\n";
    displayToFile(AdvancedOperationOut);

    if (cols != rows + 1)
        throw logic_error("AdvancedOperation must be augmented");

    for (int k = 0; k < rows - 1; k++)
    {
        int maxRow = k;

        for (int i = k + 1; i < rows; i++)
            if (abs(mat[i][k]) > abs(mat[maxRow][k]))
                maxRow = i;

        if (mat[maxRow][k] == 0)
            throw runtime_error("Singular AdvancedOperation");

        if (maxRow != k)
            swap(mat[k], mat[maxRow]);

        for (int i = k + 1; i < rows; i++)
        {
            double factor = mat[i][k] / mat[k][k];
            for (int j = k; j < cols; j++)
                mat[i][j] -= factor * mat[k][j];
        }
    }

    vector<double> x(rows);

    for (int i = rows - 1; i >= 0; i--)
    {
        x[i] = mat[i][cols - 1];
        for (int j = i + 1; j < rows; j++)
            x[i] -= mat[i][j] * x[j];
        x[i] /= mat[i][i];
    }

    vectorOut << "# X Y\n";
    for (int i = 0; i < rows; i++)
        vectorOut << i + 1 << " " << x[i] << endl;

    // Graph only for Gaussian
    ofstream plot("plot.gnu");
    plot << "set title 'X vs Y Solution Graph'\n";
    plot << "set xlabel 'X (Index)'\n";
    plot << "set ylabel 'Y (Solution Value)'\n";
    plot << "set grid\n";
    plot << "plot 'resultvector.txt' using 1:2 with linespoints title 'X vs Y'\n";
    plot << "pause -1\n";
}