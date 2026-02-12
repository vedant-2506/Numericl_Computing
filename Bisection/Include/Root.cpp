#include <iostream>
#include <cmath>
#include <iomanip>
#include "../Src/Root.hpp"

//Base class
RootFinder::RootFinder(double tolerance) {
    tol = tolerance;
}

// Define the function f(x) 
double RootFinder::f(double x) {
    return 4*x*x*x - 3*x;   //test case 1
    //return x*x*x - x -2;    //test case 2
    //return x*x -5;          //test case 3
    //return cos(x) - x;        //test case 4
}

//1) Bisection Method
Bisection::Bisection(double tolerance) : RootFinder(tolerance){
    a = 0;
    b = 0;
}

// Find an interval [a, b] where f(a) and f(b) have opposite signs
bool Bisection::findInterval() {
    double step = 0.05;

    for(int i=-100;i<100;i++){
        double left =i*step;
        double right = left + step;

        if(f(left) * f(right) < 0) {
            a = left;
            b = right;
            return true;
        }
    }
    return false;
}

// Solve method for Bisection
double Bisection::solve() {
    if (!findInterval()) {
        std::cout << "No valid interval found." << std::endl;
        return 0;
    }

    std::cout << "\nBisection Method Table\n";
    std::cout << "Iter"
              << std::setw(15) << "a "
              << std::setw(15) << "b "
              << std::setw(15) << "Mid "
              << std::setw(15) << "f(Mid) "
              << std::endl;

    std::cout << "---------------------------------------------------------------\n";

    double m_prev = a , m_curr = 0;
    int iter = 1;
    while(true){
        m_curr = (a + b) / 2;       //mid point 
        double fm = f(m_curr);     

        std::cout << iter << "\t |"
                  << std::setw(10) << a << "\t |"
                  << std::setw(10) << b << "\t |"
                  << std::setw(10) << m_curr << "\t |"
                  << fm << std::endl;
        
        if (std::abs(m_curr - m_prev) < tol)
            break;

        if (f(a) * fm < 0)
            b = m_curr;
        else
            a = m_curr;

        m_prev = m_curr;
        iter++;
    }

    return m_curr;
    
} 

// 2) Newton-Raphson Method
NewtonRaphson::NewtonRaphson(double tolerance) : RootFinder(tolerance) {

}

// Derivative of f(x)
double NewtonRaphson::df(double x) {
    return 12*x*x - 3;    //test case 1
    //return 3*x*x - 1;      //test case 2
    //return 2*x;            //test case 3
    //return -sin(x) - 1;     //test case 4
}

double NewtonRaphson::solve()
{
    std::cout << "\nNewton-Raphson Method Table\n";
    std::cout << "Iter "
              << std::setw(15) << "x0 "
              << std::setw(15) << "x1 "
              << std::endl;

    std::cout << "--------------------------------------------\n";

    double x0 = 1.0;   // Better initial guess
    double x1 = 0;
    int iter = 1;
    int maxIter = 1000;   // Safety limit

    while (iter <= maxIter)
    {
        double derivative = df(x0);

        // Check if derivative is zero
        if (std::abs(derivative) < 1e-10)
        {
            std::cout << "Derivative is zero. Method fails.\n";
            return x0;
        }

        x1 = x0 - f(x0) / derivative;    //Newton-Raphson formula

        std::cout << iter << "\t |" 
                   <<std::setw(10)<< x0 << "\t |"
                  << std::setw(10)<< x1 << std::endl;

        if (std::abs(x1 - x0) < tol)
            return x1;

        x0 = x1;
        iter++;
    }

    std::cout << "Maximum iterations reached.\n";
    return x1;
}


//3)Fixed Point Iteration Method
FixedPoint::FixedPoint(double tolerance) : RootFinder(tolerance) {}

// rearranged equation
double FixedPoint::g(double x)
{
    return cbrt((3 * x) / 4);  //test case 1
    //return cbrt(x + 2);        //test case 2
    //return sqrt(5);          //test case 3
    //return cos(x);              //test case 4
}

// Solve method 
double FixedPoint::solve()
{
    std::cout << "\nFixed Point Method Table\n";
    std::cout << "Iter"
              << std::setw(15) << "x0"
              << std::setw(15) << "x1"
              << std::endl;

    std::cout << "--------------------------------------------\n";

    double x0 = 0.5;
    double x1;
    int iter = 1;

    while (true)
    {
        x1 = g(x0);

        std::cout << iter << "\t |" 
                  <<std::setw(10)<< x0 << "\t |"
                  << std::setw(10)<< x1 << std::endl;

        if (std::abs(x1 - x0) < tol)
            break;

        x0 = x1;
        iter++;
    }

    return x1;
}