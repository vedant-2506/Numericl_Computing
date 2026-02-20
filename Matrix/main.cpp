#include "./Src/Gaussian.hpp"   
#include <iostream>
using namespace std;

int main()
{
    try
    {
        int r, c;

        // first matrix
        cout << "Enter rows and columns for First Matrix: ";
        cin >> r >> c;

        Matrix M1(r, c);
        M1.inputData();

        cout << "\nFirst Matrix:\n";
        M1.printData();

        // second matrix
        cout << "\nEnter rows and columns for Second Matrix: ";
        cin >> r >> c;

        Matrix M2(r, c);
        M2.inputData();

        cout << "\nSecond Matrix:\n";
        M2.printData();

        // Addition
        Matrix Sum = M1.addMatrix(M2);
        cout << "\nAddition Result:\n";
        Sum.printData();

        // Subtraction
        Matrix Diff = M1.subMatrix(M2);
        cout << "\nSubtraction Result:\n";
        Diff.printData();

        // Gaussian Elimination
        cout << "\nGaussian Elimination \n";
        cout << "Enter number of equations: ";
        cin >> r;

        Matrix G(r, r + 1);

        cout << "Enter augmented matrix values:\n";
        G.inputData();

        cout << "\nAugmented Matrix:\n";
        G.printData();

        G.performGaussian();   // Now performs full RREF
        G.solveBack();         // Extract solution
    }
    catch(const exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
