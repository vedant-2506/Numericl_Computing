#include "../Include/Lagrange.hpp"
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <string>

using namespace std;

// Constructor
Lagrange::Lagrange() : n(0) {}

// Constructor with data
Lagrange::Lagrange(const vector<double>& x, const vector<double>& y) 
{
    setData(x, y);
}

// Destructor
Lagrange::~Lagrange() {}

// Set interpolation data
void Lagrange::setData(const vector<double>& x, const vector<double>& y)
{
    if (x.size() != y.size() || x.empty())
        throw runtime_error("Lagrange: X and Y data must have same non-zero size.");
    
    // Check for duplicate x values
    for (size_t i = 0; i < x.size(); i++)
    {
        for (size_t j = i + 1; j < x.size(); j++)
        {
            if (fabs(x[i] - x[j]) < 1e-15)
                throw runtime_error("Lagrange: Duplicate x values detected at indices " 
                                  + to_string(i) + " and " + to_string(j));
        }
    }
    
    xData = x;
    yData = y;
    n = x.size();
}

// Calculate basis polynomial L_i(x)
double Lagrange::basisPolynomial(int i, double x, ofstream& fout) const
{
    if (i < 0 || i >= n)
        throw out_of_range("Lagrange::basisPolynomial: index i out of range.");
    
    double xi = xData[i];
    
    fout << "  L_" << i << "(x) = ";
    double num = 1.0, den = 1.0;
    bool firstFactor = true;
    
    // Calculate numerator
    for (int j = 0; j < n; j++)
    {
        if (j == i)
            continue;
        double xj = xData[j];
        
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
        double xj = xData[j];
        if (!firstFactor)
            fout << " * ";
        fout << "(" << xi << " - " << xj << ")";
        firstFactor = false;
    }
    
    if (fabs(den) < 1e-15)
        throw runtime_error("Lagrange: Invalid denominator in basis polynomial.");
    
    double Li = num / den;
    
    fout << "\n        = " << num << " / " << den
         << " = " << Li << "\n\n";
    
    return Li;
}

// Get all basis values at query point
vector<double> Lagrange::allBasisValues(double x) const
{
    vector<double> L(n, 1.0);
    for (int i = 0; i < n; i++)
    {
        double xi = xData[i];
        for (int j = 0; j < n; j++)
        {
            if (j == i)
                continue;
            double xj = xData[j];
            double den = xi - xj;
            if (fabs(den) < 1e-15)
                throw runtime_error("Lagrange: duplicate x values at indices " 
                                  + to_string(i) + " and " + to_string(j));
            L[i] *= (x - xj) / den;
        }
    }
    return L;
}

