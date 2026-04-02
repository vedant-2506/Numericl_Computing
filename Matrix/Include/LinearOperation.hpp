#ifndef LINEAR_OPERATION_HPP                      // prevent multiple inclusion
#define LINEAR_OPERATION_HPP                      // define header guard

#include "Matrix.hpp"                    // include base Matrix class
#include <string>                        // for std::string
#include <iostream>                      // for std::ostream

class LinearOperation : public Matrix             // declare class
{ 
public:

    LinearOperation();                            // call function or method
    LinearOperation(int r, int c);                // declare and initialize object

    void generateAugmentedMatrixFile(             // execute statement
        const std::string &leftFile,              // declare variable
        const std::string &rightFile,             // declare variable
        const std::string &outputFile);           // declare variable

    friend std::ostream& operator<<(std::ostream &out, const LinearOperation &lo);  // declare function
};

#endif                                            // end header guard