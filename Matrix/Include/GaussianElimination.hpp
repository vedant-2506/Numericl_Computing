#ifndef GAUSSIAN_ELIMINATION_HPP                      // prevent multiple inclusion
#define GAUSSIAN_ELIMINATION_HPP                      // define header guard

#include "LinearOperation.hpp"                        // include parent class
#include <fstream>                                    // for std::ofstream

class GaussianElimination : public LinearOperation    // inherits LinearOperation -> Matrix
{
public:                                               // accessible by everyone

    // constructors
    GaussianElimination();                            // default constructor
    GaussianElimination(int r, int c);                // parameterized constructor

    // friend stream operator
    friend std::ostream& operator<<(std::ostream &out, const GaussianElimination &ge);

    // Gaussian WITH partial pivoting  (menu choice 7)
    static void gaussianWithPivoting(
        Matrix &matrix,                               // augmented matrix [A|b]
        std::ofstream &matrixOut,                     // upper triangular output file
        std::ofstream &vectorOut);                    // solution vector output file

    // Gaussian WITHOUT pivoting  (menu choice 8)
    static void gaussianWithoutPivoting(
        Matrix &matrix,                               // augmented matrix [A|b]
        std::ofstream &matrixOut,                     // upper triangular output file
        std::ofstream &vectorOut);                    // solution vector output file
};

#endif                                                // end header guard