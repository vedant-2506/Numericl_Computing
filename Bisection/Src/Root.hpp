#ifndef ROOT_HPP
#define ROOT_HPP

#include <iostream>

//base class
class RootFinder{
protected:
    double tol;  //tolerance value

public:
    RootFinder(double tolerance = 0.0001);
    virtual ~RootFinder() = default;

    virtual double f(double x);   //equation f(x)

    virtual double solve() = 0;    //method to find root
};

//Bisection Methode
class Bisection : public RootFinder{
private:
    double a,b;
    bool findInterval();
public:
    Bisection(double tolerance = 0.0001);
    double solve() override;  //override solve method
};

// Newton-Raphson Method
class NewtonRaphson : public RootFinder{
private:
    double df(double x); // derivative of f(x)
public:
    NewtonRaphson(double tolerance = 0.0001);
    double solve() override;  //override solve method
};

//Fixed Point Iteration Method
class FixedPoint : public RootFinder{
private:
    double g(double x);   // function g(x) for fixed point iteration
public:
    FixedPoint(double tolerance = 0.0001);
    double solve() override;  //override solve method
};

#endif // ROOT_HPP