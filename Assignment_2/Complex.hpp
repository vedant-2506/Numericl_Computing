
using namespace std;

class Complex
{
public:
    //declear
    float real,img;
    
    // default constructor
    Complex();
    
    // parameterized constructor
    Complex(float r,float i);

    //addition
    Complex add(Complex &c);

    //subtraction
    Complex subtract(Complex &c);

    //multiplication
    Complex multiply(Complex &c);

    //division
    Complex divide(Complex &c);

    //conjugate
    Complex conjugate();

    //norm
    float norm();

};