#ifndef LAGRANGE_HPP
#define LAGRANGE_HPP

#include <vector>
#include <string>
#include <fstream>

class Lagrange
{
private:
    std::vector<double> xData;      // Time values (x-coordinates)
    std::vector<double> yData;      // Temperature values (y-coordinates)
    int n;                          // Number of data points

public:
    // Constructors
    Lagrange();
    Lagrange(const std::vector<double>& x, const std::vector<double>& y);

    // Destructor
    ~Lagrange();

    // Methods
    void setData(const std::vector<double>& x, const std::vector<double>& y);
    
    // Calculate basis polynomial L_i(x)
    double basisPolynomial(int i, double x, std::ofstream& fout) const;
    
    // Get all basis values at query point
    std::vector<double> allBasisValues(double x) const;
    
    // Main interpolation method
    double interpolate(double xQuery, std::ofstream& fout);
    
    // Utility methods
    int getDataSize() const { return n; }
    double getXValue(int i) const { return xData[i]; }
    double getYValue(int i) const { return yData[i]; }
};

#endif
