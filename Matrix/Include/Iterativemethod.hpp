#ifndef ITERATIVE_METHOD_HPP                      // prevent multiple inclusion
#define ITERATIVE_METHOD_HPP                      // define header guard

#include "LinearOperation.hpp" // include LinearOperation.hpp
#include <string> // include string
#include <iostream> // include iostream

class IterativeMethod : public LinearOperation    // declare class
{ 
public:
    IterativeMethod();                            // call function or method
    IterativeMethod(int r, int c);                // declare and initialize object

    friend std::ostream& operator<<(std::ostream &out, const IterativeMethod &im);  // declare function

    void gaussJacobi(                             // execute statement
        const std::string &Afile,                 // declare variable
        const std::string &bfile,                 // declare variable
        int    maxIter,                           // declare variable
        double tol);                              // declare variable

    void gaussSeidel(                             // execute statement
        const std::string &Afile,                 // declare variable
        const std::string &bfile,                 // declare variable
        int    maxIter,                           // declare variable
        double tol);                              // declare variable
};

#endif                                            // end header guard