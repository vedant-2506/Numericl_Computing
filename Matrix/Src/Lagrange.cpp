#include "../Include/Lagrange.hpp"  // Lagrange class declaration.
#include <cmath>                     // fabs.
#include <iomanip>                   // fixed, setprecision.
#include <stdexcept>                 // runtime_error, out_of_range.
#include <string>                    // to_string.

using namespace std;

Lagrange::Lagrange() : Interpolation() {}

Lagrange::Lagrange(const Matrix &m) : Interpolation(m) {}

double Lagrange::basisPolynomial(int i, double x, ofstream &fout) const    // Compute L_i(x).
{
    int n = rows;
    if (i < 0 || i >= n)
        throw out_of_range("Lagrange::basisPolynomial: index i out of range.");

    double xi = data[i][0];

    fout << "  L_" << i << "(x) = ";
    double num = 1.0, den = 1.0;            // numerator / denominator product
    bool firstFactor = true;

    for (int j = 0; j < n; j++)
    {
        if (j == i)
            continue;
        double xj = data[j][0];

        num *= (x - xj);
        den *= (xi - xj);

        if (!firstFactor)
            fout << " * ";
        fout << "(x - " << xj << ")";
        firstFactor = false;
    }

    fout << "\n        / ";
    firstFactor = true;
    for (int j = 0; j < n; j++)
    {
        if (j == i)
            continue;
        double xj = data[j][0];
        if (!firstFactor)
            fout << " * ";
        fout << "(" << xi << " - " << xj << ")";
        firstFactor = false;
    }

    if (fabs(den) < 1e-15)  // duplicate x-values are not allowed
        throw runtime_error("Lagrange: duplicate x values detected in data points.");

    double Li = num / den;

    fout << "\n        = " << num << " / " << den
         << " = " << Li << "\n";

    return Li;
}

vector<double> Lagrange::allBasisValues(double x) const  // Compute all basis values at x.
{
    int n = rows;
    vector<double> L(n, 1.0);
    for (int i = 0; i < n; i++)
    {
        double xi = data[i][0];
        for (int j = 0; j < n; j++)
        {
            if (j == i)
                continue;
            double xj = data[j][0];
            double den = xi - xj;
            if (fabs(den) < 1e-15)
                throw runtime_error("Lagrange: duplicate x values at indices "
                                    + to_string(i) + " and " + to_string(j));
            L[i] *= (x - xj) / den;
        }
    }
    return L;
}

double Lagrange::interpolate(double xQuery, ofstream &fout)  // Evaluate P(xQuery) using Lagrange formula.
{
    validateDataPoints();
    int n = rows;

    fout << "========================================\n";
    fout << "       Lagrange Interpolation\n";
    fout << "========================================\n";
    fout << "Query point: x = " << xQuery << "\n\n";

    printDataPoints(fout);

    for (int i = 0; i < n; i++)
        if (fabs(data[i][0] - xQuery) < 1e-12)  // direct node hit: return exact y
        {
            fout << "x = " << xQuery
                 << " is a data node. Exact value = " << data[i][1] << "\n";
            return data[i][1];
        }

    fout << "Formula:  P(x) = sum of [ y_i * L_i(x) ]  for i = 0 to " << n - 1 << "\n\n";

    double result = 0.0;

    fout << fixed << setprecision(6);
    fout << "--- Computing basis polynomials at x = " << xQuery << " ---\n\n";

    for (int i = 0; i < n; i++)
    {
        double Li = basisPolynomial(i, xQuery, fout);
        double yi = data[i][1];
        double term = yi * Li;

        fout << "  y_" << i << " * L_" << i << "(" << xQuery << ")"
             << " = " << yi << " * " << Li
             << " = " << term << "\n\n";

        result += term;
    }

    double sumL = 0.0;
    vector<double> Lvals = allBasisValues(xQuery);
    for (size_t i = 0; i < Lvals.size(); i++)
        sumL += Lvals[i];

    fout << "--- Summary ---\n";
    fout << "Sum of all L_i (should be 1.0): " << sumL << "\n\n";  // sanity check for basis values

    fout << "Contribution of each term:\n\n";
    for (int i = 0; i < n; i++)
    {
        double yi = data[i][1];
        double Li = Lvals[i];
        double term = yi * Li;

        fout << "i = " << i
             << " : y_" << i << " = " << yi
             << ", L_" << i << "(x) = " << Li
             << ", y_" << i << " * L_" << i << "(x) = " << term << "\n";
    }
    fout << "\n";

    fout << "  P(" << xQuery << ") = " << result << "\n";  // final interpolated value

    return result;
}
