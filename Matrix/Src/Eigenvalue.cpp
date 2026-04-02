#include "../Include/Eigenvalue.hpp"                  // include own header
#include <fstream>                                     // for ofstream, ifstream
#include <vector>                                      // for vector
#include <cmath>                                       // for fabs, sqrt
#include <iomanip>                                     // for setw, setprecision
#include <sstream>                                     // for ostringstream
#include <stdexcept>                                   // for runtime_error
using namespace std;                              // use standard namespace


EigenValue::EigenValue()                          // define function
    : Matrix() {}                                 // execute statement

EigenValue::EigenValue(int r, int c)              // define function
    : Matrix(r, c) {}                             // execute statement


ostream& operator<<(ostream &out, const EigenValue &ev)  // stream input/output operation
{
    out << "EigenValue(" << ev.rows << "x" << ev.cols << ")\n";  // check equality or comparison
    for (int i = 0; i < ev.rows; i++)             // iterate over elements
    {
        for (int j = 0; j < ev.cols; j++)         // iterate over elements
            out << ev.data[i][j] << " ";          // check equality or comparison
        out << "\n";                              // check equality or comparison
    }
    return out;                                   // return result from function
}


void loadMatrix(const string &Afile,              // declare and initialize object
    vector<vector<double>> &A, int &n)            // declare variable
{
    ifstream fin(Afile);                          // handle file stream operation
    if (!fin) throw runtime_error("Cannot open file: " + Afile);  // check condition
    int cols;                                     // declare variable
    fin >> n >> cols;                             // check equality or comparison
    if (n != cols) throw runtime_error("EigenValue: matrix must be square");  // check condition
    A.assign(n, vector<double>(n));               // check equality or comparison
    for (int i = 0; i < n; i++)                   // iterate over elements
        for (int j = 0; j < n; j++)               // iterate over elements
            fin >> A[i][j];                       // check equality or comparison
}


