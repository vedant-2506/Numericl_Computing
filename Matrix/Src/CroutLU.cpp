#include "../Include/LUDecomposition.hpp" // include ../Include/LUDecomposition.hpp
#include "../Include/PlotHelper.hpp" // include ../Include/PlotHelper.hpp
#include "../Include/Solutionprinter.hpp" // include ../Include/Solutionprinter.hpp
#include <fstream> // include fstream
#include <vector> // include vector
using namespace std;                              // use standard namespace

void writeCroutMatrix(const string &path, int n,    // declare and initialize object
    const vector<vector<double>> &M)              // stream input/output operation
{
    ofstream f(path);                             // call function or method
    f << n << " " << n << "\n";                   // check equality or comparison
    for (auto &row : M) { for (double v : row) f << v << " "; f << "\n"; }  // iterate over elements
}

void LUDecomposition::croutLU(const string &Afile, const string &bfile)  // define function
{
    string augFile = "Output/crout_augmented.txt";  // declare and assign variable
    LinearOperation linearOperation;                // create helper object
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
    for (int j = 0; j < n; j++) {                 // iterate over elements
        U[j][j] = 1;                              // assign value to variable
        for (int i = j; i < n; i++) {             // iterate over elements
            double s = 0;                         // declare and assign variable
            for (int k = 0; k < j; k++) s += L[i][k] * U[k][j];  // iterate over elements
            L[i][j] = A[i][j] - s;                // assign value to variable
        }
        for (int i = j+1; i < n; i++) {           // iterate over elements
            double s = 0;                         // declare and assign variable
            for (int k = 0; k < j; k++) s += L[j][k] * U[k][i];  // iterate over elements
            U[j][i] = (A[j][i] - s) / L[j][j];    // assign value to variable
        }
    }

    vector<double> y(n, 0);                       // check equality or comparison
    for (int i = 0; i < n; i++) {                 // iterate over elements
        y[i] = b[i];                              // assign value to variable
        for (int j = 0; j < i; j++) y[i] -= L[i][j] * y[j];  // iterate over elements
        y[i] /= L[i][i];                          // update variable with operation
    }

    vector<double> x(n, 0);                       // check equality or comparison
    for (int i = n-1; i >= 0; i--) {              // iterate over elements
        x[i] = y[i];                              // assign value to variable
        for (int j = i+1; j < n; j++) x[i] -= U[i][j] * x[j];  // iterate over elements
    }

    writeCroutMatrix("Output/crout_L.txt", n, L); // call function or method
    writeCroutMatrix("Output/crout_U.txt", n, U); // call function or method

    ofstream vecOut("Output/crout_vector.txt");   // call function or method
    SolutionPrinter solutionPrinter;               // create printer object
    solutionPrinter.printDirect(x, "Crout LU Decomposition", vecOut);  // declare function

    PlotHelper plotHelper;                        // create plot helper object
    plotHelper.saveAndPlot(x,                    // define function
        "Output/crout_plot.dat",                  // execute statement
        "Output/crout_plot.gnu",                  // execute statement
        "Output/crout_graph.png",                 // execute statement
        "Crout LU - Solution");                   // execute statement
}