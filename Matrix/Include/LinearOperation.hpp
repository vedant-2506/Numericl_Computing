#ifndef LINEAR_OPERATION_HPP // prevent multiple include
#define LINEAR_OPERATION_HPP // start header guard

#include "Matrix.hpp" // include matrix class
#include <string> // string library

using namespace std; // use standard namespace

class LinearOperation : public Matrix // inherit Matrix class
{
public: // public members

    LinearOperation(); // default constructor

    LinearOperation(int r, int c); // parameter constructor

    static void generateAugmentedMatrixFile( // create augmented matrix
        const string &leftFile, // input matrix A file
        const string &rightFile, // input vector b file
        const string &outputFile); // output augmented file
};

#endif // end header guard