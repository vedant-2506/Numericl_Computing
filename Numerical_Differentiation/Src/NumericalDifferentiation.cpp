#include "../Include/NumericalDifferentiation.hpp"

#include <cmath>
#include <cerrno>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <sys/stat.h>

using namespace std;

// Initializes step sizes h = 10^-1 to 10^-10 and registers benchmark test functions.
NumericalDifferentiation::NumericalDifferentiation()
{
    for (int power = 1; power <= 10; ++power)
        hValues.push_back(pow(10.0, -power));

    // Registers concrete TestFunction instances via polymorphism.
    polymorphicFunctions.push_back(make_shared<ExponentialFunction>());
    polymorphicFunctions.push_back(make_shared<SineFunction>());
    polymorphicFunctions.push_back(make_shared<CosineFunction>());
    polymorphicFunctions.push_back(make_shared<PolynomialFunction>());

    for (const auto &fn : polymorphicFunctions)
    {
        testFunctions.push_back({
            fn->getName(),
            fn->getExpression(),
            fn->getDerivativeExpression(),
            fn->getGraphStem(),
            [fn](double x) { return fn->evaluate(x); },
            [fn](double x) { return fn->exactDerivative(x); }
        });
    }
}

// Approximates f'(x) using the forward difference formula.
double NumericalDifferentiation::forwardDifference(const function<double(double)> &function,
                                                   double x,
                                                   double h) const
{
    // Validates h because division by zero or a non-positive step is invalid.
    if (h <= 0.0)
        throw runtime_error("forwardDifference: h must be positive.");

    return (function(x + h) - function(x)) / h;
}

// Approximates f'(x) using the backward difference formula.
double NumericalDifferentiation::backwardDifference(const function<double(double)> &function,
                                                    double x,
                                                    double h) const
{
    // Validates h because division by zero or a non-positive step is invalid.
    if (h <= 0.0)
        throw runtime_error("backwardDifference: h must be positive.");

    return (function(x) - function(x - h)) / h;
}

// Approximates f'(x) using the central difference formula.
double NumericalDifferentiation::centralDifference(const function<double(double)> &function,
                                                   double x,
                                                   double h) const
{
    // Validates h because division by zero or a non-positive step is invalid.
    if (h <= 0.0)
        throw runtime_error("centralDifference: h must be positive.");

    return (function(x + h) - function(x - h)) / (2.0 * h);
}

// Creates the output directory when it does not already exist.
bool NumericalDifferentiation::ensureDirectory(const string &directory) const
{
    if (mkdir(directory.c_str(), 0777) == 0)
        return true;

    if (errno == EEXIST)
        return true;

    throw runtime_error("Cannot create directory: " + directory);
}

// Calculates one sample containing the numerical derivative and required function values.
DifferentiationSample NumericalDifferentiation::buildSample(
    const TestFunctionSpec &spec,
    double x,
    double h,
    const function<double(const function<double(double)> &, double, double)> &calculator,
    bool includePlusValue,
    bool includeMinusValue) const
{
    DifferentiationSample sample{};
    sample.h = h;
    sample.functionAtPlus = includePlusValue ? spec.function(x + h) : 0.0;
    sample.functionAtMinus = includeMinusValue ? spec.function(x - h) : 0.0;
    sample.numericalDerivative = calculator(spec.function, x, h);
    sample.absoluteError = 0.0;
    return sample;
}

// Evaluates one test function for all 10 h values.
FunctionResult NumericalDifferentiation::evaluateFunction(
    const TestFunctionSpec &spec,
    double x,
    const function<DifferentiationSample(const TestFunctionSpec &, double, double)> &sampleBuilder) const
{
    FunctionResult result;
    result.spec = spec;
    result.evaluationPoint = x;
    result.exactDerivativeAtPoint = spec.exactDerivative(x);

    // Calculates numerical derivative and absolute error for all h values.
    for (double h : hValues)
    {
        DifferentiationSample sample = sampleBuilder(spec, x, h);
        sample.absoluteError = abs(result.exactDerivativeAtPoint - sample.numericalDerivative);
        result.samples.push_back(sample);
    }

    return result;
}

// Combines the results of all four functions for the selected method.
MethodResult NumericalDifferentiation::buildMethodResult(
    const string &methodName,
    const string &outputFile,
    const string &graphPrefix,
    double x,
    const function<DifferentiationSample(const TestFunctionSpec &, double, double)> &sampleBuilder) const
{
    MethodResult result;
    result.methodName = methodName;
    result.outputFile = outputFile;
    result.graphPrefix = graphPrefix;

    for (const TestFunctionSpec &spec : testFunctions)
        result.functionResults.push_back(evaluateFunction(spec, x, sampleBuilder));

    return result;
}

// Evaluates Forward Difference for all test functions at x.
MethodResult NumericalDifferentiation::processForwardDifference(double x) const
{
    // A lambda passes the selected differentiation operation without duplicating the evaluation loop.
    return buildMethodResult(
        "Forward Difference",
        "Output/ForwardDifference.txt",
        "ForwardDifference",
        x,
        [this](const TestFunctionSpec &spec, double evaluationPoint, double h) {
            DifferentiationSample sample{};
            sample.h = h;
            sample.functionAtPlus = spec.function(evaluationPoint + h);
            sample.functionAtMinus = 0.0;
            sample.numericalDerivative = forwardDifference(spec.function, evaluationPoint, h);
            return sample;
        });
}

