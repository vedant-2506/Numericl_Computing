#include "../Include/LeastSquareParabola.hpp"
#include "../Include/Matrix.hpp"
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <cstdlib>

using namespace std;

// ─────────────────────────────────────────────
// Constructors
// ─────────────────────────────────────────────
LeastSquareParabola::LeastSquareParabola() : CurveFitting() {}

LeastSquareParabola::LeastSquareParabola(const Matrix &m) : CurveFitting(m) {}

// ─────────────────────────────────────────────
// fit()  –  build Vandermonde system and solve
// ─────────────────────────────────────────────
vector<double> LeastSquareParabola::fit(int degree, ofstream &fout)
{
    validateSamplePoints();                          // Ensure cols==2, rows>=2

    int n = rows;
    if (n <= degree)
        throw runtime_error("LeastSquareParabola::fit: need more data points than polynomial degree.");

    // Build design matrix A (n x (degree+1)):  A[i][j] = x_i^j
    Matrix A(n, degree + 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= degree; j++)
            A.set(i, j, pow(data[i][0], j));

    // Build column vector Y (n x 1)
    Matrix Y(n, 1);
    for (int i = 0; i < n; i++)
        Y.set(i, 0, data[i][1]);

    // Normal equations:  (A^T A) c = A^T Y
    Matrix AT  = A.transpose();
    Matrix ATA = AT * A;
    Matrix ATY = AT * Y;

    // Build augmented matrix [ATA | ATY]
    int sz = degree + 1;
    Matrix aug(sz, sz + 1);
    for (int i = 0; i < sz; i++)
    {
        for (int j = 0; j < sz; j++)
            aug.set(i, j, ATA.get(i, j));
        aug.set(i, sz, ATY.get(i, 0));
    }

    vector<double> coeffs = solveNormalEquation(aug);

    // ── Write results to output file ──
    fout << fixed << setprecision(6);
    fout << "\n========== Least Squares Parabola (Degree " << degree << ") ==========\n\n";
    fout << "Fitted Polynomial Coefficients:\n";
    fout << "  f(x) = ";
    for (int k = 0; k <= degree; k++)
    {
        if (k > 0) fout << " + ";
        fout << coeffs[k];
        if (k == 1) fout << "*x";
        if (k >= 2) fout << "*x^" << k;
    }
    fout << "\n\n";

    outputTable(fout, coeffs, degree);

    double rmse = computeRMSE(coeffs);
    fout << "\nRMSE = " << rmse << "\n";

    return coeffs;
}

// ─────────────────────────────────────────────
// predict()  –  Horner / direct evaluation
// ─────────────────────────────────────────────
double LeastSquareParabola::predict(double x,
                                    const vector<double> &coefficients) const
{
    double y = 0.0, power = 1.0;
    for (size_t k = 0; k < coefficients.size(); k++)
    {
        y     += coefficients[k] * power;
        power *= x;
    }
    return y;
}

// ─────────────────────────────────────────────
// outputTable()
// ─────────────────────────────────────────────
void LeastSquareParabola::outputTable(ofstream &fout,
                                      const vector<double> &coefficients,
                                      int degree) const
{
    fout << "Data Table:\n";
    fout << left
         << setw(12) << "x"
         << setw(12) << "y(data)"
         << setw(14) << "f(x)(fitted)"
         << setw(14) << "residual"
         << setw(16) << "(fi-y)^2\n";
    fout << string(68, '-') << "\n";

    double sumSq = 0.0;
    for (int i = 0; i < rows; i++)
    {
        double x   = data[i][0];
        double y   = data[i][1];
        double fi  = predict(x, coefficients);
        double res = fi - y;
        double sq  = res * res;
        sumSq += sq;

        fout << left
             << setw(12) << x
             << setw(12) << y
             << setw(14) << fi
             << setw(14) << res
             << setw(16) << sq << "\n";
    }
    fout << string(68, '-') << "\n";
    fout << "Sum of squared residuals = " << sumSq << "\n";
}

// ─────────────────────────────────────────────
// computeRMSE()
// ─────────────────────────────────────────────
double LeastSquareParabola::computeRMSE(
    const vector<double> &coefficients) const
{
    double sumSq = 0.0;
    for (int i = 0; i < rows; i++)
    {
        double fi  = predict(data[i][0], coefficients);
        double err = fi - data[i][1];
        sumSq += err * err;
    }
    return sqrt(sumSq / rows);
}

