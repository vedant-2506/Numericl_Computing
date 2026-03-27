#ifndef ITERATIVE_METHOD_HPP
#define ITERATIVE_METHOD_HPP

#include "LinearOperation.hpp"
#include <string>
#include <iostream>

class IterativeMethod : public LinearOperation
{
public:
    IterativeMethod();
    IterativeMethod(int r, int c);

    friend std::ostream& operator<<(std::ostream &out, const IterativeMethod &im);

    // Gauss-Jacobi: all updates use PREVIOUS iteration values
    static void gaussJacobi(
        const std::string &Afile,
        const std::string &bfile,
        int    maxIter,
        double tol);

    // Gauss-Seidel: each update uses LATEST available values
    static void gaussSeidel(
        const std::string &Afile,
        const std::string &bfile,
        int    maxIter,
        double tol);
};

#endif