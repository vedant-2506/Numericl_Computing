#ifndef GAUSSIAN_ELIMINATION_HPP // prevent multiple include
#define GAUSSIAN_ELIMINATION_HPP // start header guard

#include "LinearOperation.hpp" // include linear operation class
#include <string> // string support

using namespace std; // use standard namespace

class GaussianElimination : public LinearOperation // inherit LinearOperation
{
public: // public members

    static void gaussianEliminationWithPivoting( // gaussian elimination method
        Matrix &matrix, // augmented matrix input
        ofstream &matrixOut, // output matrix file
        ofstream &vectorOut); // output solution vector file
};

#endif // end header guard