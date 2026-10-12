#include "../Include/PowerMethod.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <stdexcept>

using namespace std;
namespace fs = std::filesystem;

PowerMethod::PowerMethod() : NumericalDifferentiation() {}

PowerMethod::Matrix PowerMethod::readMatrix(const string &matrixFilename) const
{
    const fs::path inputPath = fs::path("Input") / matrixFilename;
    ifstream input(inputPath);
    if (!input)
        throw runtime_error("Cannot open matrix file: " + inputPath.string());

    Matrix matrix;
    string line;
    while (getline(input, line))
    {
        istringstream rowStream(line);
        vector<double> row;
        double value = 0.0;
        while (rowStream >> value)
        {
            if (!isfinite(value))
                throw invalid_argument("Matrix contains a non-finite value.");
            row.push_back(value);
        }

        if (!rowStream.eof())
            throw invalid_argument("Matrix contains a non-numeric value.");
        if (!row.empty())
            matrix.push_back(row);
    }

    validateMatrix(matrix);
    return matrix;
}

void PowerMethod::validateMatrix(const Matrix &matrix) const
{
    if (matrix.empty())
        throw invalid_argument("Matrix file is empty.");

    const size_t dimension = matrix.front().size();
    if (dimension == 0)
        throw invalid_argument("Matrix must contain at least one value.");
    if (matrix.size() != dimension)
        throw invalid_argument("Power Method requires a square matrix.");

    bool hasNonZeroValue = false;
    for (const vector<double> &row : matrix)
    {
        if (row.size() != dimension)
            throw invalid_argument("Every matrix row must have the same number of columns.");
        for (double value : row)
        {
            if (!isfinite(value))
                throw invalid_argument("Matrix contains a non-finite value.");
            hasNonZeroValue = hasNonZeroValue || value != 0.0;
        }
    }
    if (!hasNonZeroValue)
        throw invalid_argument("The zero matrix cannot produce a usable power iteration.");
}

vector<double> PowerMethod::multiply(const Matrix &matrix,
                                      const vector<double> &inputVector) const
{
    vector<double> product(matrix.size(), 0.0);
    for (size_t row = 0; row < matrix.size(); ++row)
    {
        long double value = 0.0L;
        for (size_t column = 0; column < matrix[row].size(); ++column)
            value += static_cast<long double>(matrix[row][column]) * inputVector[column];
        product[row] = static_cast<double>(value);
        if (!isfinite(product[row]))
            throw runtime_error("Matrix-vector multiplication produced a non-finite value.");
    }
    return product;
}

double PowerMethod::euclideanNorm(const vector<double> &vector) const
{
    long double sum = 0.0L;
    for (double value : vector)
        sum += static_cast<long double>(value) * value;
    const double norm = static_cast<double>(sqrt(sum));
    if (!isfinite(norm))
        throw runtime_error("Vector norm is not finite.");
    return norm;
}

string PowerMethod::outputStem(const string &matrixFilename) const
{
    const string stem = fs::path(matrixFilename).stem().string();
    if (stem.empty())
        throw invalid_argument("Matrix filename must have a valid .txt stem.");
    return stem;
}

