#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "Include/GraphHelper.hpp"
#include "Include/LagrangeInterpolation.hpp"
#include "Include/NewtonInterpolation.hpp"
#include "Include/RichardsonExtrapolation.hpp"

using namespace std;

namespace
{
double readNumber(const string &prompt)
{
  double value = 0.0;
  while (true)
  {
    cout << prompt << flush;
    if (cin >> value && isfinite(value))
      return value;

    cout << "Invalid numeric input. Please try again." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

size_t readPointCount()
{
  while (true)
  {
    cout << "Enter total number of data points: " << flush;
    long long count = 0;
    if (cin >> count && count >= 2)
      return static_cast<size_t>(count);

    cout << "The number of data points must be at least 2." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

bool readYesNo(const string &prompt)
{
  char answer = '\0';
  while (true)
  {
    cout << prompt << flush;
    if (cin >> answer)
    {
      if (answer == 'y' || answer == 'Y')
        return true;
      if (answer == 'n' || answer == 'N')
        return false;
    }

    cout << "Please enter y or n." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

template <typename InterpolationMethod>
void runInterpolation(GraphHelper &graphHelper)
{
  const size_t pointCount = readPointCount();
  vector<double> xValues;
  vector<double> yValues;
  xValues.reserve(pointCount);
  yValues.reserve(pointCount);

  for (size_t index = 0; index < pointCount; ++index)
  {
    xValues.push_back(readNumber("Enter x" + to_string(index) + ": "));
    yValues.push_back(readNumber("Enter f(x" + to_string(index) + "): "));
  }

  InterpolationMethod interpolation(xValues, yValues);
  vector<double> queryValues;
  while (readYesNo("Do you want to find f(x) for another x value? (y/n): "))
  {
    queryValues.push_back(readNumber("Enter x value to find f(x): "));
  }

  const InterpolationResult result = interpolation.buildResult(queryValues);
  interpolation.writeResultsToFile(result);
  cout << "[OUTPUT] Results -> " << result.outputFile << endl;

  try
  {
    graphHelper.generateInterpolationGraph(result);
  }
  catch (const exception &error)
  {
    cout << "Graph generation error: " << error.what() << endl;
  }
}
}

int main() {
  try {
    RichardsonExtrapolation differentiation;
    GraphHelper graphHelper;

    int choice = 0;

    cout << "========================================\n";
    cout << "       NUMERICAL METHODS\n";
    cout << "========================================\n\n";
    cout << "1. Forward Difference\n";
    cout << "2. Backward Difference\n";
    cout << "3. Central Difference\n";
    cout << "4. Richardson Extrapolation\n";
    cout << "5. Lagrange Interpolation\n";
    cout << "6. Newton Interpolation\n";
    cout << "7. Exit\n\n";
    cout << "Enter your choice: " << flush;

    if (!(cin >> choice)) {
      cout << "Invalid input. Please enter a numeric menu choice." << endl;
      return 0;
    }

    MethodResult result;
    switch (choice) {
    case 1:
      result = differentiation.processForwardDifference(1.0);
      break;
    case 2:
      result = differentiation.processBackwardDifference(1.0);
      break;
    case 3:
      result = differentiation.processCentralDifference(1.0);
      break;
    case 4:
      result = differentiation.processRichardsonExtrapolation(1.0);
      break;
    case 5:
      runInterpolation<LagrangeInterpolation>(graphHelper);
      return 0;
    case 6:
      runInterpolation<NewtonInterpolation>(graphHelper);
      return 0;
    case 7:
      cout << "Exit." << endl;
      return 0;
    default:
      cout << "Invalid choice." << endl;
      return 0;
    }

    differentiation.writeResultsToFile(result);
    cout << "[OUTPUT] Results -> " << result.outputFile << endl;

    graphHelper.generateGraphs(result);
  } catch (const exception &e) {
    cout << "Error: " << e.what() << endl;
  }

  return 0;
}
