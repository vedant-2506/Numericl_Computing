#include "../Include/GaussianElimination.hpp"         // include header
#include "../Include/PlotHelper.hpp"                   // include plot helper
#include <vector>                                      // for vector
#include <cmath>                                       // for abs()
#include <stdexcept>                                   // for runtime_error
using namespace std;                                   // use standard namespace

// ---- CONSTRUCTORS ----
GaussianElimination::GaussianElimination() : LinearOperation() {}               // default
GaussianElimination::GaussianElimination(int r, int c) : LinearOperation(r,c){} // parameterized

// ---- FRIEND OPERATOR ----
ostream& operator<<(ostream &out, const GaussianElimination &ge)
{
    out << "GaussianElimination object\n";             // label
    return out;                                        // return stream
}

// ================================================================
// GAUSSIAN WITH PARTIAL PIVOTING  (choice 7)
// ================================================================
void GaussianElimination::gaussianWithPivoting(
    Matrix &matrix, ofstream &matrixOut, ofstream &vectorOut)
{
    int n   = matrix.getRows();                        // number of equations
    auto &A = matrix.getData();                        // reference to matrix data

    // ---- FORWARD ELIMINATION WITH PARTIAL PIVOTING ----
    for (int k = 0; k < n-1; k++)                     // loop pivot columns
    {
        int mx = k;                                    // assume current row is max
        for (int i = k+1; i < n; i++)                 // search below for larger pivot
            if (abs(A[i][k]) > abs(A[mx][k])) mx = i; // update max row index
        swap(A[k], A[mx]);                             // swap rows

        if (abs(A[k][k]) < 1e-9)                      // check for zero pivot
            throw runtime_error("Singular matrix: zero pivot");

        for (int i = k+1; i < n; i++)                 // eliminate below pivot
        {
            double f = A[i][k] / A[k][k];             // elimination factor
            for (int j = k; j < n+1; j++)             // apply to entire row
                A[i][j] -= f * A[k][j];               // row reduction
        }
    }

    // ---- SAVE UPPER TRIANGULAR MATRIX ----
    matrixOut << n << " " << n+1 << endl;              // write dimensions
    for (int i = 0; i < n; i++)                        // loop rows
    {
        for (int j = 0; j < n+1; j++)                 // loop columns
            matrixOut << A[i][j] << " ";               // write element
        matrixOut << endl;                             // newline
    }

    // ---- BACK SUBSTITUTION ----
    vector<double> x(n);                               // solution vector
    for (int i = n-1; i >= 0; i--)                    // loop backward
    {
        x[i] = A[i][n];                                // start with b value
        for (int j = i+1; j < n; j++)                 // subtract known values
            x[i] -= A[i][j] * x[j];                   // accumulated sum
        x[i] /= A[i][i];                               // divide by diagonal
    }

    // ---- SAVE SOLUTION VECTOR ----
    vectorOut << n << " 1" << endl;                    // write dimensions
    for (int i = 0; i < n; i++)                        // loop solution
        vectorOut << x[i] << endl;                     // write each value

    // ---- PLOT ----
    PlotHelper::saveAndPlot(x,
        "Output/gaussian_pivot_plot.dat",
        "Output/gaussian_pivot_plot.gnu",
        "Output/gaussian_pivot_graph.png",
        "Gaussian Elimination WITH Pivoting");
}

// ================================================================
// GAUSSIAN WITHOUT PIVOTING  (choice 8)
// ================================================================
void GaussianElimination::gaussianWithoutPivoting(
    Matrix &matrix, ofstream &matrixOut, ofstream &vectorOut)
{
    int n   = matrix.getRows();                        // number of equations
    auto &A = matrix.getData();                        // reference to matrix data

    // ---- FORWARD ELIMINATION (NO ROW SWAPPING) ----
    for (int k = 0; k < n-1; k++)                     // loop pivot columns
    {
        if (abs(A[k][k]) < 1e-9)                      // check for zero pivot
            throw runtime_error("Zero pivot: try Gaussian WITH pivoting instead");

        for (int i = k+1; i < n; i++)                 // eliminate below pivot
        {
            double f = A[i][k] / A[k][k];             // elimination factor
            for (int j = k; j < n+1; j++)             // apply to entire row
                A[i][j] -= f * A[k][j];               // row reduction
        }
    }

    // ---- SAVE UPPER TRIANGULAR MATRIX ----
    matrixOut << n << " " << n+1 << endl;              // write dimensions
    for (int i = 0; i < n; i++)                        // loop rows
    {
        for (int j = 0; j < n+1; j++)                 // loop columns
            matrixOut << A[i][j] << " ";               // write element
        matrixOut << endl;                             // newline
    }

    // ---- BACK SUBSTITUTION ----
    vector<double> x(n);                               // solution vector
    for (int i = n-1; i >= 0; i--)                    // loop backward
    {
        x[i] = A[i][n];                                // start with b value
        for (int j = i+1; j < n; j++)                 // subtract known values
            x[i] -= A[i][j] * x[j];                   // accumulated sum
        x[i] /= A[i][i];                               // divide by diagonal
    }

    // ---- SAVE SOLUTION VECTOR ----
    vectorOut << n << " 1" << endl;                    // write dimensions
    for (int i = 0; i < n; i++)                        // loop solution
        vectorOut << x[i] << endl;                     // write each value

    // ---- PLOT ----
    PlotHelper::saveAndPlot(x,
        "Output/gaussian_nopivot_plot.dat",
        "Output/gaussian_nopivot_plot.gnu",
        "Output/gaussian_nopivot_graph.png",
        "Gaussian Elimination WITHOUT Pivoting");
}