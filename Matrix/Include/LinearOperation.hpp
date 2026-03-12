#ifndef LINEAR_OPERATION_HPP             // prevent multiple inclusion
#define LINEAR_OPERATION_HPP             // define header guard

#include "Matrix.hpp"                    // include base Matrix class
#include <string>                        // for std::string
#include <iostream>                      // for std::ostream

class LinearOperation : public Matrix    // inherits Matrix base class
{
public:                                  // accessible by everyone

    // constructors
    LinearOperation();                   // default constructor
    LinearOperation(int r, int c);       // parameterized constructor

    // augmented matrix builder
    static void generateAugmentedMatrixFile(        // build [A|b] file
        const std::string &leftFile,                // coefficient matrix A file
        const std::string &rightFile,               // constant vector b file
        const std::string &outputFile);             // output augmented file

    // friend stream operator
    friend std::ostream& operator<<(std::ostream &out, const LinearOperation &lo); // print
};

#endif                                   // end header guard