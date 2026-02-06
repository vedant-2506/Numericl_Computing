#include<iostream>
#include "Complex.hpp"
using namespace std;

int main(){

    //create object 
    Complex c1,c2,ans;

    //Enter number
    //cout<<"Enter Complex numbers: "<<endl

    //1st
    cout<<"Enter 1st Complex number :"<<endl;
    cout<<"Enter real part :";
    cin>>c1.real;

    cout<<"Enter imginary part :";
    cin>>c1.img;


    //2nd
    cout<<"\nEnter 2nd Complex number :"<<endl;
    cout<<"Enter real part :";
    cin>>c2.real;

    cout<<"Enter imginary part :";
    cin>>c2.img;

    //addition
    ans = c1.add(c2);
    cout << "\n Addition is :  "<< ans.real << " + ("<<ans.img<< ")i";

    //substrction
    ans=c1.subtract(c2);
    cout<< "\n Subtraction: "<< ans.real << " + ("<<ans.img << ")i";
    
    //multiplication
    ans=c1.multiply(c2);
    cout<<"\n Multiplication:"<<ans.real << " + ("<<ans.img << ")i";

    //division
    ans=c1.divide(c2);
    cout << "\n Division:"<<ans.real << " + (" <<ans.img << ")i \n";

    //conjugate 
    //1st
    Complex conj1 = c1.conjugate();
    cout << "\n Conjugate of 1st  complex number : "<< conj1.real << " + (" << conj1.img << ")i";

    //2nd
    Complex conj2 = c2.conjugate();
    cout << "\n Conjugate of 2nd  complex number : "<< conj2.real << " + (" << conj2.img << ")i \n";

    
    //norm
    //1st
    cout << "\n Norm of 1st complex number: "<< c1.norm();

    //2nd
    cout << "\n Norm of 2nd  complex number " << c2.norm()<< endl;

 
    return 0;
}