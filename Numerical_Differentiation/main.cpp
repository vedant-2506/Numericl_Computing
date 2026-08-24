#include <iostream>
#include <stdexcept>

#include "Include/GraphHelper.hpp"
#include "Include/NumericalDifferentiation.hpp"

using namespace std;

int main() {
  try {
    NumericalDifferentiation differentiation;
    GraphHelper graphHelper;

    int choice = 0;

    cout << "NUMERICAL DIFFERENTIATION\n";
    cout << "CHOICE(1 to 4):\n";
    cout << "1. Forward Difference\n";
    cout << "2. Backward Difference\n";
    cout << "3. Central Difference\n";
    cout << "4. Exit\n\n";
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
