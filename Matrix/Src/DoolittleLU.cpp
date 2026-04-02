#include "../Include/LUDecomposition.hpp" // include ../Include/LUDecomposition.hpp
#include "../Include/PlotHelper.hpp" // include ../Include/PlotHelper.hpp
#include "../Include/Solutionprinter.hpp" // include ../Include/Solutionprinter.hpp
#include <fstream> // include fstream
#include <vector> // include vector
using namespace std;                              // use standard namespace

LUDecomposition::LUDecomposition() : LinearOperation() {}  // define function
LUDecomposition::LUDecomposition(int r, int c) : LinearOperation(r, c) {}  // define function

ostream& operator<<(ostream &out, const LUDecomposition &lu)  // stream input/output operation
{ out << "LUDecomposition object\n"; return out; }  // stream input/output operation

void writeDoolittleMatrix(const string &path, int n,  // declare and initialize object
    const vector<vector<double>> &M)              // stream input/output operation
{
    ofstream f(path);                             // call function or method
    f << n << " " << n << "\n";                   // check equality or comparison
    for (auto &row : M) { for (double v : row) f << v << " "; f << "\n"; }  // iterate over elements
}

void LUDecomposition::doolittleLU(const string &Afile, const string &bfile)  // define function
{
    string augFile = "Output/doolittle_augmented.txt";  // declare and assign variable
    LinearOperation linearOperation;                     // create helper object
    linearOperation.generateAugmentedMatrixFile(Afile, bfile, augFile);  // declare function
    ifstream fin(augFile);                        // handle file stream operation
    int n, c; fin >> n >> c;                      // declare variable
    vector<vector<double>> A(n, vector<double>(n));  // check equality or comparison
    vector<double> b(n);                          // check equality or comparison
    for (int i = 0; i < n; i++) {                 // iterate over elements
        for (int j = 0; j < n; j++) fin >> A[i][j];  // iterate over elements
        fin >> b[i];                              // check equality or comparison
    }
    fin.close();                                  // call function or method

    vector<vector<double>> L(n, vector<double>(n, 0));  // check equality or comparison
    vector<vector<double>> U(n, vector<double>(n, 0));  // check equality or comparison
    for (int i = 0; i < n; i++) {                 // iterate over elements
        for (int k = i; k < n; k++) {             // iterate over elements
            double s = 0;                         // declare and assign variable
            for (int j = 0; j < i; j++) s += L[i][j] * U[j][k];  // iterate over elements
            U[i][k] = A[i][k] - s;                // assign value to variable
        }
        for (int k = i; k < n; k++) {             // iterate over elements
            if (i == k) { L[i][i] = 1; continue; }  // check condition
            double s = 0;                         // declare and assign variable
            for (int j = 0; j < i; j++) s += L[k][j] * U[j][i];  // iterate over elements
            L[k][i] = (A[k][i] - s) / U[i][i];    // assign value to variable
        }
    }

    vector<double> y(n, 0);                       // check equality or comparison
    for (int i = 0; i < n; i++) {                 // iterate over elements
        y[i] = b[i];                              // assign value to variable
        for (int j = 0; j < i; j++) y[i] -= L[i][j] * y[j];  // iterate over elements
    }

    vector<double> x(n, 0);                       // check equality or comparison
    for (int i = n-1; i >= 0; i--) {              // iterate over elements
        x[i] = y[i];                              // assign value to variable
        for (int j = i+1; j < n; j++) x[i] -= U[i][j] * x[j];  // iterate over elements
        x[i] /= U[i][i];                          // update variable with operation
    }

    writeDoolittleMatrix("Output/doolittle_L.txt", n, L);  // call function or method
    writeDoolittleMatrix("Output/doolittle_U.txt", n, U);  // call function or method

    ofstream vecOut("Output/doolittle_vector.txt");  // call function or method
    SolutionPrinter solutionPrinter;                     // create printer object
    solutionPrinter.printDirect(x, "Doolittle LU Decomposition", vecOut);  // declare function

    PlotHelper plotHelper;                          // create plot helper object
    plotHelper.saveAndPlot(x,                    // define function
        "Output/doolittle_plot.dat",              // execute statement
        "Output/doolittle_plot.gnu",              // execute statement
        "Output/doolittle_graph.png",             // execute statement
        "Doolittle LU - Solution");               // execute statement
}