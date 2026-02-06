#include<iostream>
#include<cmath>
#include "Complex.hpp"


//default constructor
Complex::Complex()
{
    
}

//parameterized constructor
Complex::Complex(float r, float i)
{
    real = r;
    img = i;
}

//addition
Complex Complex::add(Complex &c)
{
    return Complex(real + c.real, img + c.img);
}


//subtraction
Complex Complex::subtract(Complex &c)
{
    return Complex(real-c.real,img-c.img);
}

//multiplication
Complex Complex::multiply(Complex &c)
{
    float r =real*c.real-img*c.img;
    float i = real*c.img+img*c.real;

    return Complex(r, i);
}

//division
Complex Complex::divide(Complex &c)
{
    //denominator
    float denom =c.real*c.real+c.img*c.img;


    float r=(real*c.real+img*c.img)/denom;
    float i=(img * c.real - real * c.img) / denom;
    
    return Complex(r,i);
}


//conjugate
Complex Complex::conjugate()
{
    return Complex(real,-img);
}


//norm
float Complex::norm()
{
    return sqrt(real*real+img*img);
}