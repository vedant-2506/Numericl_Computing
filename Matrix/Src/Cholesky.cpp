#include "../Include/LUDecomposition.hpp"      // include ../Include/LUDecomposition.hpp
#include "../Include/PlotHelper.hpp"          // include ../Include/PlotHelper.hpp
#include "../Include/Solutionprinter.hpp"     // include ../Include/Solutionprinter.hpp
#include <fstream>                            // include fstream
#include <vector>                             // include vector
#include <cmath>                              // include cmath
#include <stdexcept>                          // include stdexcept
using namespace std;                              // use standard namespace

void LUDecomposition::choleskyDecomposition(const string &Afile, const string &bfile)  // define function
{ 
    string augFile = "Output/cholesky_augmented.txt";  // declare and assign variable
    LinearOperation linearOperation;                   // create helper object
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

    for (int i = 0; i < n; i++)                   // iterate over elements
        for (int j = 0; j < n; j++)               // iterate over elements
            if (abs(A[i][j] - A[j][i]) > 1e-9)    // check condition
                throw runtime_error("Cholesky: matrix is not symmetric");  // raise exception with error

    vector<vector<double>> L(n, vector<double>(n, 0));  // check equality or comparison
    for (int i = 0; i < n; i++)                   // iterate over elements
        for (int j = 0; j <= i; j++)              // iterate over elements
        { 
            double s = 0;                         // declare and assign variable
            for (int k = 0; k < j; k++) s += L[i][k] * L[j][k];  // iterate over elements
            if (i == j) {                        // check condition
                double v = A[i][i] - s;           // declare and assign variable
                if (v <= 0) throw runtime_error("Cholesky: not positive definite");  // check condition
                L[i][j] = sqrt(v);                // assign value to variable
            } else L[i][j] = (A[i][j] - s) / L[j][j];  // assign value to variable
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
        for (int j = i+1; j < n; j++) x[i] -= L[j][i] * x[j];  // iterate over elements
        x[i] /= L[i][i];                          // update variable with operation
    } 

    ofstream fL("Output/cholesky_L.txt");         // call function or method
    fL << n << " " << n << "\n";                  // check equality or comparison
    for (auto &row : L) { for (double v : row) fL << v << " "; fL << "\n"; }  // iterate over elements

    ofstream vecOut("Output/cholesky_vector.txt");  // call function or method
    SolutionPrinter solutionPrinter;                  // create printer object
    solutionPrinter.printDirect(x, "Cholesky Decomposition", vecOut);  // declare function

    PlotHelper plotHelper;                         // create plot helper object
    plotHelper.saveAndPlot(x,                    // define function
        "Output/cholesky_plot.dat",               // execute statement
        "Output/cholesky_plot.gnu",               // execute statement
        "Output/cholesky_graph.png",              // execute statement
        "Cholesky - Solution");                   // execute statement
}