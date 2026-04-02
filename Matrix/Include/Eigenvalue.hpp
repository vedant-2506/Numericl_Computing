#ifndef EIGEN_VALUE_HPP                           // prevent multiple inclusion
#define EIGEN_VALUE_HPP                           // define header guard

#include "Matrix.hpp"                                // include base Matrix class
#include <string>                                    // for std::string
#include <vector>                                    // for std::vector
#include <iostream>                                  // for std::ostream

class EigenValue : public Matrix                  // declare class
{ 
public:

    EigenValue();                                 // call function or method
    EigenValue(int r, int c);                     // declare and initialize object

    friend std::ostream& operator<<(std::ostream &out, const EigenValue &ev);  // declare function

    
    void gerschgorin(const std::string &Afile);   // declare function
};

#endif                                            // end header guard