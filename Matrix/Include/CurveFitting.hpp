#ifndef CURVE_FITTING_HPP
#define CURVE_FITTING_HPP

#include "Matrix.hpp"
#include <fstream>
#include <vector>

class CurveFitting : public Matrix
{
public:
    CurveFitting();
    CurveFitting(const Matrix &m);

    void loadSamplePoints(std::ifstream &fin);
    void printSamplePoints(std::ofstream &fout) const;
    void validateSamplePoints() const;

    virtual std::vector<double> fit(int degree, std::ofstream &fout) = 0;
    virtual double predict(double x, const std::vector<double> &coefficients) const;

    virtual ~CurveFitting() {}
};

#endif