// Evaluates Backward Difference for all test functions at x.
MethodResult NumericalDifferentiation::processBackwardDifference(double x) const
{
    // A lambda passes the selected differentiation operation without duplicating the evaluation loop.
    return buildMethodResult(
        "Backward Difference",
        "Output/BackwardDifference.txt",
        "BackwardDifference",
        x,
        [this](const TestFunctionSpec &spec, double evaluationPoint, double h) {
            DifferentiationSample sample{};
            sample.h = h;
            sample.functionAtPlus = 0.0;
            sample.functionAtMinus = spec.function(evaluationPoint - h);
            sample.numericalDerivative = backwardDifference(spec.function, evaluationPoint, h);
            return sample;
        });
}

// Evaluates Central Difference for all test functions at x.
MethodResult NumericalDifferentiation::processCentralDifference(double x) const
{
    // A lambda passes the selected differentiation operation without duplicating the evaluation loop.
    return buildMethodResult(
        "Central Difference",
        "Output/CentralDifference.txt",
        "CentralDifference",
        x,
        [this](const TestFunctionSpec &spec, double evaluationPoint, double h) {
            DifferentiationSample sample{};
            sample.h = h;
            sample.functionAtPlus = spec.function(evaluationPoint + h);
            sample.functionAtMinus = spec.function(evaluationPoint - h);
            sample.numericalDerivative = centralDifference(spec.function, evaluationPoint, h);
            return sample;
        });
}

// Writes one function's numerical results as a formatted table.
void NumericalDifferentiation::writeFunctionBlock(ofstream &out,
                                                 const FunctionResult &result,
                                                 const string &methodName) const
{
    auto formatScientific = [](double value) {
        ostringstream stream;
        stream << scientific << setprecision(12) << value;
        return stream.str();
    };

    auto formatFixed = [](double value) {
        ostringstream stream;
        stream << fixed << setprecision(6) << value;
        return stream.str();
    };

    out << "Function: " << result.spec.name << '\n';
    out << "Expression: f(x) = " << result.spec.expression << '\n';
    out << "Derivative: f'(x) = " << result.spec.derivativeExpression << '\n';
    out << "Evaluation point: x = " << formatFixed(result.evaluationPoint) << '\n';
    out << "Exact derivative at x = " << formatScientific(result.exactDerivativeAtPoint) << '\n';
    out << '\n';

    out << left;

    if (methodName == "Forward Difference")
    {
        out << setw(18) << "h" << " | "
            << setw(22) << "f(x+h)" << " | "
            << setw(24) << "Numerical Derivative" << " | "
            << setw(20) << "Absolute Error" << '\n';
        out << string(98, '-') << '\n';
        for (const DifferentiationSample &sample : result.samples)
        {
            out << setw(18) << formatScientific(sample.h) << " | "
                << setw(22) << formatScientific(sample.functionAtPlus) << " | "
                << setw(24) << formatScientific(sample.numericalDerivative) << " | "
                << setw(20) << formatScientific(sample.absoluteError) << '\n';
        }
    }
    else if (methodName == "Backward Difference")
    {
        out << setw(18) << "h" << " | "
            << setw(22) << "f(x-h)" << " | "
            << setw(24) << "Numerical Derivative" << " | "
            << setw(20) << "Absolute Error" << '\n';
        out << string(98, '-') << '\n';
        for (const DifferentiationSample &sample : result.samples)
        {
            out << setw(18) << formatScientific(sample.h) << " | "
                << setw(22) << formatScientific(sample.functionAtMinus) << " | "
                << setw(24) << formatScientific(sample.numericalDerivative) << " | "
                << setw(20) << formatScientific(sample.absoluteError) << '\n';
        }
    }
    else
    {
        out << setw(18) << "h" << " | "
            << setw(22) << "f(x+h)" << " | "
            << setw(22) << "f(x-h)" << " | "
            << setw(24) << "Numerical Derivative" << " | "
            << setw(20) << "Absolute Error" << '\n';
        out << string(124, '-') << '\n';
        for (const DifferentiationSample &sample : result.samples)
        {
            out << setw(18) << formatScientific(sample.h) << " | "
                << setw(22) << formatScientific(sample.functionAtPlus) << " | "
                << setw(22) << formatScientific(sample.functionAtMinus) << " | "
                << setw(24) << formatScientific(sample.numericalDerivative) << " | "
                << setw(20) << formatScientific(sample.absoluteError) << '\n';
        }
    }

    out << defaultfloat << '\n';
}

// Saves all results of the selected method to its output file.
void NumericalDifferentiation::writeResultsToFile(const MethodResult &result) const
{
    ensureDirectory("Output");

    ofstream out(result.outputFile);
    if (!out)
        throw runtime_error("Cannot open output file: " + result.outputFile);

    out << "========================================\n";
    out << "     NUMERICAL DIFFERENTIATION\n";
    out << "========================================\n\n";
    out << "Method: " << result.methodName << "\n\n";

    for (size_t index = 0; index < result.functionResults.size(); ++index)
    {
        writeFunctionBlock(out, result.functionResults[index], result.methodName);
        if (index + 1 < result.functionResults.size())
            out << "\n";
    }
}
