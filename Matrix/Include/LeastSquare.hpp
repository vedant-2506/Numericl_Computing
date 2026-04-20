#ifndef LEAST_SQUARE_HPP
#define LEAST_SQUARE_HPP

#include "CurveFitting.hpp"
#include <vector>

class LeastSquare : public CurveFitting
{
public:
    LeastSquare();
    LeastSquare(const Matrix &m);

    std::vector<double> fit(int degree, std::ofstream &fout);
    double predict(double x, const std::vector<double> &coefficients, std::ofstream &fout);
    double residualSumOfSquares(const std::vector<double> &coefficients) const;

private:
    std::vector<double> solveNormalEquation(Matrix augmented) const;
};

#endif
