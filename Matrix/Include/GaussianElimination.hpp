#ifndef GAUSSIAN_ELIMINATION_HPP                  // prevent multiple inclusion
#define GAUSSIAN_ELIMINATION_HPP                  // define header guard

#include "LinearOperation.hpp"                        // include parent class
#include <fstream>                                    // for std::ofstream

class GaussianElimination : public LinearOperation  // declare class
{
public:

    GaussianElimination();                        // call function or method
    GaussianElimination(int r, int c);            // declare and initialize object

    friend std::ostream& operator<<(std::ostream &out, const GaussianElimination &ge);  // declare function

    void gaussianWithPivoting(                    // execute statement
        Matrix &matrix,                           // execute statement
        std::ofstream &matrixOut,                 // handle file stream operation
        std::ofstream &vectorOut);                // handle file stream operation

    void gaussianWithoutPivoting(                 // execute statement
        Matrix &matrix,                           // execute statement
        std::ofstream &matrixOut,                 // handle file stream operation
        std::ofstream &vectorOut);                // handle file stream operation
};

#endif                                            // end header guard