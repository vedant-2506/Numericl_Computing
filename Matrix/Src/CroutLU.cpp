#include "../Include/LUDecomposition.hpp" // include LU header
#include "../Include/PlotHelper.hpp"       // include plot helper
#include <fstream>                         // for ofstream
#include <vector>                          // for vector
using namespace std;                       // use standard namespace

static void writeMatrix(const string &path, int n,
    const vector<vector<double>> &M)       // helper: write matrix to file
{
    ofstream f(path);
    f << n << " " << n << "\n";
    for (auto &row : M) { for (double v : row) f << v << " "; f << "\n"; }
}

void LUDecomposition::croutLU(const string &Afile, const string &bfile)
{
    // build and load augmented matrix [A|b]
    string augFile = "Output/crout_augmented.txt";
    LinearOperation::generateAugmentedMatrixFile(Afile, bfile, augFile);
    ifstream fin(augFile);
    int n, c; fin >> n >> c;
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) fin >> A[i][j];
        fin >> b[i];
    }
    fin.close();

    // Crout decomposition: U has 1s on diagonal
    vector<vector<double>> L(n, vector<double>(n, 0)); // lower triangular
    vector<vector<double>> U(n, vector<double>(n, 0)); // upper triangular
    for (int j = 0; j < n; j++)
    {
        U[j][j] = 1;                           // diagonal of U = 1
        for (int i = j; i < n; i++)            // compute L column j
        {
            double s = 0;
            for (int k = 0; k < j; k++) s += L[i][k] * U[k][j];
            L[i][j] = A[i][j] - s;
        }
        for (int i = j+1; i < n; i++)          // compute U row j
        {
            double s = 0;
            for (int k = 0; k < j; k++) s += L[j][k] * U[k][i];
            U[j][i] = (A[j][i] - s) / L[j][j];
        }
    }

    // forward substitution: Ly = b
    vector<double> y(n, 0);
    for (int i = 0; i < n; i++)
    {
        y[i] = b[i];
        for (int j = 0; j < i; j++) y[i] -= L[i][j] * y[j];
        y[i] /= L[i][i];                       // divide by L diagonal
    }

    // back substitution: Ux = y (U diagonal = 1, no division)
    vector<double> x(n, 0);
    for (int i = n-1; i >= 0; i--)
    {
        x[i] = y[i];
        for (int j = i+1; j < n; j++) x[i] -= U[i][j] * x[j];
    }

    // save outputs
    writeMatrix("Output/crout_L.txt", n, L);
    writeMatrix("Output/crout_U.txt", n, U);
    ofstream fx("Output/crout_vector.txt");
    fx << n << " 1\n";
    for (double v : x) fx << v << "\n";

    PlotHelper::saveAndPlot(x,
        "Output/crout_plot.dat",
        "Output/crout_plot.gnu",
        "Output/crout_graph.png",
        "Crout LU - Solution");
}