// Main interpolation function
double Lagrange::interpolate(double xQuery, ofstream& fout)
{
    if (n == 0)
        throw runtime_error("Lagrange: No data points set.");
    
    fout << "========================================\n";
    fout << "    LAGRANGE INTERPOLATION ANALYSIS\n";
    fout << "========================================\n";
    fout << "Query point: x = " << fixed << setprecision(6) << xQuery << "\n\n";
    
    // Check if query point is exact data node
    for (int i = 0; i < n; i++)
    {
        if (fabs(xData[i] - xQuery) < 1e-12)
        {
            fout << "x = " << xQuery 
                 << " is an exact data point. Exact value = " << yData[i] << "\n";
            return yData[i];
        }
    }
    
    // Print data table
    fout << "DATA TABLE:\n";
    fout << "Index | Time (Hour) | Temperature (°C)\n";
    fout << "------|-------------|------------------\n";
    for (int i = 0; i < n; i++)
    {
        fout << setw(5) << i << " | " 
             << setw(11) << setprecision(2) << xData[i] << " | "
             << setw(16) << setprecision(1) << yData[i] << "\n";
    }
    fout << "\n";
    
    fout << "LAGRANGE FORMULA: P(x) = Σ y_i * L_i(x) for i = 0 to " << n - 1 << "\n\n";
    
    double result = 0.0;
    
    fout << "--- Computing Basis Polynomials at x = " << xQuery << " ---\n\n";
    
    for (int i = 0; i < n; i++)
    {
        double Li = basisPolynomial(i, xQuery, fout);
        double yi = yData[i];
        double term = yi * Li;
        
        fout << "  Contribution: y_" << i << " * L_" << i << "(" << xQuery << ")"
             << " = " << setprecision(6) << yi << " * " << Li
             << " = " << term << "\n\n";
        
        result += term;
    }
    
    // Verify basis sum property
    vector<double> Lvals = allBasisValues(xQuery);
    double sumL = 0.0;
    for (size_t i = 0; i < Lvals.size(); i++)
        sumL += Lvals[i];
    
    fout << "--- VERIFICATION ---\n";
    fout << "Sum of all L_i (must equal 1.0): " << sumL << "\n\n";
    
    fout << "--- BASIS POLYNOMIAL SUMMARY ---\n";
    fout << "Index | L_i(x) Value\n";
    fout << "------|-------------------\n";
    for (int i = 0; i < n; i++)
    {
        fout << setw(5) << i << " | " 
             << setw(17) << setprecision(10) << Lvals[i] << "\n";
    }
    fout << "\n";
    
    fout << "========================================\n";
    fout << "POLYNOMIAL EQUATIONS (First 3 Basis Polynomials):\n";
    fout << "========================================\n\n";
    
    // Show L_0 equation
    fout << "L_0(x) = ";
    for (int j = 1; j < min(5, n); j++)
    {
        if (j > 1) fout << " * ";
        fout << "(x - " << setprecision(2) << xData[j] << ")";
    }
    if (n > 5) fout << " * ... (and " << (n-5) << " more terms)";
    fout << "\n        / ";
    for (int j = 1; j < min(5, n); j++)
    {
        if (j > 1) fout << " * ";
        fout << "(" << setprecision(2) << xData[0] << " - " << xData[j] << ")";
    }
    if (n > 5) fout << " * ... (and " << (n-5) << " more terms)";
    fout << "\n\n";
    
    // Show L_1 equation
    if (n > 1)
    {
        fout << "L_1(x) = ";
        bool first = true;
        for (int j = 0; j < min(5, n); j++)
        {
            if (j == 1) continue;
            if (!first) fout << " * ";
            fout << "(x - " << setprecision(2) << xData[j] << ")";
            first = false;
        }
        if (n > 5) fout << " * ... (and " << (n-5) << " more terms)";
        fout << "\n        / ";
        first = true;
        for (int j = 0; j < min(5, n); j++)
        {
            if (j == 1) continue;
            if (!first) fout << " * ";
            fout << "(" << setprecision(2) << xData[1] << " - " << xData[j] << ")";
            first = false;
        }
        if (n > 5) fout << " * ... (and " << (n-5) << " more terms)";
        fout << "\n\n";
    }
    
    // Show L_2 equation
    if (n > 2)
    {
        fout << "L_2(x) = ";
        bool first = true;
        for (int j = 0; j < min(5, n); j++)
        {
            if (j == 2) continue;
            if (!first) fout << " * ";
            fout << "(x - " << setprecision(2) << xData[j] << ")";
            first = false;
        }
        if (n > 5) fout << " * ... (and " << (n-5) << " more terms)";
        fout << "\n        / ";
        first = true;
        for (int j = 0; j < min(5, n); j++)
        {
            if (j == 2) continue;
            if (!first) fout << " * ";
            fout << "(" << setprecision(2) << xData[2] << " - " << xData[j] << ")";
            first = false;
        }
        if (n > 5) fout << " * ... (and " << (n-5) << " more terms)";
        fout << "\n\n";
    }
    
    fout << "========================================\n";
    fout << "FINAL INTERPOLATION POLYNOMIAL:\n";
    fout << "========================================\n";
    fout << "P(x) = ";
    for (int i = 0; i < min(5, n); i++)
    {
        if (i > 0) fout << " + ";
        fout << setprecision(1) << yData[i] << "*L_" << i << "(x)";
    }
    if (n > 5) fout << " + ... + " << setprecision(1) << yData[n-1] << "*L_" << (n-1) << "(x)";
    fout << "\n\n";
    
    fout << "========================================\n";
    fout << "FINAL RESULT:\n";
    fout << "========================================\n";
    fout << "P(" << xQuery << ") = " << setprecision(6) << result << " °C\n";
    fout << "Estimated Temperature at Time " << xQuery 
         << " hours: " << result << " °C\n";
    fout << "========================================\n";
    
    return result;
}