PowerMethodResult PowerMethod::process(const string &matrixFilename,
                                       double tolerance,
                                       size_t maxIterations) const
{
    if (!isfinite(tolerance) || tolerance <= 0.0)
        throw invalid_argument("Power Method tolerance must be positive and finite.");
    if (maxIterations == 0)
        throw invalid_argument("Power Method maximum iterations must be greater than zero.");
    const fs::path filenamePath(matrixFilename);
    if (filenamePath.is_absolute() || filenamePath.has_parent_path())
        throw invalid_argument("Provide a matrix filename located directly in the Input directory.");
    if (filenamePath.extension() != ".txt")
        throw invalid_argument("Matrix input files must use the .txt extension.");

    const Matrix matrix = readMatrix(matrixFilename);
    const size_t dimension = matrix.size();
    vector<double> currentVector(dimension, 1.0);
    const double initialNorm = euclideanNorm(currentVector);
    for (double &value : currentVector)
        value /= initialNorm;

    PowerMethodResult result;
    result.inputFile = matrixFilename;
    result.methodName = "Power Method";
    result.outputFile = "Output/power_method_" + outputStem(matrixFilename) + ".txt";
    result.graphFile = "Graphs/power_method_" + outputStem(matrixFilename) + ".png";
    result.matrixDimension = dimension;
    result.tolerance = tolerance;
    result.maxIterations = maxIterations;
    result.initialVector.assign(dimension, 1.0);
    result.eigenvalue = numeric_limits<double>::quiet_NaN();
    result.eigenvector = currentVector;
    result.normalizedResidual = numeric_limits<double>::quiet_NaN();
    result.converged = false;

    double previousEigenvalue = 0.0;
    bool hasPreviousEigenvalue = false;
    const double minimumNorm = numeric_limits<double>::epsilon();

    for (size_t iteration = 1; iteration <= maxIterations; ++iteration)
    {
        const vector<double> product = multiply(matrix, currentVector);
        const double productNorm = euclideanNorm(product);
        if (productNorm <= minimumNorm)
        {
            result.status = "Stopped: matrix-vector product is numerically zero.";
            break;
        }

        currentVector = product;
        for (double &value : currentVector)
            value /= productNorm;

        const vector<double> ax = multiply(matrix, currentVector);
        long double numerator = 0.0L;
        long double denominator = 0.0L;
        for (size_t index = 0; index < dimension; ++index)
        {
            numerator += static_cast<long double>(currentVector[index]) * ax[index];
            denominator += static_cast<long double>(currentVector[index]) * currentVector[index];
        }
        if (denominator <= 0.0L)
        {
            result.status = "Stopped: eigenvalue quotient has a zero denominator.";
            break;
        }

        const double eigenvalue = static_cast<double>(numerator / denominator);
        std::vector<double> residual(dimension, 0.0);
        for (size_t index = 0; index < dimension; ++index)
            residual[index] = ax[index] - eigenvalue * currentVector[index];

        const double axNorm = euclideanNorm(ax);
        const double vectorNorm = euclideanNorm(currentVector);
        const double denominatorNorm = max(axNorm, abs(eigenvalue) * vectorNorm);
        // P_k = ||A X_k - lambda_k X_k||_2 /
        //       max(||A X_k||_2, |lambda_k| ||X_k||_2).
        if (denominatorNorm <= 0.0)
        {
            result.status = "Stopped: normalized residual has a zero denominator.";
            break;
        }
        const double normalizedResidual = euclideanNorm(residual) / denominatorNorm;

        PowerIteration sample;
        sample.iteration = iteration;
        sample.eigenvalue = eigenvalue;
        sample.hasEigenvalueDifference = hasPreviousEigenvalue;
        sample.eigenvalueDifference = hasPreviousEigenvalue
                                           ? abs(eigenvalue - previousEigenvalue)
                                           : 0.0;
        sample.normalizedResidual = normalizedResidual;
        result.iterations.push_back(sample);
        result.eigenvalue = eigenvalue;
        result.eigenvector = currentVector;
        result.normalizedResidual = normalizedResidual;

        if (hasPreviousEigenvalue && sample.eigenvalueDifference < tolerance)
        {
            result.converged = true;
            result.status = "Converged: eigenvalue difference is below tolerance.";
            break;
        }
        previousEigenvalue = eigenvalue;
        hasPreviousEigenvalue = true;
    }

    if (result.iterations.empty() && result.status.empty())
        result.status = "Stopped without producing an iteration.";
    else if (!result.converged && result.status.empty())
        result.status = "Did not converge within the maximum iteration count.";

    return result;
}

void PowerMethod::writeResultsToFile(const PowerMethodResult &result) const
{
    if (!ensureDirectory("Output"))
        throw runtime_error("Cannot create output directory.");

    ofstream output(result.outputFile);
    if (!output)
        throw runtime_error("Cannot create output file: " + result.outputFile);

    output << "Power Method Results\n"
           << "Input matrix file: " << result.inputFile << "\n"
           << "Matrix dimensions: " << result.matrixDimension << " x " << result.matrixDimension << "\n"
           << "Tolerance: " << scientific << setprecision(12) << result.tolerance << "\n"
           << "Initial vector convention: X0 = [1, 1, ..., 1]^T, normalized with the Euclidean norm\n"
           << "Stopping condition: |lambda_k - lambda_(k-1)| < tolerance\n"
           << "Status: " << result.status << "\n\n";

    output << left << setw(12) << "Iteration"
           << setw(28) << "Eigenvalue Estimate (lambda_k)"
           << setw(28) << "Absolute Difference"
           << "Normalized Residual (P_k)\n";
    output << string(90, '-') << '\n';
    for (const PowerIteration &sample : result.iterations)
    {
        output << left << setw(12) << sample.iteration
               << setw(28) << scientific << setprecision(12) << sample.eigenvalue;
        if (sample.hasEigenvalueDifference)
            output << setw(28) << sample.eigenvalueDifference;
        else
            output << setw(28) << "N/A";
        output << sample.normalizedResidual << '\n';
    }

    output << "\n";
    if (result.iterations.empty())
    {
        output << "Final estimated dominant eigenvalue: unavailable\n"
               << "Final estimated eigenvector: unavailable\n"
               << "Final normalized residual: unavailable\n";
    }
    else
    {
        output << "Final estimated dominant eigenvalue: " << result.eigenvalue
               << "\nFinal estimated eigenvector:\n";
        for (double value : result.eigenvector)
            output << "  " << value << '\n';
        output << "Final normalized residual: " << result.normalizedResidual << '\n';
    }
    output << "Iterations performed: " << result.iterations.size() << '\n';
}
