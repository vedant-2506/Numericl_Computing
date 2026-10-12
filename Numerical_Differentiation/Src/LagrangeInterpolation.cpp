#include "../Include/LagrangeInterpolation.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace
{
string formatValue(double value)
{
    ostringstream stream;
    stream << fixed << setprecision(10) << value;
    return stream.str();
}

vector<double> multiplyByLinear(const vector<double> &polynomial, double root)
{
    vector<double> result(polynomial.size() + 1, 0.0);
    for (size_t index = 0; index < polynomial.size(); ++index)
    {
        result[index] -= root * polynomial[index];
        result[index + 1] += polynomial[index];
    }
    return result;
}

string formatBasisExpression(const vector<double> &xValues, size_t basisIndex)
{
    ostringstream expression;
    expression << "(";
    bool firstFactor = true;
    for (size_t index = 0; index < xValues.size(); ++index)
    {
        if (index == basisIndex)
            continue;

        if (!firstFactor)
            expression << " * ";
        expression << "(x - " << formatValue(xValues[index]) << ") / ("
                   << formatValue(xValues[basisIndex]) << " - "
                   << formatValue(xValues[index]) << ")";
        firstFactor = false;
    }
    expression << ")";
    return expression.str();
}
}

LagrangeInterpolation::LagrangeInterpolation(const vector<double> &xValues,
                                             const vector<double> &yValues)
    : xValues(xValues), yValues(yValues)
{
    validateData();
    buildPolynomial();
}

void LagrangeInterpolation::validateData() const
{
    if (xValues.size() < 2 || xValues.size() != yValues.size())
        throw invalid_argument("Interpolation requires at least two matching data points.");

    for (size_t first = 0; first < xValues.size(); ++first)
    {
        if (!isfinite(xValues[first]) || !isfinite(yValues[first]))
            throw invalid_argument("Interpolation data must contain finite numbers.");

        for (size_t second = 0; second < first; ++second)
        {
            const double scale = max({1.0, abs(xValues[first]), abs(xValues[second])});
            if (abs(xValues[first] - xValues[second]) <= numeric_limits<double>::epsilon() * scale)
                throw invalid_argument("Duplicate x values are not valid for interpolation.");
        }
    }
}

// Calculates each Lagrange basis polynomial for polynomial construction.
vector<double> LagrangeInterpolation::buildBasisPolynomial(size_t index) const
{
    vector<double> basis(1, 1.0);
    for (size_t other = 0; other < xValues.size(); ++other)
    {
        if (other == index)
            continue;

        const double denominator = xValues[index] - xValues[other];
        basis = multiplyByLinear(basis, xValues[other]);
        for (double &coefficient : basis)
            coefficient /= denominator;
    }
    return basis;
}

void LagrangeInterpolation::buildPolynomial()
{
    polynomialCoefficients.assign(xValues.size(), 0.0);
    basisCoefficients.clear();

    for (size_t index = 0; index < xValues.size(); ++index)
    {
        vector<double> basis = buildBasisPolynomial(index);
        basisCoefficients.push_back(basis);
        for (size_t coefficient = 0; coefficient < basis.size(); ++coefficient)
            polynomialCoefficients[coefficient] += yValues[index] * basis[coefficient];
    }
}

double LagrangeInterpolation::evaluate(double x) const
{
    double result = 0.0;
    for (auto coefficient = polynomialCoefficients.rbegin();
         coefficient != polynomialCoefficients.rend(); ++coefficient)
    {
        result = result * x + *coefficient;
    }
    return result;
}

string LagrangeInterpolation::formatPolynomial() const
{
    ostringstream expression;
    expression << fixed << setprecision(10) << "P(x) = ";
    bool hasTerm = false;
    for (size_t index = polynomialCoefficients.size(); index-- > 0;)
    {
        const double coefficient = polynomialCoefficients[index];
        if (abs(coefficient) < 1e-12)
            continue;

        if (hasTerm)
            expression << (coefficient >= 0.0 ? " + " : " - ");
        else if (coefficient < 0.0)
            expression << "-";

        expression << abs(coefficient);
        if (index >= 1)
            expression << "x";
        if (index >= 2)
            expression << "^" << index;
        hasTerm = true;
    }
    if (!hasTerm)
        expression << "0";
    return expression.str();
}

InterpolationResult LagrangeInterpolation::buildResult(const vector<double> &queryValues) const
{
    InterpolationResult result;
    result.methodName = "Lagrange Interpolation";
    result.outputFile = "Output/LagrangeInterpolation.txt";
    result.graphFile = "Graphs/LagrangeInterpolation.png";
    result.xValues = xValues;
    result.yValues = yValues;
    // Stores polynomial data so the interpolation curve can be evaluated and plotted.
    result.polynomialCoefficients = polynomialCoefficients;
    result.polynomialExpression = formatPolynomial();

    for (size_t index = 0; index < xValues.size(); ++index)
        result.basisExpressions.push_back("L" + to_string(index) + "(x) = " +
                                          formatBasisExpression(xValues, index));

    for (double query : queryValues)
    {
        InterpolationQuery interpolationQuery{query, evaluate(query)};
        result.queries.push_back(interpolationQuery);
        vector<double> values;
        for (const vector<double> &basis : basisCoefficients)
        {
            double value = 0.0;
            for (auto coefficient = basis.rbegin(); coefficient != basis.rend(); ++coefficient)
                value = value * query + *coefficient;
            values.push_back(value);
        }
        result.basisValuesForQueries.push_back(values);
    }
    if (!result.basisValuesForQueries.empty())
        result.basisValues = result.basisValuesForQueries.front();
    return result;
}

void LagrangeInterpolation::writeResultsToFile(const InterpolationResult &result) const
{
    if (!ensureDirectory("Output"))
        throw runtime_error("Cannot create output directory.");

    ofstream out(result.outputFile);
    if (!out)
        throw runtime_error("Cannot open output file: " + result.outputFile);

    out << "========================================\n"
        << "     LAGRANGE INTERPOLATION\n"
        << "========================================\n\n"
        << "Input Data Points\n-----------------\n"
        << left << setw(18) << "x" << "f(x)\n" << string(32, '-') << '\n';
    out << fixed << setprecision(10);
    for (size_t index = 0; index < xValues.size(); ++index)
        out << setw(18) << xValues[index] << yValues[index] << '\n';

    out << "\nLagrange Basis Polynomials\n--------------------------\n";
    for (const string &basis : result.basisExpressions)
        out << basis << '\n';

    if (!result.queries.empty())
    {
        out << "\nLagrange Basis Values\n---------------------\n";
        for (size_t queryIndex = 0; queryIndex < result.queries.size(); ++queryIndex)
        {
            out << "At x = " << result.queries[queryIndex].x << '\n';
            for (size_t index = 0; index < result.basisValuesForQueries[queryIndex].size(); ++index)
                out << "L" << index << "(x) = "
                    << result.basisValuesForQueries[queryIndex][index] << '\n';
        }
    }

    out << "\n" << result.polynomialExpression << "\n";
    if (!result.queries.empty())
    {
        out << "\nRequested Points\n----------------\n"
            << left << setw(18) << "x" << "f(x)\n" << string(32, '-') << '\n';
        for (const InterpolationQuery &query : result.queries)
            out << setw(18) << query.x << query.y << '\n';
    }
}