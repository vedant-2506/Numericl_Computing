#include "../Include/GaussianElimination.hpp" // include ../Include/GaussianElimination.hpp
#include "../Include/PlotHelper.hpp" // include ../Include/PlotHelper.hpp
#include "../Include/Solutionprinter.hpp" // include ../Include/Solutionprinter.hpp
#include <vector> // include vector
#include <cmath> // include cmath
#include <stdexcept> // include stdexcept
using namespace std;                              // use standard namespace

GaussianElimination::GaussianElimination() : LinearOperation() {}  // define function
GaussianElimination::GaussianElimination(int r, int c) : LinearOperation(r,c){}  // define function

ostream& operator<<(ostream &out, const GaussianElimination &ge)  // stream input/output operation
{ out << "GaussianElimination object\n"; return out; }  // stream input/output operation

void GaussianElimination::gaussianWithPivoting(   // define function
    Matrix &matrix, ofstream &matrixOut, ofstream &vectorOut)  // handle file stream operation
{
    int n   = matrix.getRows();                   // declare and initialize object
    auto &A = matrix.getData();                   // declare and initialize object

    for (int k = 0; k < n-1; k++)                 // iterate over elements
    {
        int mx = k;                               // declare and assign variable
        for (int i = k+1; i < n; i++)             // iterate over elements
            if (abs(A[i][k]) > abs(A[mx][k])) mx = i;  // check condition
        swap(A[k], A[mx]);                        // call function or method
        if (abs(A[k][k]) < 1e-12)                 // check condition
            throw runtime_error("Singular matrix: zero pivot");  // raise exception with error
        for (int i = k+1; i < n; i++)             // iterate over elements
        {
            double f = A[i][k] / A[k][k];         // declare and assign variable
            for (int j = k; j < n+1; j++)         // iterate over elements
                A[i][j] -= f * A[k][j];           // update variable with operation
        }
    }

    matrixOut << n << " " << n+1 << "\n";         // check equality or comparison
    for (int i = 0; i < n; i++) {                 // iterate over elements
        for (int j = 0; j < n+1; j++) matrixOut << A[i][j] << " ";  // iterate over elements
        matrixOut << "\n";                        // check equality or comparison
    }

    vector<double> x(n);                          // check equality or comparison
    for (int i = n-1; i >= 0; i--) {              // iterate over elements
        x[i] = A[i][n];                           // assign value to variable
        for (int j = i+1; j < n; j++) x[i] -= A[i][j] * x[j];  // iterate over elements
        x[i] /= A[i][i];                          // update variable with operation
    }

    SolutionPrinter solutionPrinter;              // create printer object
    solutionPrinter.printDirect(x, "Gaussian Elimination WITH Pivoting", vectorOut);  // declare function

    PlotHelper plotHelper;                        // create plot helper object
    plotHelper.saveAndPlot(x,                    // define function
        "Output/gaussian_pivot_plot.dat",         // execute statement
        "Output/gaussian_pivot_plot.gnu",         // execute statement
        "Output/gaussian_pivot_graph.png",        // execute statement
        "Gaussian Elimination WITH Pivoting");    // execute statement
}

void GaussianElimination::gaussianWithoutPivoting(  // define function
    Matrix &matrix, ofstream &matrixOut, ofstream &vectorOut)  // handle file stream operation
{
    int n   = matrix.getRows();                   // declare and initialize object
    auto &A = matrix.getData();                   // declare and initialize object

    for (int k = 0; k < n-1; k++)                 // iterate over elements
    {
        if (abs(A[k][k]) < 1e-12)                 // check condition
            throw runtime_error("Zero pivot: try Gaussian WITH pivoting instead");  // raise exception with error
        for (int i = k+1; i < n; i++)             // iterate over elements
        {
            double f = A[i][k] / A[k][k];         // declare and assign variable
            for (int j = k; j < n+1; j++)         // iterate over elements
                A[i][j] -= f * A[k][j];           // update variable with operation
        }
    }

    matrixOut << n << " " << n+1 << "\n";         // check equality or comparison
    for (int i = 0; i < n; i++) {                 // iterate over elements
        for (int j = 0; j < n+1; j++) matrixOut << A[i][j] << " ";  // iterate over elements
        matrixOut << "\n";                        // check equality or comparison
    }

    vector<double> x(n);                          // check equality or comparison
    for (int i = n-1; i >= 0; i--) {              // iterate over elements
        x[i] = A[i][n];                           // assign value to variable
        for (int j = i+1; j < n; j++) x[i] -= A[i][j] * x[j];  // iterate over elements
        x[i] /= A[i][i];                          // update variable with operation
    }

    SolutionPrinter solutionPrinter;              // create printer object
    solutionPrinter.printDirect(x, "Gaussian Elimination WITHOUT Pivoting", vectorOut);  // declare function

    PlotHelper plotHelper;                        // create plot helper object
    plotHelper.saveAndPlot(x,                    // define function
        "Output/gaussian_nopivot_plot.dat",       // execute statement
        "Output/gaussian_nopivot_plot.gnu",       // execute statement
        "Output/gaussian_nopivot_graph.png",      // execute statement
        "Gaussian Elimination WITHOUT Pivoting"); // execute statement
}