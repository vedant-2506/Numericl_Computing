#ifndef LU_DECOMPOSITION_HPP // prevent multiple include
#define LU_DECOMPOSITION_HPP // start header guard

#include "LinearOperation.hpp" // include linear operation class
#include <string> // string library

using namespace std; // use standard namespace

class LUDecomposition : public LinearOperation // inherit LinearOperation
{
public: // public members

    static void doolittleLU(const string &Afile, const string &bfile); // doolittle method

    static void croutLU(const string &Afile, const string &bfile); // crout method

    static void choleskyDecomposition(const string &Afile, const string &bfile); // cholesky method
};

#endif // end header guard