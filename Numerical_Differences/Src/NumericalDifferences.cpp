#include "../Include/NumericalDifferences.hpp"

#include <cmath>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace
{
class ExpressionParser
{
private:
    const string& expression;
    double x;
    size_t position;

    void skipSpaces()
    {
        while (position < expression.size() && isspace(static_cast<unsigned char>(expression[position])))
        {
            ++position;
        }
    }

    bool match(char character)
    {
        skipSpaces();
        if (position < expression.size() && expression[position] == character)
        {
            ++position;
            return true;
        }
        return false;
    }

    double parseExpression()
    {
        double value = parseTerm();
        while (true)
        {
            if (match('+'))
            {
                value += parseTerm();
            }
            else if (match('-'))
            {
                value -= parseTerm();
            }
            else
            {
                break;
            }
        }
        return value;
    }

    double parseTerm()
    {
        double value = parsePower();
        while (true)
        {
            if (match('*'))
            {
                value *= parsePower();
            }
            else if (match('/'))
            {
                double denominator = parsePower();
                if (denominator == 0.0)
                {
                    throw runtime_error("Division by zero in expression.");
                }
                value /= denominator;
            }
            else
            {
                break;
            }
        }
        return value;
    }

    double parsePower()
    {
        double base = parseUnary();
        skipSpaces();
        if (position < expression.size() && expression[position] == '^')
        {
            ++position;
            double exponent = parsePower();
            return pow(base, exponent);
        }
        return base;
    }

    double parseUnary()
    {
        skipSpaces();
        if (match('+'))
        {
            return parseUnary();
        }
        if (match('-'))
        {
            return -parseUnary();
        }
        return parsePrimary();
    }

    double parseIdentifierOrFunction()
    {
        skipSpaces();
        const size_t start = position;
        while (position < expression.size() && (isalpha(static_cast<unsigned char>(expression[position])) || expression[position] == '_'))
        {
            ++position;
        }

        string name = expression.substr(start, position - start);
        for (char& ch : name)
        {
            ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        }

        if (name == "x")
        {
            return x;
        }
        if (name == "pi")
        {
            return acos(-1.0);
        }
        if (name == "e")
        {
            return exp(1.0);
        }

        if (!match('('))
        {
            throw runtime_error("Unknown identifier: " + name);
        }

        double argument = parseExpression();
        if (!match(')'))
        {
            throw runtime_error("Missing closing parenthesis for function: " + name);
        }

        if (name == "sin") return sin(argument);
        if (name == "cos") return cos(argument);
        if (name == "tan") return tan(argument);
        if (name == "asin") return asin(argument);
        if (name == "acos") return acos(argument);
        if (name == "atan") return atan(argument);
        if (name == "sinh") return sinh(argument);
        if (name == "cosh") return cosh(argument);
        if (name == "tanh") return tanh(argument);
        if (name == "exp") return exp(argument);
        if (name == "log") return log(argument);
        if (name == "sqrt")
        {
            if (argument < 0.0)
            {
                throw runtime_error("Square root of a negative value.");
            }
            return sqrt(argument);
        }
        if (name == "abs") return fabs(argument);

        throw runtime_error("Unsupported function: " + name);
    }

    double parseNumber()
    {
        skipSpaces();
        const size_t start = position;
        bool sawDigit = false;
        bool sawDecimalPoint = false;

        while (position < expression.size())
        {
            char character = expression[position];
            if (isdigit(static_cast<unsigned char>(character)))
            {
                sawDigit = true;
                ++position;
            }
            else if (character == '.' && !sawDecimalPoint)
            {
                sawDecimalPoint = true;
                ++position;
            }
            else
            {
                break;
            }
        }

        if (!sawDigit)
        {
            throw runtime_error("Expected a number at position " + to_string(start + 1) + ".");
        }

        string token = expression.substr(start, position - start);
        return stod(token);
    }

    double parsePrimary()
    {
        skipSpaces();
        if (position >= expression.size())
        {
            throw runtime_error("Unexpected end of expression.");
        }

        char character = expression[position];
        if (character == '(')
        {
            ++position;
            double value = parseExpression();
            if (!match(')'))
            {
                throw runtime_error("Missing closing parenthesis.");
            }
            return value;
        }

        if (isdigit(static_cast<unsigned char>(character)) || character == '.')
        {
            return parseNumber();
        }

        if (isalpha(static_cast<unsigned char>(character)) || character == '_')
        {
            return parseIdentifierOrFunction();
        }

        throw runtime_error(string("Unexpected character: ") + character);
    }

public:
    ExpressionParser(const string& expr, double value)
        : expression(expr), x(value), position(0) {}

    double parse()
    {
        double value = parseExpression();
        skipSpaces();
        if (position != expression.size())
        {
            throw runtime_error("Invalid trailing characters in expression.");
        }
        if (!isfinite(value))
        {
            throw runtime_error("Expression evaluation produced a non-finite value.");
        }
        return value;
    }
};
} // namespace

