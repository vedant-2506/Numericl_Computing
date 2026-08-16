#include <iostream>
#include <limits>
#include <string>

#include "Include/NumericalDifferences.hpp"

using namespace std;

namespace
{
double readDoubleValue(const string& prompt)
{
    double value = 0.0;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid numeric input. Please try again." << endl;
    }
}

string readExpression(const string& prompt)
{
    string expression;
    cout << prompt;
    getline(cin, expression);
    return expression;
}
} // namespace

int main()
{
    cout << "===============================================\n";
    cout << "       NUMERICAL DIFFERENCES PROGRAM\n";
    cout << "===============================================\n\n";

    string functionExpression;
    while (true)
    {
        functionExpression = readExpression("Enter function f(x): ");
        bool blank = true;
        for (char c : functionExpression) if (!isspace(static_cast<unsigned char>(c))) { blank = false; break; }
        if (!blank) break;
        cout << "Function cannot be blank. Please enter a valid expression like x^2 or sin(x)." << endl;
    }

    string derivativeExpression = readExpression("Enter exact derivative f'(x) (leave blank to estimate): ");
    bool derivativeBlank = true;
    for (char c : derivativeExpression) if (!isspace(static_cast<unsigned char>(c))) { derivativeBlank = false; break; }
    if (derivativeBlank)
    {
        cout << "No exact derivative provided — the program will estimate the derivative numerically and use it as the reference.\n";
    }
    double xValue = readDoubleValue("Enter value of x: ");

    NumericalDifferences numericalDifferences(functionExpression, derivativeExpression, xValue);

    cout << "\nFunction:\n";
    cout << "f(x) = " << numericalDifferences.getFunction() << "\n\n";
    cout << "Derivative:\n";
    cout << "f'(x) = " << numericalDifferences.getDerivative() << "\n\n";
    cout << "x = " << numericalDifferences.getX() << "\n\n";

    while (true)
    {
        int choice = 0;

        cout << "-----------------------------------------------\n";
        cout << "1. Forward Difference\n";
        cout << "2. Backward Difference\n";
        cout << "3. Central Difference\n";
        cout << "4. Run All Methods\n";
        cout << "5. Exit\n";
        cout << "-----------------------------------------------\n";
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a numeric choice." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
                numericalDifferences.displayForwardTable();
                break;
            case 2:
                numericalDifferences.displayBackwardTable();
                break;
            case 3:
                numericalDifferences.displayCentralTable();
                break;
            case 4:
                numericalDifferences.runAllMethods();
                break;
            case 5:
                cout << "Exiting program." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please select a valid option." << endl;
                break;
        }
    }
}