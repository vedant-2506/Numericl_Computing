#include "../Include/Simpsons38Rule.hpp"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

Simpsons38Rule::Simpsons38Rule()
    : integrationFunctions{
          {"x^2", "x^2", 0.0, 1.0, 1.0 / 3.0, [](double x) { return x * x; }},
          {"exp(x^2)", "exp(x^2)", 0.0, 1.0, 1.4626517459071816,
           [](double x) { return exp(x * x); }}},
      intervalCounts{3, 6, 9, 12, 15, 30, 60, 90}
{
}

void Simpsons38Rule::validateInterval(double lowerBound, double upperBound) const
{
    if (!isfinite(lowerBound) || !isfinite(upperBound) || lowerBound >= upperBound)
        throw invalid_argument("Simpson's 3/8 Rule requires finite bounds with a < b.");
}

void Simpsons38Rule::validateFunctionValue(double value) const
{
    if (!isfinite(value))
        throw runtime_error("Simpson's 3/8 Rule encountered a non-finite function value.");
}

double Simpsons38Rule::integrate(const function<double(double)> &function,
                                 double lowerBound,
                                 double upperBound,
                                 int intervals) const
{
    validateInterval(lowerBound, upperBound);
    if (intervals <= 0 || intervals % 3 != 0)
        throw invalid_argument("Simpson's 3/8 Rule requires a positive interval count divisible by 3.");

    const double h = (upperBound - lowerBound) / static_cast<double>(intervals);
    if (!isfinite(h) || h <= 0.0)
        throw runtime_error("Simpson's 3/8 Rule produced an invalid step size.");

    long double weightedSum = 0.0L;
    for (int index = 0; index <= intervals; ++index)
    {
        const double x = lowerBound + h * static_cast<double>(index);
        const double value = function(x);
        validateFunctionValue(value);

        const int coefficient = index == 0 || index == intervals
                                    ? 1
                                    : (index % 3 == 0 ? 2 : 3);
        // Simpson's 3/8 rule gives weight 3 to non-multiples of 3 and 2 to multiples of 3.
        weightedSum += static_cast<long double>(coefficient) * value;
    }

    return static_cast<double>(3.0L * static_cast<long double>(h) * weightedSum / 8.0L);
}

Simpsons38Result Simpsons38Rule::processSimpsons38Rule() const
{
    Simpsons38Result result{"Simpson's 3/8 Rule", "Output/Simpsons38Rule.txt",
                            "Graphs/Simpsons38Rule.png",
                            "Graphs/Simpsons38Rule_Error_vs_N.png", {}};
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

void Simpsons38Rule::writeResultsToFile(const Simpsons38Result &result) const
{
    ensureDirectory("Output");
    ofstream out(result.outputFile);
    if (!out)
        throw runtime_error("Cannot open output file: " + result.outputFile);

    out << "========================================\n"
        << "        NUMERICAL INTEGRATION\n"
        << "========================================\n\n"
        << "Method: " << result.methodName << "\n\n";
    out << scientific << setprecision(12);
    for (size_t functionIndex = 0; functionIndex < result.functionResults.size(); ++functionIndex)
    {
        const IntegrationFunctionResult &functionResult = result.functionResults[functionIndex];
        const IntegrationFunctionSpec &spec = functionResult.spec;
        out << "Function: f(x) = " << spec.expression << '\n'
            << "Interval: [" << spec.lowerBound << ", " << spec.upperBound << "]\n"
            << "Exact/reference integral: " << spec.exactIntegral << "\n\n"
            << left << setw(8) << "Sr. No." << setw(15) << "n (Intervals)" << setw(22) << "h"
            << setw(26) << "Numerical Integral" << setw(22) << "Exact Integral"
            << "Absolute Error\n"
            << string(115, '-') << '\n';
        for (size_t sampleIndex = 0; sampleIndex < functionResult.samples.size(); ++sampleIndex)
        {
            const IntegrationSample &sample = functionResult.samples[sampleIndex];
            out << setw(8) << sampleIndex + 1 << setw(15) << sample.intervals
                << setw(22) << sample.h << setw(26) << sample.numericalIntegral
                << setw(22) << sample.exactIntegral << sample.absoluteError << '\n';
        }
        out << '\n';
        if (functionIndex + 1 < result.functionResults.size())
            out << '\n';
    }
}