NumericalDifferences::NumericalDifferences()
    : functionExpression(), derivativeExpression(), xValue(0.0)
{
}

NumericalDifferences::NumericalDifferences(const string& functionExpression,
                                           const string& derivativeExpression,
                                           double xValue)
    : functionExpression(functionExpression), derivativeExpression(derivativeExpression), xValue(xValue)
{
}

void NumericalDifferences::setFunction(const string& functionExpression)
{
    this->functionExpression = functionExpression;
}

void NumericalDifferences::setDerivative(const string& derivativeExpression)
{
    this->derivativeExpression = derivativeExpression;
}

void NumericalDifferences::setX(double xValue)
{
    this->xValue = xValue;
}

const string& NumericalDifferences::getFunction() const
{
    return functionExpression;
}

const string& NumericalDifferences::getDerivative() const
{
    return derivativeExpression;
}

double NumericalDifferences::getX() const
{
    return xValue;
}

double NumericalDifferences::evaluateExpression(const string& expression, double x) const
{
    ExpressionParser parser(expression, x);
    return parser.parse();
}

double NumericalDifferences::evaluateFunction(double x) const
{
    return evaluateExpression(functionExpression, x);
}

double NumericalDifferences::evaluateDerivative(double x) const
{
    // If the user did not provide an exact derivative expression, estimate
    // the derivative numerically using a small central difference step.
    bool hasDerivative = false;
    for (char c : derivativeExpression)
    {
        if (!isspace(static_cast<unsigned char>(c))) { hasDerivative = true; break; }
    }

    if (!hasDerivative || derivativeExpression.empty())
    {
        const double h_ref = 1e-8; // reference small step for estimation
        const double fxh = evaluateFunction(x + h_ref);
        const double fxmh = evaluateFunction(x - h_ref);
        return (fxh - fxmh) / (2.0 * h_ref);
    }

    return evaluateExpression(derivativeExpression, x);
}

string NumericalDifferences::formatPowerOfTen(int exponent)
{
    ostringstream output;
    output << "1e-" << exponent;
    return output.str();
}

void NumericalDifferences::printDivider(char fillCharacter, int width)
{
    cout << string(width, fillCharacter) << endl;
}

void NumericalDifferences::printHeader(const string& title)
{
    const int width = 108;
    printDivider('=', width);
    cout << setw((width + static_cast<int>(title.size())) / 2) << title << endl;
    printDivider('=', width);
}

double NumericalDifferences::forwardDifference(double h) const
{
    const double fx = evaluateFunction(xValue);
    const double fxh = evaluateFunction(xValue + h);
    return (fxh - fx) / h;
}

double NumericalDifferences::backwardDifference(double h) const
{
    const double fx = evaluateFunction(xValue);
    const double fxh = evaluateFunction(xValue - h);
    return (fx - fxh) / h;
}

double NumericalDifferences::centralDifference(double h) const
{
    const double fxh = evaluateFunction(xValue + h);
    const double fxmh = evaluateFunction(xValue - h);
    return (fxh - fxmh) / (2.0 * h);
}

void NumericalDifferences::displayForwardTable() const
{
    try
    {
        const double fx = evaluateFunction(xValue);
        const double exactDerivative = evaluateDerivative(xValue);

        bool hasDerivative = false;
        for (char c : derivativeExpression) if (!isspace(static_cast<unsigned char>(c))) { hasDerivative = true; break; }

        printHeader("FORWARD DIFFERENCE");
        cout << fixed << setprecision(6);
        cout << " x = " << xValue << endl;
        cout << " f(x) = " << fx << endl;
        if (hasDerivative && !derivativeExpression.empty())
            cout << " Exact Derivative = " << exactDerivative << endl;
        else
            cout << " Exact Derivative (estimated) = " << exactDerivative << endl;
        printDivider('-', 108);
        cout << left << setw(12) << "h"
             << setw(18) << "f(x+h)"
             << setw(18) << "f(x)"
             << setw(22) << "Numerical f'(x)"
             << setw(18) << "Exact Derivative"
             << setw(18) << "Absolute Error" << endl;
        printDivider('-', 108);

        for (int exponent = 1; exponent <= 10; ++exponent)
        {
            const double h = pow(10.0, -exponent);
            const double fxh = evaluateFunction(xValue + h);
            const double numericalDerivative = (fxh - fx) / h;
            const double absoluteError = abs(exactDerivative - numericalDerivative);

            cout << left << setw(12) << formatPowerOfTen(exponent)
                 << setw(18) << fxh
                 << setw(18) << fx
                 << setw(22) << numericalDerivative
                 << setw(18) << exactDerivative
                 << setw(18) << absoluteError << endl;
        }

        printDivider('=', 108);
    }
    catch (const exception& exceptionObject)
    {
        cout << "Error: " << exceptionObject.what() << endl;
    }
}

