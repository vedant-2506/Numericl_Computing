#include<iostream>
#include<cmath>
#include <iomanip>
#include "Root.hpp"


// constructor definition: initialize members
bisect::bisect(double tolerance)
{
    tol = tolerance;   // initialize tolerance member
    a = 0.0;
    b = 0.0;
}

// This is the equation 
double bisect::f(double x)
{
    // return the calculated value of the equation

    return  4 * x * x * x - 3 * x;     //// f(x) = 4x^3 - 3x
}


// function to find a valid interval automatically f(left) * f(right) < 0
int bisect::findInterval()
{
    
    double step = 0.05;         // step size used to scan the x-axis

    
    for (int i = -100; i < 100; i++)
    {
        
        double left = i * step;   // calculate left point of interval

        
        double right = left + step;   // calculate right point of interval

        
        if (f(left) * f(right) < 0)    // check if function values have opposite signs
        {
            // store the valid interval
            a = left;
            b = right;

            
            return 1;   // interval found 
        }
    }

    return 0;     // interval not found
}



// Function for  Bisection Method
double bisect::solve()
{
    // find a valid interval 
    if (!findInterval())
    {
        // If interval is not found
        std::cout << "Could not find a valid interval\n";
        return 0;
    }


    std::cout << "Starting interval: [" << a << ", " << b << "]\n";   //// starting interval

    std::cout << "Tolerance: " << tol << "\n\n";     // tolerance value

    std::cout  << "Iter\t" 
               << "\t a \t" 
               << "\t b\t" 
              << "\t m\t" 
              << "\tf(m)\n";    // table header 

    
    double m_prev = a;   //previous midpoint value 

    double m_curr;      //current midpoint value
    
    int iter = 1;      //Iteration 
    
    while (true)
    {
        
        m_curr = (a + b) / 2;    // Calculate midpoint 

        
        double fm = f(m_curr);     // calculate function value at midpoint

        // Print  iteration values
        std::cout << iter << "\t "  
         << std::setw(10) << a << "\t "  
          << std::setw(10) << b << "\t " 
          << std::setw(10) << m_curr << "\t "  
          << fm << 
          std::endl;



        if (std::abs(m_curr - m_prev) < tol)      // check stopping condition
            break;


        
        if (f(a) * fm < 0)   // update interval 
            b = m_curr;     
        else
            a = m_curr;     



        // update previous midpoint
        m_prev = m_curr;

        
        iter++;
    }

  
    return m_curr;
}
