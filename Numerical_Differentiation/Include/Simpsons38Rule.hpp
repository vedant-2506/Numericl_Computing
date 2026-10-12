#ifndef SIMPSONS_38_RULE_HPP
#define SIMPSONS_38_RULE_HPP

#include "NumericalDifferentiation.hpp"
#include "TrapezoidalRule.hpp"

#include <functional>
#include <string>
#include <vector>

struct Simpsons38Result
{
    std::string methodName;
    std::string outputFile;
    std::string graphFile;
    std::string errorGraphFile;
    std::vector<IntegrationFunctionResult> functionResults;
};

class Simpsons38Rule : public NumericalDifferentiation
{
public:
    Simpsons38Rule();

    double integrate(const std::function<double(double)> &function,
                     double lowerBound,
                     double upperBound,
                     int intervals) const;

    Simpsons38Result processSimpsons38Rule() const;
    void writeResultsToFile(const Simpsons38Result &result) const;

private:
    std::vector<IntegrationFunctionSpec> integrationFunctions;
    std::vector<int> intervalCounts;

    void validateInterval(double lowerBound, double upperBound) const;
    void validateFunctionValue(double value) const;
};

#endif