void NumericalDifferences::displayBackwardTable() const
{
    try
    {
        const double fx = evaluateFunction(xValue);
        const double exactDerivative = evaluateDerivative(xValue);

        bool hasDerivative = false;
        for (char c : derivativeExpression) if (!isspace(static_cast<unsigned char>(c))) { hasDerivative = true; break; }

        printHeader("BACKWARD DIFFERENCE");
        cout << fixed << setprecision(6);
        cout << " x = " << xValue << endl;
        cout << " f(x) = " << fx << endl;
        if (hasDerivative && !derivativeExpression.empty())
            cout << " Exact Derivative = " << exactDerivative << endl;
        else
            cout << " Exact Derivative (estimated) = " << exactDerivative << endl;
        printDivider('-', 108);
        cout << left << setw(12) << "h"
             << setw(18) << "f(x)"
             << setw(18) << "f(x-h)"
             << setw(22) << "Numerical f'(x)"
             << setw(18) << "Exact Derivative"
             << setw(18) << "Absolute Error" << endl;
        printDivider('-', 108);

        for (int exponent = 1; exponent <= 10; ++exponent)
        {
            const double h = pow(10.0, -exponent);
            const double fxmh = evaluateFunction(xValue - h);
            const double numericalDerivative = (fx - fxmh) / h;
            const double absoluteError = abs(exactDerivative - numericalDerivative);

            cout << left << setw(12) << formatPowerOfTen(exponent)
                 << setw(18) << fx
                 << setw(18) << fxmh
                 << setw(22) << numericalDerivative
                 << setw(18) << exactDerivative
                 << setw(18) << absoluteError << endl;
        }

        printDivider('=', 108);
    }
    catch (const exception& exceptionObject)
    {
        cout << "Error: " << exceptionObject.what() << endl;
    }
}

void NumericalDifferences::displayCentralTable() const
{
    try
    {
        const double fx = evaluateFunction(xValue);
        const double exactDerivative = evaluateDerivative(xValue);

        bool hasDerivative = false;
        for (char c : derivativeExpression) if (!isspace(static_cast<unsigned char>(c))) { hasDerivative = true; break; }

        printHeader("CENTRAL DIFFERENCE");
        cout << fixed << setprecision(6);
        cout << " x = " << xValue << endl;
        cout << " f(x) = " << fx << endl;
        if (hasDerivative && !derivativeExpression.empty())
            cout << " Exact Derivative = " << exactDerivative << endl;
        else
            cout << " Exact Derivative (estimated) = " << exactDerivative << endl;
        printDivider('-', 108);
        cout << left << setw(12) << "h"
             << setw(18) << "f(x+h)"
             << setw(18) << "f(x-h)"
             << setw(22) << "Numerical f'(x)"
             << setw(18) << "Exact Derivative"
             << setw(18) << "Absolute Error" << endl;
        printDivider('-', 108);

        for (int exponent = 1; exponent <= 10; ++exponent)
        {
            const double h = pow(10.0, -exponent);
            const double fxh = evaluateFunction(xValue + h);
            const double fxmh = evaluateFunction(xValue - h);
            const double numericalDerivative = (fxh - fxmh) / (2.0 * h);
            const double absoluteError = abs(exactDerivative - numericalDerivative);

            cout << left << setw(12) << formatPowerOfTen(exponent)
                 << setw(18) << fxh
                 << setw(18) << fxmh
                 << setw(22) << numericalDerivative
                 << setw(18) << exactDerivative
                 << setw(18) << absoluteError << endl;
        }

        printDivider('=', 108);
    }
    catch (const exception& exceptionObject)
    {
        cout << "Error: " << exceptionObject.what() << endl;
    }
}

void NumericalDifferences::runAllMethods() const
{
    displayForwardTable();
    displayBackwardTable();
    displayCentralTable();
}