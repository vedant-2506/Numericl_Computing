#ifndef LAGRANGE_INTERPOLATION_HPP
#define LAGRANGE_INTERPOLATION_HPP

#include "InterpolationData.hpp"
#include "NumericalDifferentiation.hpp"

// Inherits the common numerical-method interface used by the project.
class LagrangeInterpolation : public NumericalDifferentiation
{
public:
    LagrangeInterpolation(const std::vector<double> &xValues,
                          const std::vector<double> &yValues);

    double evaluate(double x) const;
    InterpolationResult buildResult(const std::vector<double> &queryValues) const;
    void writeResultsToFile(const InterpolationResult &result) const;

private:
    // Stores user-provided points used to construct the interpolation polynomial.
    std::vector<double> xValues;
    std::vector<double> yValues;
    std::vector<double> polynomialCoefficients;
    std::vector<std::vector<double>> basisCoefficients;

    void validateData() const;
    void buildPolynomial();
    std::vector<double> buildBasisPolynomial(size_t index) const;
    std::string formatPolynomial() const;
};

#endif