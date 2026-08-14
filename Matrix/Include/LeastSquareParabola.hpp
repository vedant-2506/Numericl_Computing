#ifndef LEAST_SQUARE_PARABOLA_HPP
#define LEAST_SQUARE_PARABOLA_HPP

#include "CurveFitting.hpp"
#include <vector>
#include <fstream>
#include <string>

// LeastSquareParabola: fits data to polynomial y = a0 + a1*x + a2*x^2 + ...
// Inherits CurveFitting (which inherits Matrix) for data storage and validation
class LeastSquareParabola : public CurveFitting
{
public:
    LeastSquareParabola();                          // Default constructor
    LeastSquareParabola(const Matrix &m);           // Construct from existing Matrix

    // Fit polynomial of given degree using normal equations (ATA * c = ATY)
    // Returns coefficient vector [a0, a1, ..., a_degree]
    std::vector<double> fit(int degree, std::ofstream &fout) override;

    // Predict y for given x using coefficient vector
    double predict(double x, const std::vector<double> &coefficients) const override;

    // Print formatted results table to file
    void outputTable(std::ofstream &fout,
                     const std::vector<double> &coefficients,
                     int degree) const;

    // Compute Root Mean Square Error
    double computeRMSE(const std::vector<double> &coefficients) const;

    // Generate PNG graph via Gnuplot showing data points + fitted curve
    void plotGraph(const std::vector<double> &coefficients,
                   int degree,
                   const std::string &pngFile) const;

private:
    // Solve linear system via Gaussian elimination with partial pivoting
    std::vector<double> solveNormalEquation(Matrix augmented) const;
};

#endif