#ifndef TEST_FUNCTION_HPP
#define TEST_FUNCTION_HPP

#include <cmath>
#include <string>

// Abstract base class representing a mathematical test function for numerical differentiation.
class TestFunction
{
public:
    virtual ~TestFunction() = default;

    virtual std::string getName() const = 0;
    virtual std::string getExpression() const = 0;
    virtual std::string getDerivativeExpression() const = 0;
    virtual std::string getGraphStem() const = 0;

    virtual double evaluate(double x) const = 0;
    virtual double exactDerivative(double x) const = 0;

    virtual long double evaluateLong(long double x) const { return evaluate(static_cast<double>(x)); }
    virtual long double exactDerivativeLong(long double x) const { return exactDerivative(static_cast<double>(x)); }
};

// f(x) = e^x
class ExponentialFunction : public TestFunction
{
public:
    std::string getName() const override { return "e^x"; }
    std::string getExpression() const override { return "f(x) = exp(x)"; }
    std::string getDerivativeExpression() const override { return "f'(x) = exp(x)"; }
    std::string getGraphStem() const override { return "Exp"; }

    double evaluate(double x) const override { return std::exp(x); }
    double exactDerivative(double x) const override { return std::exp(x); }

    long double evaluateLong(long double x) const override { return std::exp(x); }
    long double exactDerivativeLong(long double x) const override { return std::exp(x); }
};

// f(x) = sin(x)
class SineFunction : public TestFunction
{
public:
    std::string getName() const override { return "sin(x)"; }
    std::string getExpression() const override { return "f(x) = sin(x)"; }
    std::string getDerivativeExpression() const override { return "f'(x) = cos(x)"; }
    std::string getGraphStem() const override { return "Sin"; }

    double evaluate(double x) const override { return std::sin(x); }
    double exactDerivative(double x) const override { return std::cos(x); }

    long double evaluateLong(long double x) const override { return std::sin(x); }
    long double exactDerivativeLong(long double x) const override { return std::cos(x); }
};

// f(x) = cos(x)
class CosineFunction : public TestFunction
{
public:
    std::string getName() const override { return "cos(x)"; }
    std::string getExpression() const override { return "f(x) = cos(x)"; }
    std::string getDerivativeExpression() const override { return "f'(x) = -sin(x)"; }
    std::string getGraphStem() const override { return "Cos"; }

    double evaluate(double x) const override { return std::cos(x); }
    double exactDerivative(double x) const override { return -std::sin(x); }

    long double evaluateLong(long double x) const override { return std::cos(x); }
    long double exactDerivativeLong(long double x) const override { return -std::sin(x); }
};

// f(x) = x^2 + 2x + 1
class PolynomialFunction : public TestFunction
{
public:
    std::string getName() const override { return "x^2 + 2x + 1"; }
    std::string getExpression() const override { return "f(x) = x^2 + 2x + 1"; }
    std::string getDerivativeExpression() const override { return "f'(x) = 2x + 2"; }
    std::string getGraphStem() const override { return "Polynomial"; }

    double evaluate(double x) const override { return x * x + 2.0 * x + 1.0; }
    double exactDerivative(double x) const override { return 2.0 * x + 2.0; }

    long double evaluateLong(long double x) const override { return x * x + 2.0L * x + 1.0L; }
    long double exactDerivativeLong(long double x) const override { return 2.0L * x + 2.0L; }
};

#endif
