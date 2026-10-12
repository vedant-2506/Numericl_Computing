#ifndef SIMPSONS_13_RULE_HPP
#define SIMPSONS_13_RULE_HPP

#include "NumericalDifferentiation.hpp"
#include "TrapezoidalRule.hpp"

#include <functional>
#include <string>
#include <vector>

struct Simpsons13Result
{
    std::string methodName;
    std::string outputFile;
    std::string graphFile;
    std::string errorGraphFile;
    std::vector<IntegrationFunctionResult> functionResults;
};

class Simpsons13Rule : public NumericalDifferentiation
{
public:
    Simpsons13Rule();

    double integrate(const std::function<double(double)> &function,
                     double lowerBound,
                     double upperBound,
                     int intervals) const;

    Simpsons13Result processSimpsons13Rule() const;
    void writeResultsToFile(const Simpsons13Result &result) const;

private:
    std::vector<IntegrationFunctionSpec> integrationFunctions;
    std::vector<int> intervalCounts;

    void validateInterval(double lowerBound, double upperBound) const;
    void validateFunctionValue(double value) const;
};

#endif
