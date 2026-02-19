#ifndef ROOT_HPP
#define ROOT_HPP

//base class
class RootFinder{
protected:
    double tol;  //tolerance value

public:
    RootFinder(double tolerance = 0.0001);
    virtual ~RootFinder() = default;    //initialize virtual destructor

    virtual double f(double x);   //equation f(x)

    virtual double solve() = 0;    //method to find root pure virtual function
};

//1) Bisection Method
class Bisection : public RootFinder{
private:
    double a,b;
    bool findInterval();
public:
    Bisection(double tolerance = 0.0001);
    double solve() override;  //override solve method
};

// 2)Newton-Raphson Method
class NewtonRaphson : public RootFinder{
private:
    double df(double x); // derivative of f(x)
public:
    NewtonRaphson(double tolerance = 0.0001);
    double solve() override;  //override solve method
};

//3)Fixed Point Iteration Method
class FixedPoint : public RootFinder{
private:
    double g(double x);   // function g(x) for fixed point iteration
public:
    FixedPoint(double tolerance = 0.0001);
    double solve() override;  //override solve method
};

#endif // ROOT_HPP