#include "../Include/LUDecomposition.hpp"
#include "../Include/PlotHelper.hpp"
#include "../Include/Solutionprinter.hpp"
#include <fstream>
#include <vector>
using namespace std;

void writeCroutMatrix(const string &path, int n,
    const vector<vector<double>> &M)
{
    ofstream f(path);
    f << n << " " << n << "\n";
    for (auto &row : M) { for (double v : row) f << v << " "; f << "\n"; }
}

void LUDecomposition::croutLU(const string &Afile, const string &bfile)
{
    string augFile = "Output/crout_augmented.txt";
    LinearOperation linearOperation;
    linearOperation.generateAugmentedMatrixFile(Afile, bfile, augFile);
    ifstream fin(augFile);
    int n, c; fin >> n >> c;
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) fin >> A[i][j];
        fin >> b[i];
    }
    fin.close();

    vector<vector<double>> L(n, vector<double>(n, 0));
    vector<vector<double>> U(n, vector<double>(n, 0));
    for (int j = 0; j < n; j++) {
        U[j][j] = 1;
        for (int i = j; i < n; i++) {
            double s = 0;
            for (int k = 0; k < j; k++) s += L[i][k] * U[k][j];
            L[i][j] = A[i][j] - s;
        }
        for (int i = j+1; i < n; i++) {
            double s = 0;
            for (int k = 0; k < j; k++) s += L[j][k] * U[k][i];
            U[j][i] = (A[j][i] - s) / L[j][j];
        }
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
        for (int j = i+1; j < n; j++) x[i] -= U[i][j] * x[j];
    }

    writeCroutMatrix("Output/crout_L.txt", n, L);
    writeCroutMatrix("Output/crout_U.txt", n, U);

    ofstream vecOut("Output/crout_vector.txt");
    SolutionPrinter solutionPrinter;
    solutionPrinter.printDirect(x, "Crout LU Decomposition", vecOut);

    PlotHelper plotHelper;
    plotHelper.saveAndPlot(x,
        "Output/crout_plot.dat",
        "Output/crout_plot.gnu",
        "Output/crout_graph.png",
        "Crout LU - Solution");
}