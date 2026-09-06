#ifndef NUMERICAL_DIFFERENTIATION_HPP
#define NUMERICAL_DIFFERENTIATION_HPP

#include "TestFunction.hpp"
#include <functional>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

// Stores a mathematical test function, its exact derivative, and metadata needed for tables and graphs.
struct TestFunctionSpec
{
    std::string name;
    std::string expression;
    std::string derivativeExpression;
    std::string graphStem;
    std::function<double(double)> function;
    std::function<double(double)> exactDerivative;
    std::function<long double(long double)> functionLong;
    std::function<long double(long double)> exactDerivativeLong;
};

// Stores the numerical evaluation data and absolute error for one specific step size h.
struct DifferentiationSample
{
    double h;
    double functionAtPlus;
    double functionAtMinus;
    double numericalDerivative;
    double absoluteError;
};

// Groups all step-size sample evaluations and exact derivative values for one mathematical function.
struct FunctionResult
{
    TestFunctionSpec spec;
    double evaluationPoint;
    double exactDerivativeAtPoint;
    std::vector<DifferentiationSample> samples;
};

// Groups the complete evaluation results of all test functions for one numerical differentiation method.
struct MethodResult
{
    std::string methodName;
    std::string outputFile;
    std::string graphPrefix;
    std::vector<FunctionResult> functionResults;
};

// Main engine that performs numerical differentiation methods and writes results.
class NumericalDifferentiation
{
public:
    // Initializes the step sizes h from 10^-1 to 10^-10 and registers the benchmark test functions.
    NumericalDifferentiation();

    // Uses f(x+h) to approximate f'(x) from the right side with O(h) truncation error.
    double forwardDifference(const std::function<double(double)> &function, double x, double h) const;

    // Uses f(x-h) to approximate f'(x) from the left side with O(h) truncation error.
    double backwardDifference(const std::function<double(double)> &function, double x, double h) const;

    // Uses values on both sides of x to achieve higher O(h^2) truncation error accuracy.
    double centralDifference(const std::function<double(double)> &function, double x, double h) const;

    // Runs the Forward Difference scheme across all benchmark functions.
    MethodResult processForwardDifference(double x = 1.0) const;

    // Runs the Backward Difference scheme across all benchmark functions.
    MethodResult processBackwardDifference(double x = 1.0) const;

    // Runs the Central Difference scheme across all benchmark functions.
    MethodResult processCentralDifference(double x = 1.0) const;

    // Saves formatted numerical result tables and absolute errors to an output file.
    void writeResultsToFile(const MethodResult &result) const;

protected:
    std::vector<double> hValues;
    std::vector<TestFunctionSpec> testFunctions;
    std::vector<std::shared_ptr<TestFunction>> polymorphicFunctions;

    // Creates the output directory when it does not already exist.
    bool ensureDirectory(const std::string &directory) const;

    // Calculates one sample containing the numerical derivative and required function values.
    DifferentiationSample buildSample(const TestFunctionSpec &spec,
                                      double x,
                                      double h,
                                      const std::function<double(const std::function<double(double)> &, double, double)> &calculator,
                                      bool includePlusValue,
                                      bool includeMinusValue) const;

    // Evaluates one test function across all configured h values.
    FunctionResult evaluateFunction(const TestFunctionSpec &spec,
                                    double x,
                                    const std::function<DifferentiationSample(const TestFunctionSpec &, double, double)> &sampleBuilder) const;

    // Combines the results of all four functions for the selected method.
    MethodResult buildMethodResult(const std::string &methodName,
                                   const std::string &outputFile,
                                   const std::string &graphPrefix,
                                   double x,
                                   const std::function<DifferentiationSample(const TestFunctionSpec &, double, double)> &sampleBuilder) const;

    // Writes one function's numerical results as a formatted table.
    void writeFunctionBlock(std::ofstream &out,
                            const FunctionResult &result,
                            const std::string &methodName) const;
};

#endif