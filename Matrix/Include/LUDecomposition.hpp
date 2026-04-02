#ifndef LU_DECOMPOSITION_HPP                      // prevent multiple inclusion
#define LU_DECOMPOSITION_HPP                      // define header guard

#include "LinearOperation.hpp"           // include LinearOperation
#include <string>                        // for std::string
#include <iostream>                      // for std::ostream

class LUDecomposition : public LinearOperation    // declare class
{ 
public:

    LUDecomposition();                            // call function or method
    LUDecomposition(int r, int c);                // declare and initialize object

    void doolittleLU(                             // execute statement
        const std::string &Afile,                 // declare variable
        const std::string &bfile);                // declare variable

    void croutLU(                                 // execute statement
        const std::string &Afile,                 // declare variable
        const std::string &bfile);                // declare variable

    void choleskyDecomposition(                   // execute statement
        const std::string &Afile,                 // declare variable
        const std::string &bfile);                // declare variable

    friend std::ostream& operator<<(std::ostream &out, const LUDecomposition &lu);  // declare function
};

#endif                                            // end header guard