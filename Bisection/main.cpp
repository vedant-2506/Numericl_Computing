#include <iostream>
#include "../Src/Root.hpp"

int main()
{
    RootFinder* solver;    //Pointer Declaration

    std::cout << "1. Bisection\n";
    std::cout << "2. Newton Raphson\n";
    std::cout << "3. Fixed Point\n";
    std::cout << "Choose Method: ";

    int choice;
    std::cin >> choice;

    if (choice == 1)
        solver = new Bisection();
    else if (choice == 2)
        solver = new NewtonRaphson();
    else
        solver = new FixedPoint();

    double root = solver->solve();   //Finding root using chosen method

    std::cout << "\nApproximate Root = " << root << std::endl;

    delete solver;   //Deleting object

    return 0;
}
