#include "../Include/GaussianElimination.hpp"
#include "../Include/PlotHelper.hpp"
#include "../Include/Solutionprinter.hpp"
#include <vector>
#include <cmath>
#include <stdexcept>
using namespace std;

GaussianElimination::GaussianElimination() : LinearOperation() {}
GaussianElimination::GaussianElimination(int r, int c) : LinearOperation(r,c){}

ostream& operator<<(ostream &out, const GaussianElimination &ge)
{ out << "GaussianElimination object\n"; return out; }

//  Gaussian WITH Partial Pivoting 
void GaussianElimination::gaussianWithPivoting(
    Matrix &matrix, ofstream &matrixOut, ofstream &vectorOut)
{
    int n   = matrix.getRows();
    auto &A = matrix.getData();

    for (int k = 0; k < n-1; k++)
    {
        int mx = k;
        for (int i = k+1; i < n; i++)
            if (abs(A[i][k]) > abs(A[mx][k])) mx = i;
        swap(A[k], A[mx]);
        if (abs(A[k][k]) < 1e-12)
            throw runtime_error("Singular matrix: zero pivot");
        for (int i = k+1; i < n; i++)
        {
            double f = A[i][k] / A[k][k];
            for (int j = k; j < n+1; j++)
                A[i][j] -= f * A[k][j];
        }
    }

    matrixOut << n << " " << n+1 << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n+1; j++) matrixOut << A[i][j] << " ";
        matrixOut << "\n";
    }

    vector<double> x(n);
    for (int i = n-1; i >= 0; i--) {
        x[i] = A[i][n];
        for (int j = i+1; j < n; j++) x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }

    // ── unified output format ──
    SolutionPrinter::printDirect(x, "Gaussian Elimination WITH Pivoting", vectorOut);

    PlotHelper::saveAndPlot(x,
        "Output/gaussian_pivot_plot.dat",
        "Output/gaussian_pivot_plot.gnu",
        "Output/gaussian_pivot_graph.png",
        "Gaussian Elimination WITH Pivoting");
}

//  Gaussian WITHOUT Pivoting 
void GaussianElimination::gaussianWithoutPivoting(
    Matrix &matrix, ofstream &matrixOut, ofstream &vectorOut)
{
    int n   = matrix.getRows();
    auto &A = matrix.getData();

    for (int k = 0; k < n-1; k++)
    {
        if (abs(A[k][k]) < 1e-12)
            throw runtime_error("Zero pivot: try Gaussian WITH pivoting instead");
        for (int i = k+1; i < n; i++)
        {
            double f = A[i][k] / A[k][k];
            for (int j = k; j < n+1; j++)
                A[i][j] -= f * A[k][j];
        }
    }

    matrixOut << n << " " << n+1 << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n+1; j++) matrixOut << A[i][j] << " ";
        matrixOut << "\n";
    }

    vector<double> x(n);
    for (int i = n-1; i >= 0; i--) {
        x[i] = A[i][n];
        for (int j = i+1; j < n; j++) x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }

    SolutionPrinter::printDirect(x, "Gaussian Elimination WITHOUT Pivoting", vectorOut);

    PlotHelper::saveAndPlot(x,
        "Output/gaussian_nopivot_plot.dat",
        "Output/gaussian_nopivot_plot.gnu",
        "Output/gaussian_nopivot_graph.png",
        "Gaussian Elimination WITHOUT Pivoting");
}