// ─────────────────────────────────────────────
// plotGraph()  –  writes .dat + .gnu then calls gnuplot
// ─────────────────────────────────────────────
void LeastSquareParabola::plotGraph(const vector<double> &coefficients,
                                    int degree,
                                    const string &pngFile) const
{
    // Derive companion file names from the PNG path
    string datFile = pngFile.substr(0, pngFile.rfind('.')) + "_data.dat";
    string gnuFile = pngFile.substr(0, pngFile.rfind('.')) + "_plot.gnu";

    // ── 1. Write data points ──
    ofstream dat(datFile);
    if (!dat)
        throw runtime_error("plotGraph: cannot create data file: " + datFile);
    dat << fixed << setprecision(6);
    dat << "# x   y_data\n";
    for (int i = 0; i < rows; i++)
        dat << data[i][0] << "   " << data[i][1] << "\n";
    dat.close();

    // ── 2. Build Gnuplot polynomial expression ──
    //   e.g.  f(x) = c0 + c1*x + c2*x**2
    string polyExpr = "f(x) = ";
    for (int k = 0; k <= degree; k++)
    {
        if (k > 0) polyExpr += " + ";
        polyExpr += to_string(coefficients[k]);
        if (k == 1) polyExpr += "*x";
        if (k >= 2) polyExpr += "*x**" + to_string(k);
    }

    // ── 3. Write Gnuplot script ──
    ofstream gnu(gnuFile);
    if (!gnu)
        throw runtime_error("plotGraph: cannot create gnuplot script: " + gnuFile);

    gnu << "set terminal pngcairo size 900,600 enhanced font 'Helvetica,13'\n";
    gnu << "set output '" << pngFile << "'\n";
    gnu << "set title 'Least Squares Polynomial Fit (degree " << degree << ")' font 'Helvetica,15'\n";
    gnu << "set xlabel 'x'\n";
    gnu << "set ylabel 'y'\n";
    gnu << "set grid lc rgb '#cccccc' lw 1\n";
    gnu << "set key top left\n";
    gnu << "set style data points\n";

    // Determine x range from data
    double xMin = data[0][0], xMax = data[0][0];
    for (int i = 1; i < rows; i++)
    {
        if (data[i][0] < xMin) xMin = data[i][0];
        if (data[i][0] > xMax) xMax = data[i][0];
    }
    double margin = (xMax - xMin) * 0.1;
    gnu << "set xrange [" << (xMin - margin) << ":" << (xMax + margin) << "]\n";

    // Define the fitted curve function
    gnu << polyExpr << "\n\n";

    // Plot: data points + fitted curve
    gnu << "plot '" << datFile << "' using 1:2 "
        << "with points pt 7 ps 1.5 lc rgb '#e74c3c' title 'Data Points', \\\n"
        << "     f(x) "
        << "with lines lw 2.5 lc rgb '#2980b9' title 'Fitted Curve (degree " << degree << ")'\n";
    gnu.close();

    // ── 4. Execute Gnuplot ──
    int ret = system(("gnuplot \"" + gnuFile + "\"").c_str());
    if (ret != 0)
        throw runtime_error("plotGraph: gnuplot execution failed. Is gnuplot installed?");
}

// ─────────────────────────────────────────────
// solveNormalEquation()  –  Gaussian elimination
//   with partial pivoting on augmented matrix
// ─────────────────────────────────────────────
vector<double> LeastSquareParabola::solveNormalEquation(Matrix aug) const
{
    int n = aug.getRows();

    // Forward elimination with partial pivoting
    for (int col = 0; col < n; col++)
    {
        // Find pivot row
        int pivot = col;
        for (int row = col + 1; row < n; row++)
            if (fabs(aug.get(row, col)) > fabs(aug.get(pivot, col)))
                pivot = row;

        // Swap rows
        if (pivot != col)
            for (int k = 0; k <= n; k++)
            {
                double tmp = aug.get(col, k);
                aug.set(col, k, aug.get(pivot, k));
                aug.set(pivot, k, tmp);
            }

        if (fabs(aug.get(col, col)) < 1e-12)
            throw runtime_error("solveNormalEquation: singular or near-singular system.");

        double diag = aug.get(col, col);
        for (int row = col + 1; row < n; row++)
        {
            double factor = aug.get(row, col) / diag;
            for (int k = col; k <= n; k++)
                aug.set(row, k, aug.get(row, k) - factor * aug.get(col, k));
        }
    }

    // Back substitution
    vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; i--)
    {
        x[i] = aug.get(i, n);
        for (int j = i + 1; j < n; j++)
            x[i] -= aug.get(i, j) * x[j];
        x[i] /= aug.get(i, i);
    }
    return x;
}