#pragma once

#include <string>

class NumericalDifferences
{
private:
    std::string functionExpression;
    std::string derivativeExpression;
    double xValue;

    double evaluateExpression(const std::string& expression, double x) const;
    double evaluateFunction(double x) const;
    double evaluateDerivative(double x) const;

    static std::string formatPowerOfTen(int exponent);
    static void printDivider(char fillCharacter, int width);
    static void printHeader(const std::string& title);

public:
    NumericalDifferences();
    NumericalDifferences(const std::string& functionExpression,
                         const std::string& derivativeExpression,
                         double xValue);

    void setFunction(const std::string& functionExpression);
    void setDerivative(const std::string& derivativeExpression);
    void setX(double xValue);

    const std::string& getFunction() const;
    const std::string& getDerivative() const;
    double getX() const;

    double forwardDifference(double h) const;
    double backwardDifference(double h) const;
    double centralDifference(double h) const;

    void displayForwardTable() const;
    void displayBackwardTable() const;
    void displayCentralTable() const;
    void runAllMethods() const;
};