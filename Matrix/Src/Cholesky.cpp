#include "../Include/LUDecomposition.hpp" // include LU header
#include "../Include/PlotHelper.hpp"       // include plot helper
#include <fstream>                         // for ofstream
#include <vector>                          // for vector
#include <cmath>                           // for sqrt()
#include <stdexcept>                       // for runtime_error
using namespace std;                       // use standard namespace

void LUDecomposition::choleskyDecomposition(const string &Afile, const string &bfile)
{
    // build and load augmented matrix [A|b]
    string augFile = "Output/cholesky_augmented.txt";
    LinearOperation::generateAugmentedMatrixFile(Afile, bfile, augFile);
    ifstream fin(augFile);
    int n, c; fin >> n >> c;
    vector<vector<double>> A(n, vector<double>(n)); // must be symmetric positive definite
    vector<double> b(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) fin >> A[i][j];
        fin >> b[i];
    }
    fin.close();

    // check symmetry
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (abs(A[i][j] - A[j][i]) > 1e-9)
                throw runtime_error("Cholesky: matrix is not symmetric");

    // Cholesky decomposition: A = L * L^T
    vector<vector<double>> L(n, vector<double>(n, 0)); // lower triangular L
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++)
        {
            double s = 0;
            for (int k = 0; k < j; k++) s += L[i][k] * L[j][k];
            if (i == j)
            {
                double v = A[i][i] - s;
                if (v <= 0) throw runtime_error("Cholesky: not positive definite");
                L[i][j] = sqrt(v);
            }
            else L[i][j] = (A[i][j] - s) / L[j][j];
        }

    // forward substitution: Ly = b
    vector<double> y(n, 0);
    for (int i = 0; i < n; i++)
    {
        y[i] = b[i];
        for (int j = 0; j < i; j++) y[i] -= L[i][j] * y[j];
        y[i] /= L[i][i];
    }

    // back substitution: L^T x = y
    vector<double> x(n, 0);
    for (int i = n-1; i >= 0; i--)
    {
        x[i] = y[i];
        for (int j = i+1; j < n; j++) x[i] -= L[j][i] * x[j]; // L^T[i][j] = L[j][i]
        x[i] /= L[i][i];
    }

    // save L matrix and solution vector
    ofstream fL("Output/cholesky_L.txt");
    fL << n << " " << n << "\n";
    for (auto &row : L) { for (double v : row) fL << v << " "; fL << "\n"; }
    ofstream fx("Output/cholesky_vector.txt");
    fx << n << " 1\n";
    for (double v : x) fx << v << "\n";

    PlotHelper::saveAndPlot(x,
        "Output/cholesky_plot.dat",
        "Output/cholesky_plot.gnu",
        "Output/cholesky_graph.png",
        "Cholesky - Solution");
}