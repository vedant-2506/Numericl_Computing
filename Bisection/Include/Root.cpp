#include <iostream>
#include <cmath>
#include <iomanip>
#include "../Src/Root.hpp"

//Base class
RootFinder::RootFinder(double tolerance) {
    tol = tolerance;
}

double RootFinder::f(double x) {
    // Define the function f(x) 
    return 4*x*x*x - 3*x; 
}

// Bisection Method
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

    double m_prev = a , m_curr = 0;
    int iter = 1;
    while(true){
        m_curr = (a + b) / 2;
        double fm = f(m_curr);

        std::cout << iter << "\t"
                  << std::setw(10) << a << "\t"
                  << std::setw(10) << b << "\t"
                  << std::setw(10) << m_curr << "\t"
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

// Newton-Raphson Method
NewtonRaphson::NewtonRaphson(double tolerance) : RootFinder(tolerance) {

}

double NewtonRaphson::df(double x) {
    return 12*x*x - 3;   // Derivative of f(x)
}

double NewtonRaphson::solve() {
    double x0 = 0.5; // Initial guess
    double x1 = 0;
    int iter = 1;

    while (true) {
        x1 = x0 - f(x0) / df(x0);
        
        std::cout << iter << "\t"
                  << x0 << "\t"
                  << x1 << std::endl;

        if (std::abs(x1 - x0) < tol)
        break;
    
        x0 = x1;
        iter++;
    }
    return x1;
}

//Fixed Point Iteration Method
FixedPoint::FixedPoint(double tolerance) : RootFinder(tolerance) {}

double FixedPoint::g(double x)
{
    return cbrt((3 * x) / 4);  // rearranged equation
}

// Solve method 
double FixedPoint::solve()
{
    double x0 = 0.5;
    double x1;
    int iter = 1;

    while (true)
    {
        x1 = g(x0);

        std::cout << iter << "\t"
                  << x0 << "\t"
                  << x1 << std::endl;

        if (std::abs(x1 - x0) < tol)
            break;

        x0 = x1;
        iter++;
    }

    return x1;
}