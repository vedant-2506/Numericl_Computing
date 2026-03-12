#ifndef LU_DECOMPOSITION_HPP             // prevent multiple inclusion
#define LU_DECOMPOSITION_HPP             // define header guard

#include "LinearOperation.hpp"           // include LinearOperation
#include <string>                        // for std::string
#include <iostream>                      // for std::ostream

class LUDecomposition : public LinearOperation  // inherits LinearOperation -> Matrix
{
public:                                  // accessible by everyone

    // constructors
    LUDecomposition();                   // default constructor
    LUDecomposition(int r, int c);       // parameterized constructor

    static void doolittleLU(             // Doolittle: L diagonal = 1
        const std::string &Afile,        // coefficient matrix file
        const std::string &bfile);       // constant vector file

    static void croutLU(                 // Crout: U diagonal = 1
        const std::string &Afile,        // coefficient matrix file
        const std::string &bfile);       // constant vector file

    static void choleskyDecomposition(   // Cholesky: A = L * L^T
        const std::string &Afile,        // coefficient matrix file
        const std::string &bfile);       // constant vector file

    // friend stream operator
    friend std::ostream& operator<<(std::ostream &out, const LUDecomposition &lu);
};

#endif                                   // end header guard