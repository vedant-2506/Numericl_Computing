#ifndef ROOT_HPP
#define ROOT_HPP

class bisect
{
private:
    //store the left and right interval values
    double a, b;
    // tolerance value for stopping condition
    double tol;

public:
     // constructor to initialize tolerance
    bisect(double tolerance = 0.0001);

    // function representing the mathematical equation f(x)
     double f(double x);

    // function to automatically find an interval [a, b]
    int findInterval();


    // function that applies the bisection method
    double solve();

};

#endif


