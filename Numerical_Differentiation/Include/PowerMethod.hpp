#ifndef POWER_METHOD_HPP
#define POWER_METHOD_HPP

#include "NumericalDifferentiation.hpp"

#include <string>
#include <cstddef>
#include <vector>

struct PowerIteration
{
    size_t iteration;
    double eigenvalue;
    bool hasEigenvalueDifference;
    double eigenvalueDifference;
    double normalizedResidual;
};

struct PowerMethodResult
{
    std::string inputFile;
    std::string methodName;
    std::string outputFile;
    std::string graphFile;
    size_t matrixDimension;
    double tolerance;
    size_t maxIterations;
    std::vector<double> initialVector;
    std::vector<PowerIteration> iterations;
    double eigenvalue;
    std::vector<double> eigenvector;
    double normalizedResidual;
    bool converged;
    std::string status;
};

/**
 * @brief Approximates the dominant eigenpair of a square matrix.
 *
 * The initial vector is [1, 1, ..., 1]^T and is normalized with the
 * Euclidean norm. Each iteration uses the Rayleigh quotient for the
 * eigenvalue estimate and stops only when consecutive estimates differ by
 * less than the configured tolerance.
 */
class PowerMethod : public NumericalDifferentiation
{
public:
    PowerMethod();

    /**
     * @brief Reads a matrix from Input/, runs the power iteration, and returns its results.
     * @param matrixFilename Filename relative to the Input directory.
     * @param tolerance Absolute eigenvalue-difference tolerance.
     * @param maxIterations Safety limit for iterations.
     */
    PowerMethodResult process(const std::string &matrixFilename,
                              double tolerance = 1e-5,
                              size_t maxIterations = 10000) const;

    void writeResultsToFile(const PowerMethodResult &result) const;

private:
    using Matrix = std::vector<std::vector<double>>;

    Matrix readMatrix(const std::string &matrixFilename) const;
    void validateMatrix(const Matrix &matrix) const;
    std::vector<double> multiply(const Matrix &matrix,
                                 const std::vector<double> &vector) const;
    double euclideanNorm(const std::vector<double> &vector) const;
    std::string outputStem(const std::string &matrixFilename) const;
};

#endif
