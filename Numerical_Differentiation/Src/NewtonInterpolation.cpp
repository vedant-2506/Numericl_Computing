#include "../Include/NewtonInterpolation.hpp"

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

string dividedDifferenceLabel(size_t order)
{
    if (order % 100 >= 11 && order % 100 <= 13)
        return to_string(order) + "th Divided Diff";
    if (order % 10 == 1)
        return to_string(order) + "st Divided Diff";
    if (order % 10 == 2)
        return to_string(order) + "nd Divided Diff";
    if (order % 10 == 3)
        return to_string(order) + "rd Divided Diff";
    return to_string(order) + "th Divided Diff";
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
}

NewtonInterpolation::NewtonInterpolation(const vector<double> &xValues,
                                         const vector<double> &yValues)
    : xValues(xValues), yValues(yValues)
{
    validateData();
    buildDividedDifferenceTable();
    buildPolynomial();
}

void NewtonInterpolation::validateData() const
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

void NewtonInterpolation::buildDividedDifferenceTable()
{
    const size_t pointCount = xValues.size();
    dividedDifferenceTable.assign(pointCount, vector<double>(pointCount, 0.0));
    for (size_t row = 0; row < pointCount; ++row)
        dividedDifferenceTable[row][0] = yValues[row];

    for (size_t order = 1; order < pointCount; ++order)
    {
        for (size_t row = 0; row + order < pointCount; ++row)
        {
            const double denominator = xValues[row + order] - xValues[row];
            if (denominator == 0.0)
                throw invalid_argument("Duplicate x values cause a zero divided-difference denominator.");
            dividedDifferenceTable[row][order] =
                (dividedDifferenceTable[row + 1][order - 1] - dividedDifferenceTable[row][order - 1]) /
                denominator;
        }
    }
}

void NewtonInterpolation::buildPolynomial()
{
    const size_t pointCount = xValues.size();
    newtonCoefficients.resize(pointCount);
    for (size_t index = 0; index < pointCount; ++index)
        newtonCoefficients[index] = dividedDifferenceTable[0][index];

    polynomialCoefficients.assign(pointCount, 0.0);
    vector<double> term(1, 1.0);
    for (size_t order = 0; order < pointCount; ++order)
    {
        for (size_t coefficient = 0; coefficient < term.size(); ++coefficient)
            polynomialCoefficients[coefficient] += newtonCoefficients[order] * term[coefficient];
        if (order + 1 < pointCount)
            term = multiplyByLinear(term, xValues[order]);
    }
}

double NewtonInterpolation::evaluate(double x) const
{
    double result = newtonCoefficients.back();
    for (size_t index = newtonCoefficients.size() - 1; index-- > 0;)
        result = result * (x - xValues[index]) + newtonCoefficients[index];
    return result;
}

string NewtonInterpolation::formatPolynomial() const
{
    ostringstream expression;
    expression << fixed << setprecision(10) << "P(x) = " << newtonCoefficients[0];
    for (size_t index = 1; index < newtonCoefficients.size(); ++index)
    {
        expression << (newtonCoefficients[index] >= 0.0 ? " + " : " - ")
                   << abs(newtonCoefficients[index]);
        for (size_t factor = 0; factor < index; ++factor)
            expression << "(x - " << formatValue(xValues[factor]) << ")";
    }
    return expression.str();
}

InterpolationResult NewtonInterpolation::buildResult(const vector<double> &queryValues) const
{
    InterpolationResult result;
    result.methodName = "Newton Interpolation";
    result.outputFile = "Output/NewtonInterpolation.txt";
    result.graphFile = "Graphs/NewtonInterpolation.png";
    result.xValues = xValues;
    result.yValues = yValues;
    result.polynomialCoefficients = polynomialCoefficients;
    result.dividedDifferenceTable = dividedDifferenceTable;
    result.polynomialExpression = formatPolynomial();

    for (double query : queryValues)
        result.queries.push_back({query, evaluate(query)});
    return result;
}

void NewtonInterpolation::writeResultsToFile(const InterpolationResult &result) const
{
    if (!ensureDirectory("Output"))
        throw runtime_error("Cannot create output directory.");

    ofstream out(result.outputFile);
    if (!out)
        throw runtime_error("Cannot open output file: " + result.outputFile);

    out << "========================================\n"
        << "      NEWTON INTERPOLATION\n"
        << "========================================\n\n"
        << "Input Data Points and Divided-Difference Table\n"
        << "-----------------------------------------------\n";
    out << left << setw(16) << "x" << setw(18) << "f(x)";
    for (size_t order = 1; order < xValues.size(); ++order)
        out << setw(20) << dividedDifferenceLabel(order);
    out << '\n' << string(16 + 18 + 20 * (xValues.size() - 1), '-') << '\n';

    out << fixed << setprecision(10);
    for (size_t row = 0; row < xValues.size(); ++row)
    {
        out << setw(16) << xValues[row] << setw(18) << yValues[row];
        for (size_t order = 1; order < xValues.size(); ++order)
        {
            if (row + order < xValues.size())
                out << setw(20) << dividedDifferenceTable[row][order];
            else
                out << setw(20) << "";
        }
        out << '\n';
    }

    out << "\nNewton Interpolation Polynomial\n-------------------------------\n"
        << result.polynomialExpression << "\n";
    for (const InterpolationQuery &query : result.queries)
        out << "\nRequested x = " << query.x << "\nInterpolated f(x) = " << query.y << '\n';
}