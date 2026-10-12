#include "../Include/TrapezoidalRule.hpp"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

TrapezoidalRule::TrapezoidalRule()
    : integrationFunctions{
          {"x^2", "x^2", 0.0, 1.0, 1.0 / 3.0, [](double x) { return x * x; }},
          {"exp(-x^2)", "exp(-x^2)", 0.0, 1.0,
           sqrt(acos(-1.0)) * erf(1.0) / 2.0,
           [](double x) { return exp(-(x * x)); }}}
    , intervalCounts{2, 4, 8, 16, 32, 64, 128, 256}
{
}

void TrapezoidalRule::validateInterval(double lowerBound, double upperBound) const
{
    if (!isfinite(lowerBound) || !isfinite(upperBound) || lowerBound >= upperBound)
        throw invalid_argument("Trapezoidal Rule requires finite bounds with a < b.");
}

void TrapezoidalRule::validateFunctionValue(double value) const
{
    if (!isfinite(value))
        throw runtime_error("Trapezoidal Rule encountered a non-finite function value.");
}

double TrapezoidalRule::integrate(const function<double(double)> &function,
                                  double lowerBound,
                                  double upperBound,
                                  int intervals) const
{
    validateInterval(lowerBound, upperBound);
    if (intervals <= 0)
        throw invalid_argument("Trapezoidal Rule requires a positive number of intervals.");

    const double h = (upperBound - lowerBound) / static_cast<double>(intervals);
    if (!isfinite(h) || h <= 0.0)
        throw runtime_error("Trapezoidal Rule produced an invalid step size.");

    const double firstValue = function(lowerBound);
    const double lastValue = function(upperBound);
    validateFunctionValue(firstValue);
    validateFunctionValue(lastValue);

    long double weightedSum = static_cast<long double>(firstValue) + lastValue;
    for (int index = 1; index < intervals; ++index)
    {
        const double x = lowerBound + h * static_cast<double>(index);
        const double value = function(x);
        validateFunctionValue(value);
        weightedSum += 2.0L * static_cast<long double>(value);
    }

    // Composite Trapezoidal Rule gives double weight to all interior points.
    return static_cast<double>(static_cast<long double>(h) * weightedSum / 2.0L);
}

TrapezoidalResult TrapezoidalRule::processTrapezoidalRule() const
{
    TrapezoidalResult result;
    result.methodName = "Trapezoidal Rule";
    result.outputFile = "Output/TrapezoidalRule.txt";
    result.graphFile = "Graphs/TrapezoidalRule.png";

    for (const IntegrationFunctionSpec &spec : integrationFunctions)
    {
        validateInterval(spec.lowerBound, spec.upperBound);
        IntegrationFunctionResult functionResult{spec, {}};
        for (const int intervals : intervalCounts)
        {
            const double h = (spec.upperBound - spec.lowerBound) / intervals;
            const double numericalIntegral = integrate(spec.function, spec.lowerBound,
                                                       spec.upperBound, intervals);
            functionResult.samples.push_back({intervals, h, numericalIntegral,
                                              spec.exactIntegral,
                                              abs(spec.exactIntegral - numericalIntegral)});
        }
        result.functionResults.push_back(functionResult);
    }

    return result;
}

void TrapezoidalRule::writeResultsToFile(const TrapezoidalResult &result) const
{
    ensureDirectory("Output");
    ofstream out(result.outputFile);
    if (!out)
        throw runtime_error("Cannot open output file: " + result.outputFile);

    out << "========================================\n"
        << "        NUMERICAL INTEGRATION\n"
        << "========================================\n\n"
        << "Method: " << result.methodName << "\n\n";
    out << fixed << setprecision(10);

    for (size_t functionIndex = 0; functionIndex < result.functionResults.size(); ++functionIndex)
    {
        const IntegrationFunctionResult &functionResult = result.functionResults[functionIndex];
        const IntegrationFunctionSpec &spec = functionResult.spec;
        out << "Function: f(x) = " << spec.expression << '\n'
            << "Interval: [" << spec.lowerBound << ", " << spec.upperBound << "]\n"
            << "Exact/reference integral: " << spec.exactIntegral << "\n\n";
        out << left << setw(8) << "Sr. No." << setw(12) << "n" << setw(18) << "h"
            << setw(28) << "Numerical Integral" << setw(20) << "Exact Integral"
            << "Absolute Error\n";
        out << string(106, '-') << '\n';
        for (size_t sampleIndex = 0; sampleIndex < functionResult.samples.size(); ++sampleIndex)
        {
            const IntegrationSample &sample = functionResult.samples[sampleIndex];
            out << setw(8) << sampleIndex + 1 << setw(12) << sample.intervals
                << setw(18) << sample.h << setw(28) << sample.numericalIntegral
                << setw(20) << sample.exactIntegral << sample.absoluteError << '\n';
        }
        out << '\n';
        if (functionIndex + 1 < result.functionResults.size())
            out << "\n";
    }
}