void EigenValue::gerschgorin(const string &Afile) // define function
{
    vector<vector<double>> A;                     // check equality or comparison
    int n;                                        // declare variable
    loadMatrix(Afile, A, n);                      // call function or method

    ofstream out("Output/eigen_gerschgorin.txt"); // call function or method
    if (!out) throw runtime_error("Cannot open Output/eigen_gerschgorin.txt");  // check condition

    string title = "================================================\n"  // declare and assign variable
                   "         GERSCHGORIN CIRCLE THEOREM            \n"  // execute statement
                   "================================================";  // check equality or comparison
    cout << "\n" << title << "\n";                // check equality or comparison
    out  << title << "\n\n";                      // check equality or comparison

    cout << "\nInput Matrix A (" << n << "x" << n << "):\n";  // check equality or comparison
    out  << "Input Matrix A (" << n << "x" << n << "):\n";  // check equality or comparison
    for (int i = 0; i < n; i++)                   // iterate over elements
    {
        for (int j = 0; j < n; j++)               // iterate over elements
        {
            cout << setw(12) << fixed << setprecision(4) << A[i][j];  // check equality or comparison
            out  << setw(12) << fixed << setprecision(4) << A[i][j];  // check equality or comparison
        }
        cout << "\n"; out << "\n";                // check equality or comparison
    }

    cout << "\n--- Gerschgorin Discs (one per row) ---\n";  // check equality or comparison
    out  << "\n--- Gerschgorin Discs (one per row) ---\n";  // check equality or comparison

    string sep = "+-------+----------------+----------------+------------------------+------------------------+";  // declare and assign variable
    string hdr = "| Row   |    Centre      |    Radius      |      Lower Bound       |      Upper Bound       |";  // declare and assign variable
    cout << sep << "\n" << hdr << "\n" << sep << "\n";  // check equality or comparison
    out  << sep << "\n" << hdr << "\n" << sep << "\n";  // check equality or comparison

    double globalLow  =  1e300;                   // declare and assign variable
    double globalHigh = -1e300;                   // declare and assign variable

    vector<double> centres(n), radii(n);          // check equality or comparison

    for (int i = 0; i < n; i++)                   // iterate over elements
    {
        double centre = A[i][i];                  // declare and assign variable
        double radius = 0.0;                      // declare and assign variable
        for (int j = 0; j < n; j++)               // iterate over elements
            if (j != i) radius += fabs(A[i][j]);  // check condition

        double lower = centre - radius;           // declare and assign variable
        double upper = centre + radius;           // declare and assign variable

        centres[i] = centre;                      // assign value to variable
        radii[i]   = radius;                      // assign value to variable

        if (lower < globalLow)  globalLow  = lower;  // check condition
        if (upper > globalHigh) globalHigh = upper;  // check condition

        ostringstream row;                        // execute statement
        row << "| " << setw(5) << (i+1) << " |"   // stream input/output operation
            << setw(14) << fixed << setprecision(5) << centre << "  |"  // stream input/output operation
            << setw(14) << fixed << setprecision(5) << radius << "  |"  // stream input/output operation
            << setw(22) << fixed << setprecision(5) << lower  << "  |"  // stream input/output operation
            << setw(22) << fixed << setprecision(5) << upper  << "  |";  // check equality or comparison
        cout << row.str() << "\n";                // check equality or comparison
        out  << row.str() << "\n";                // check equality or comparison
    }
    cout << sep << "\n";                          // check equality or comparison
    out  << sep << "\n";                          // check equality or comparison

    cout << "\n--- Eigenvalue Bounding Region (Union of All Discs) ---\n";  // check equality or comparison
    out  << "\n--- Eigenvalue Bounding Region (Union of All Discs) ---\n";  // check equality or comparison

    ostringstream union_line;                     // execute statement
    union_line << "  All eigenvalues lie within: [ "  // stream input/output operation
               << fixed << setprecision(5) << globalLow  // stream input/output operation
               << " , "                           // stream input/output operation
               << fixed << setprecision(5) << globalHigh  // stream input/output operation
               << " ]\n";                         // check equality or comparison
    union_line << "  Interval width: "            // stream input/output operation
               << fixed << setprecision(5) << (globalHigh - globalLow) << "\n";  // check equality or comparison
    cout << union_line.str();                     // check equality or comparison
    out  << union_line.str();                     // check equality or comparison

    cout << "\n--- Disc Details ---\n";           // check equality or comparison
    out  << "\n--- Disc Details ---\n";           // check equality or comparison
    for (int i = 0; i < n; i++)                   // iterate over elements
    {
        ostringstream d;                          // execute statement
        d << "  Disc " << (i+1) << ": centre = "  // stream input/output operation
          << fixed << setprecision(5) << centres[i]  // stream input/output operation
          << ", radius = "                        // stream input/output operation
          << fixed << setprecision(5) << radii[i] // stream input/output operation
          << "  =>  eigenvalue in ["              // stream input/output operation
          << fixed << setprecision(5) << (centres[i] - radii[i])  // stream input/output operation
          << ", "                                 // stream input/output operation
          << fixed << setprecision(5) << (centres[i] + radii[i])  // stream input/output operation
          << "]\n";                               // check equality or comparison
        cout << d.str();                          // check equality or comparison
        out  << d.str();                          // check equality or comparison
    }

    ofstream dat("Output/eigen_gerschgorin_plot.dat");  // call function or method
    for (int i = 0; i < n; i++)                   // iterate over elements
        dat << (i+1) << " " << centres[i] << " " << radii[i]  // stream input/output operation
            << " " << (centres[i]-radii[i]) << " " << (centres[i]+radii[i]) << "\n";  // check equality or comparison
    dat.close();                                  // call function or method

    ofstream gnu("Output/eigen_gerschgorin_plot.gnu");  // call function or method
    gnu << "set terminal png size 900,600\n";     // check equality or comparison
    gnu << "set output 'Output/eigen_gerschgorin_graph.png'\n";  // check equality or comparison
    gnu << "set title 'Gerschgorin Discs — Eigenvalue Bounds'\n";  // check equality or comparison
    gnu << "set xlabel 'Row Index'\n";            // check equality or comparison
    gnu << "set ylabel 'Value'\n";                // check equality or comparison
    gnu << "set grid\n";                          // check equality or comparison
    gnu << "plot 'Output/eigen_gerschgorin_plot.dat' using 1:2 with linespoints lw 2 pt 7 title 'Centre', \\\n";  // check equality or comparison
    gnu << "     'Output/eigen_gerschgorin_plot.dat' using 1:4 with linespoints lw 1 pt 5 dt 2 title 'Lower Bound', \\\n";  // check equality or comparison
    gnu << "     'Output/eigen_gerschgorin_plot.dat' using 1:5 with linespoints lw 1 pt 5 dt 3 title 'Upper Bound'\n";  // check equality or comparison
    gnu.close();                                  // call function or method
    system("gnuplot Output/eigen_gerschgorin_plot.gnu");  // call function or method

    cout << "\n[OUTPUT] Gerschgorin results -> Output/eigen_gerschgorin.txt\n";  // check equality or comparison
    cout <<   "[OUTPUT] Graph               -> Output/eigen_gerschgorin_graph.png\n";  // check equality or comparison
}
