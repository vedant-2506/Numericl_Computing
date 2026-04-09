#ifndef LAGRANGE_HPP            // Guard against double inclusion
#define LAGRANGE_HPP

#include "Interpolation.hpp"         // Base interpolation class
#include <vector>                    // For basis polynomial values

class Lagrange : public Interpolation  // Lagrange polynomial interpolation method
{
public:
    Lagrange();                      // Default constructor
    Lagrange(const Matrix &m);       // Constructor with interpolation points

    double basisPolynomial(int i, double x, std::ofstream &fout) const;  // Calculate L_i(x) basis polynomial

    std::vector<double> allBasisValues(double x) const;  // Get all basis values at x
    
    double interpolate(double xQuery, std::ofstream &fout);  // Evaluate polynomial at query point
};

#endif
