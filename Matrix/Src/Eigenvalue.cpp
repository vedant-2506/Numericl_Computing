#include "../Include/Eigenvalue.hpp"  // Include eigenvalue header
#include <fstream>                       // File I/O
#include <vector>                        // Dynamic vectors
#include <cmath>                         // Math functions
#include <iomanip>                       // Output formatting
#include <sstream>                       // String streams
#include <stdexcept>                     // Exception handling
using namespace std;

EigenValue::EigenValue()                 // Default constructor
    : Matrix() {}

EigenValue::EigenValue(int r, int c)     // Constructor with dimensions
    : Matrix(r, c) {}


ostream& operator<<(ostream &out, const EigenValue &ev)
{
    out << "EigenValue(" << ev.rows << "x" << ev.cols << ")\n";
    for (int i = 0; i < ev.rows; i++)
    {
        for (int j = 0; j < ev.cols; j++)
            out << ev.data[i][j] << " ";
        out << "\n";
    }
    return out;
}


void loadMatrix(const string &Afile,
    vector<vector<double>> &A, int &n)
{
    ifstream fin(Afile);
    if (!fin) throw runtime_error("Cannot open file: " + Afile);
    int cols;
    fin >> n >> cols;
    if (n != cols) throw runtime_error("EigenValue: matrix must be square");
    A.assign(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fin >> A[i][j];
}


void EigenValue::gerschgorin(const string &Afile)
{
    vector<vector<double>> A;
    int n;
    loadMatrix(Afile, A, n);

    ofstream out("Output/eigen_gerschgorin.txt");
    if (!out) throw runtime_error("Cannot open Output/eigen_gerschgorin.txt");

    string title = "================================================\n"
                   "         GERSCHGORIN CIRCLE THEOREM            \n"
                   "================================================";
    cout << "\n" << title << "\n";
    out  << title << "\n\n";

    cout << "\nInput Matrix A (" << n << "x" << n << "):\n";
    out  << "Input Matrix A (" << n << "x" << n << "):\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << setw(12) << fixed << setprecision(4) << A[i][j];
            out  << setw(12) << fixed << setprecision(4) << A[i][j];
        }
        cout << "\n"; out << "\n";
    }

    cout << "\n--- Gerschgorin Discs (one per row) ---\n";
    out  << "\n--- Gerschgorin Discs (one per row) ---\n";

    string sep = "+-------+----------------+----------------+------------------------+------------------------+";
    string hdr = "| Row   |    Centre      |    Radius      |      Lower Bound       |      Upper Bound       |";
    cout << sep << "\n" << hdr << "\n" << sep << "\n";
    out  << sep << "\n" << hdr << "\n" << sep << "\n";

    double globalLow  =  1e300;
    double globalHigh = -1e300;

    vector<double> centres(n), radii(n);

    for (int i = 0; i < n; i++)
    {
        double centre = A[i][i];
        double radius = 0.0;
        for (int j = 0; j < n; j++)
            if (j != i) radius += fabs(A[i][j]);

        double lower = centre - radius;
        double upper = centre + radius;

        centres[i] = centre;
        radii[i]   = radius;

        if (lower < globalLow)  globalLow  = lower;
        if (upper > globalHigh) globalHigh = upper;

        ostringstream row;
        row << "| " << setw(5) << (i+1) << " |"
            << setw(14) << fixed << setprecision(5) << centre << "  |"
            << setw(14) << fixed << setprecision(5) << radius << "  |"
            << setw(22) << fixed << setprecision(5) << lower  << "  |"
            << setw(22) << fixed << setprecision(5) << upper  << "  |";
        cout << row.str() << "\n";
        out  << row.str() << "\n";
    }
    cout << sep << "\n";
    out  << sep << "\n";

    cout << "\n--- Eigenvalue Bounding Region (Union of All Discs) ---\n";
    out  << "\n--- Eigenvalue Bounding Region (Union of All Discs) ---\n";

    ostringstream union_line;
    union_line << "  All eigenvalues lie within: [ "
               << fixed << setprecision(5) << globalLow
               << " , "
               << fixed << setprecision(5) << globalHigh
               << " ]\n";
    union_line << "  Interval width: "
               << fixed << setprecision(5) << (globalHigh - globalLow) << "\n";
    cout << union_line.str();
    out  << union_line.str();

    cout << "\n--- Disc Details ---\n";
    out  << "\n--- Disc Details ---\n";
    for (int i = 0; i < n; i++)
    {
        ostringstream d;
        d << "  Disc " << (i+1) << ": centre = "
          << fixed << setprecision(5) << centres[i]
          << ", radius = "
          << fixed << setprecision(5) << radii[i]
          << "  =>  eigenvalue in ["
          << fixed << setprecision(5) << (centres[i] - radii[i])
          << ", "
          << fixed << setprecision(5) << (centres[i] + radii[i])
          << "]\n";
        cout << d.str();
        out  << d.str();
    }

    ofstream dat("Output/eigen_gerschgorin_plot.dat");
    for (int i = 0; i < n; i++)
        dat << (i+1) << " " << centres[i] << " " << radii[i]
            << " " << (centres[i]-radii[i]) << " " << (centres[i]+radii[i]) << "\n";
    dat.close();

    ofstream gnu("Output/eigen_gerschgorin_plot.gnu");
    gnu << "set terminal png size 900,600\n";
    gnu << "set output 'Output/eigen_gerschgorin_graph.png'\n";
    gnu << "set title 'Gerschgorin Discs — Eigenvalue Bounds'\n";
    gnu << "set xlabel 'Row Index'\n";
    gnu << "set ylabel 'Value'\n";
    gnu << "set grid\n";
    gnu << "plot 'Output/eigen_gerschgorin_plot.dat' using 1:2 with linespoints lw 2 pt 7 title 'Centre', \\\n";
    gnu << "     'Output/eigen_gerschgorin_plot.dat' using 1:4 with linespoints lw 1 pt 5 dt 2 title 'Lower Bound', \\\n";
    gnu << "     'Output/eigen_gerschgorin_plot.dat' using 1:5 with linespoints lw 1 pt 5 dt 3 title 'Upper Bound'\n";
    gnu.close();
    system("gnuplot Output/eigen_gerschgorin_plot.gnu");

    cout << "\n[OUTPUT] Gerschgorin results -> Output/eigen_gerschgorin.txt\n";
    cout <<   "[OUTPUT] Graph               -> Output/eigen_gerschgorin_graph.png\n";
}
