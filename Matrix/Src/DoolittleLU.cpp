#include "../Include/LUDecomposition.hpp"
#include "../Include/PlotHelper.hpp"
#include "../Include/Solutionprinter.hpp"
#include <fstream>
#include <vector>
using namespace std;

LUDecomposition::LUDecomposition() : LinearOperation() {}
LUDecomposition::LUDecomposition(int r, int c) : LinearOperation(r, c) {}

ostream& operator<<(ostream &out, const LUDecomposition &lu)
{ out << "LUDecomposition object\n"; return out; }

void writeDoolittleMatrix(const string &path, int n,
    const vector<vector<double>> &M)
{
    ofstream f(path);
    f << n << " " << n << "\n";
    for (auto &row : M) { for (double v : row) f << v << " "; f << "\n"; }
}

void LUDecomposition::doolittleLU(const string &Afile, const string &bfile)
{
    string augFile = "Output/doolittle_augmented.txt";
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
    for (int i = 0; i < n; i++) {
        for (int k = i; k < n; k++) {
            double s = 0;
            for (int j = 0; j < i; j++) s += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - s;
        }
        for (int k = i; k < n; k++) {
            if (i == k) { L[i][i] = 1; continue; }
            double s = 0;
            for (int j = 0; j < i; j++) s += L[k][j] * U[j][i];
            L[k][i] = (A[k][i] - s) / U[i][i];
        }
    }

    vector<double> y(n, 0);
    for (int i = 0; i < n; i++) {
        y[i] = b[i];
        for (int j = 0; j < i; j++) y[i] -= L[i][j] * y[j];
    }

    vector<double> x(n, 0);
    for (int i = n-1; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i+1; j < n; j++) x[i] -= U[i][j] * x[j];
        x[i] /= U[i][i];
    }

    writeDoolittleMatrix("Output/doolittle_L.txt", n, L);
    writeDoolittleMatrix("Output/doolittle_U.txt", n, U);

    ofstream vecOut("Output/doolittle_vector.txt");
    SolutionPrinter solutionPrinter;
    solutionPrinter.printDirect(x, "Doolittle LU Decomposition", vecOut);

    PlotHelper plotHelper;
    plotHelper.saveAndPlot(x,
        "Output/doolittle_plot.dat",
        "Output/doolittle_plot.gnu",
        "Output/doolittle_graph.png",
        "Doolittle LU - Solution");
}