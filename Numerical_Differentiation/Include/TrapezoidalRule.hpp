#ifndef TRAPEZOIDAL_RULE_HPP
#define TRAPEZOIDAL_RULE_HPP

#include "NumericalDifferentiation.hpp"

#include <functional>
#include <string>
#include <vector>

struct IntegrationFunctionSpec
{
    std::string name;
    std::string expression;
    double lowerBound;
    double upperBound;
    double exactIntegral;
    std::function<double(double)> function;
};

struct IntegrationSample
{
    int intervals;
    double h;
    double numericalIntegral;
    double exactIntegral;
    double absoluteError;
};

struct IntegrationFunctionResult
{
    IntegrationFunctionSpec spec;
    std::vector<IntegrationSample> samples;
};

struct TrapezoidalResult
{
    std::string methodName;
    std::string outputFile;
    std::string graphFile;
    std::vector<IntegrationFunctionResult> functionResults;
};

// Performs numerical integration while reusing the project's common numerical-method base.
class TrapezoidalRule : public NumericalDifferentiation
{
public:
    TrapezoidalRule();

    double integrate(const std::function<double(double)> &function,
                     double lowerBound,
                     double upperBound,
                     int intervals) const;

    TrapezoidalResult processTrapezoidalRule() const;
    void writeResultsToFile(const TrapezoidalResult &result) const;

private:
    std::vector<IntegrationFunctionSpec> integrationFunctions;
    std::vector<int> intervalCounts;

    void validateInterval(double lowerBound, double upperBound) const;
    void validateFunctionValue(double value) const;
};

#endif