#include "../Include/LUDecomposition.hpp" // include LU header
#include "../Include/PlotHelper.hpp"       // include plot helper
#include <fstream>                         // for ofstream
#include <vector>                          // for vector
using namespace std;                       // use standard namespace

// ---- CONSTRUCTORS (defined here once for all LU methods) ----

LUDecomposition::LUDecomposition()             // default constructor
    : LinearOperation() {}                     // call parent constructor

LUDecomposition::LUDecomposition(int r, int c) // parameterized constructor
    : LinearOperation(r, c) {}                 // call parent constructor

// ---- FRIEND OPERATOR ----

ostream& operator<<(ostream &out, const LUDecomposition &lu) // print to stream
{
    out << "LUDecomposition object\n";         // print label
    return out;                                // return stream
}

// ---- HELPER LAMBDA to write n x n matrix to file ----
// (defined as a free function to avoid repeating in each method)

static void writeMatrix(const string &path, int n,
    const vector<vector<double>> &M)           // write matrix M to file at path
{
    ofstream f(path);                          // open output file
    f << n << " " << n << "\n";               // write dimensions
    for (auto &row : M)                        // loop rows
    {
        for (double v : row) f << v << " ";    // write each element
        f << "\n";                             // newline after row
    }
}

// ---- DOOLITTLE METHOD ----

void LUDecomposition::doolittleLU(const string &Afile, const string &bfile)
{
    // build and load augmented matrix [A|b]
    string augFile = "Output/doolittle_augmented.txt";
    LinearOperation::generateAugmentedMatrixFile(Afile, bfile, augFile);
    ifstream fin(augFile);                     // open augmented file
    int n, c; fin >> n >> c;                   // read dimensions
    vector<vector<double>> A(n, vector<double>(n)); // matrix A
    vector<double> b(n);                        // vector b
    for (int i = 0; i < n; i++)                // read rows
    {
        for (int j = 0; j < n; j++) fin >> A[i][j]; // read A
        fin >> b[i];                            // read b
    }
    fin.close();

    // Doolittle decomposition: L has 1s on diagonal
    vector<vector<double>> L(n, vector<double>(n, 0)); // lower triangular
    vector<vector<double>> U(n, vector<double>(n, 0)); // upper triangular
    for (int i = 0; i < n; i++)
    {
        for (int k = i; k < n; k++)            // compute U row i
        {
            double s = 0;
            for (int j = 0; j < i; j++) s += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - s;
        }
        for (int k = i; k < n; k++)            // compute L column i
        {
            if (i == k) { L[i][i] = 1; continue; } // diagonal of L = 1
            double s = 0;
            for (int j = 0; j < i; j++) s += L[k][j] * U[j][i];
            L[k][i] = (A[k][i] - s) / U[i][i];
        }
    }

    // forward substitution: Ly = b
    vector<double> y(n, 0);
    for (int i = 0; i < n; i++)
    {
        y[i] = b[i];
        for (int j = 0; j < i; j++) y[i] -= L[i][j] * y[j]; // L diagonal = 1
    }

    // back substitution: Ux = y
    vector<double> x(n, 0);
    for (int i = n-1; i >= 0; i--)
    {
        x[i] = y[i];
        for (int j = i+1; j < n; j++) x[i] -= U[i][j] * x[j];
        x[i] /= U[i][i];
    }

    // save outputs
    writeMatrix("Output/doolittle_L.txt", n, L); // save L matrix
    writeMatrix("Output/doolittle_U.txt", n, U); // save U matrix
    ofstream fx("Output/doolittle_vector.txt");  // save solution
    fx << n << " 1\n";
    for (double v : x) fx << v << "\n";

    PlotHelper::saveAndPlot(x,
        "Output/doolittle_plot.dat",
        "Output/doolittle_plot.gnu",
        "Output/doolittle_graph.png",
        "Doolittle LU - Solution");
}