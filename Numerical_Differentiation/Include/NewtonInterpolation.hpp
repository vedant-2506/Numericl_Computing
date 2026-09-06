#ifndef NEWTON_INTERPOLATION_HPP
#define NEWTON_INTERPOLATION_HPP

#include "InterpolationData.hpp"
#include "NumericalDifferentiation.hpp"

// Inherits the common numerical-method base while constructing Newton divided differences.
class NewtonInterpolation : public NumericalDifferentiation
{
public:
    NewtonInterpolation(const std::vector<double> &xValues,
                        const std::vector<double> &yValues);

    double evaluate(double x) const;
    InterpolationResult buildResult(const std::vector<double> &queryValues) const;
    void writeResultsToFile(const InterpolationResult &result) const;

private:
    std::vector<double> xValues;
    std::vector<double> yValues;
    std::vector<std::vector<double>> dividedDifferenceTable;
    std::vector<double> polynomialCoefficients;
    std::vector<double> newtonCoefficients;

    void validateData() const;
    void buildDividedDifferenceTable();
    void buildPolynomial();
    std::string formatPolynomial() const;
};

#endif