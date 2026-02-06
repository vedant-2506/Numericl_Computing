#include <iostream>
#include "Root.hpp"

int main()
{
    // create object of bisect class
    bisect obj(0.0001);   // tolerance passed to constructor

    // call solve function using object
    double root = obj.solve();

    // print approximate root
    std::cout << "\nApproximate Root = " << root << std::endl;

    return 0;
}
