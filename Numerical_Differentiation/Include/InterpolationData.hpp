#ifndef INTERPOLATION_DATA_HPP
#define INTERPOLATION_DATA_HPP

#include <string>
#include <vector>

struct InterpolationQuery
{
    double x;
    double y;
};

struct InterpolationResult
{
    std::string methodName;
    std::string outputFile;
    std::string graphFile;
    std::vector<double> xValues;
    std::vector<double> yValues;
    std::vector<double> polynomialCoefficients;
    std::vector<std::vector<double>> dividedDifferenceTable;
    std::vector<double> basisValues;
    std::vector<std::vector<double>> basisValuesForQueries;
    std::vector<std::string> basisExpressions;
    std::vector<InterpolationQuery> queries;
    std::string polynomialExpression;
};

#endif