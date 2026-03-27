#include "../Include/LUDecomposition.hpp"
#include "../Include/PlotHelper.hpp"
#include "../Include/Solutionprinter.hpp"
#include <fstream>
#include <vector>
#include <cmath>
#include <stdexcept>
using namespace std;

void LUDecomposition::choleskyDecomposition(const string &Afile, const string &bfile)
{
    string augFile = "Output/cholesky_augmented.txt";
    LinearOperation::generateAugmentedMatrixFile(Afile, bfile, augFile);
    ifstream fin(augFile);
    int n, c; fin >> n >> c;
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) fin >> A[i][j];
        fin >> b[i];
    }
    fin.close();

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (abs(A[i][j] - A[j][i]) > 1e-9)
                throw runtime_error("Cholesky: matrix is not symmetric");

    vector<vector<double>> L(n, vector<double>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= i; j++)
        {
            double s = 0;
            for (int k = 0; k < j; k++) s += L[i][k] * L[j][k];
            if (i == j) {
                double v = A[i][i] - s;
                if (v <= 0) throw runtime_error("Cholesky: not positive definite");
                L[i][j] = sqrt(v);
            } else L[i][j] = (A[i][j] - s) / L[j][j];
        }

    vector<double> y(n, 0);
    for (int i = 0; i < n; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) y[i] -= L[i][j] * y[j];
        y[i] /= L[i][i];
    }

    vector<double> x(n, 0);
    for (int i = n-1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i+1; j < n; j++) x[i] -= L[j][i] * x[j];
        x[i] /= L[i][i];
    }

    ofstream fL("Output/cholesky_L.txt");
    fL << n << " " << n << "\n";
    for (auto &row : L) { for (double v : row) fL << v << " "; fL << "\n"; }

    ofstream vecOut("Output/cholesky_vector.txt");
    SolutionPrinter::printDirect(x, "Cholesky Decomposition", vecOut);

    PlotHelper::saveAndPlot(x,
        "Output/cholesky_plot.dat",
        "Output/cholesky_plot.gnu",
        "Output/cholesky_graph.png",
        "Cholesky - Solution");
}