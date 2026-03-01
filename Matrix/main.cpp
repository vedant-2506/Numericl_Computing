#include <iostream>                          // for input-output
#include <fstream>                           // for file handling
#include "Include/BasicOperation.hpp"        // include basic operations
#include "Include/AdvancedOperation.hpp"     // include advanced operations

using namespace std;

int main()
{
    try
    {
        int choice;                          // variable to store user choice

        // Display menu
        cout << "\n========== MATRIX OPERATIONS MENU ==========\n";
        cout << "Select the operation you want to perform:\n\n";
        cout << "1. Matrix Addition\n";
        cout << "2. Matrix Subtraction\n";
        cout << "3. Matrix Multiplication\n";
        cout << "4. Scalar Division\n";
        cout << "5. Transpose\n";
        cout << "6. Determinant\n";
        cout << "7. Gaussian Elimination\n";
        cout << "Enter your choice: ";
        cin >> choice;                       // read user choice

        // ADDITION
        if (choice == 1)
        {
            string file1, file2;             // input file names

            cout << "Enter first matrix file: ";
            cin >> file1;

            cout << "Enter second matrix file: ";
            cin >> file2;

            ifstream fin1(file1);            // open first file
            ifstream fin2(file2);            // open second file

            int r1, c1, r2, c2;              // store dimensions

            fin1 >> r1 >> c1;                // read dimensions
            fin2 >> r2 >> c2;

            if (r1 != r2 || c1 != c2)        // check dimension match
                throw runtime_error("Dimension mismatch for addition");

            BasicOperation A(r1, c1);        // create first matrix
            BasicOperation B(r2, c2);        // create second matrix

            A.readFromFile(fin1);            // read data
            B.readFromFile(fin2);

            BasicOperation result = A.add(B); // perform addition

            ofstream fout("result.txt");     // output file
            fout << r1 << " " << c1 << endl; // write dimension
            result.displayToFile(fout);      // write result

            cout << "Result saved in result.txt\n";
        }

        // SUBTRACTION
        
        else if (choice == 2)
        {
            string file1, file2;

            cout << "Enter first matrix file: ";
            cin >> file1;

            cout << "Enter second matrix file: ";
            cin >> file2;

            ifstream fin1(file1);
            ifstream fin2(file2);

            int r1, c1, r2, c2;

            fin1 >> r1 >> c1;
            fin2 >> r2 >> c2;

            if (r1 != r2 || c1 != c2)
                throw runtime_error("Dimension mismatch for subtraction");

            BasicOperation A(r1, c1);
            BasicOperation B(r2, c2);

            A.readFromFile(fin1);
            B.readFromFile(fin2);

            BasicOperation result = A.subtract(B); // subtraction

            ofstream fout("result.txt");
            fout << r1 << " " << c1 << endl;
            result.displayToFile(fout);

            cout << "Result saved in result.txt\n";
        }

        // MULTIPLICATION
        else if (choice == 3)
        {
            string file1, file2;

            cout << "Enter first matrix file: ";
            cin >> file1;

            cout << "Enter second matrix file: ";
            cin >> file2;

            ifstream fin1(file1);
            ifstream fin2(file2);

            int r1, c1, r2, c2;

            fin1 >> r1 >> c1;
            fin2 >> r2 >> c2;

            if (c1 != r2)                    // multiplication condition
                throw runtime_error("Invalid dimensions for multiplication");

            BasicOperation A(r1, c1);
            BasicOperation B(r2, c2);

            A.readFromFile(fin1);
            B.readFromFile(fin2);

            BasicOperation result = A.multiply(B); // multiply

            ofstream fout("result.txt");
            fout << r1 << " " << c2 << endl;
            result.displayToFile(fout);

            cout << "Result saved in result.txt\n";
        }

        // SCALAR DIVISION
        else if (choice == 4)
        {
            string file;
            double value;                    // scalar value

            cout << "Enter matrix file: ";
            cin >> file;

            cout << "Enter scalar value: ";
            cin >> value;

            ifstream fin(file);

            int r, c;
            fin >> r >> c;

            BasicOperation A(r, c);
            A.readFromFile(fin);

            BasicOperation result = A.scalarDivide(value);

            ofstream fout("result.txt");
            fout << r << " " << c << endl;
            result.displayToFile(fout);

            cout << "Result saved in result.txt\n";
        }

        // TRANSPOSE
        else if (choice == 5)
        {
            string file;

            cout << "Enter matrix file: ";
            cin >> file;

            ifstream fin(file);

            int r, c;
            fin >> r >> c;

            BasicOperation A(r, c);
            A.readFromFile(fin);

            BasicOperation result = A.transpose();

            ofstream fout("result.txt");
            fout << c << " " << r << endl;
            result.displayToFile(fout);

            cout << "Result saved in result.txt\n";
        }

        // DETERMINANT
        else if (choice == 6)
        {
            string file;

            cout << "Enter square matrix file: ";
            cin >> file;

            ifstream fin(file);

            int r, c;
            fin >> r >> c;

            if (r != c)                      // determinant only for square
                throw runtime_error("Matrix must be square");

            BasicOperation A(r, c);
            A.readFromFile(fin);

            double det = A.determinant();    // calculate determinant

            cout << "Determinant = " << det << endl;
        }

        // GAUSSIAN ELIMINATION
        else if (choice == 7)
        {
            string leftFile, rightFile, augFile;

            cout << "Enter left matrix file: ";
            cin >> leftFile;

            cout << "Enter right matrix file: ";
            cin >> rightFile;

            cout << "Enter output augmented file name: ";
            cin >> augFile;

            // generate augmented matrix file
            AdvancedOperation::generateAugmentedMatrixFile(leftFile, rightFile, augFile);

            ifstream fin(augFile);

            int r, c;
            fin >> r >> c;

            BasicOperation A(r, c);          // create matrix
            A.readFromFile(fin);

            ofstream matrixOut("resultmatrix.txt"); // store reduced matrix
            ofstream vectorOut("resultvector.txt"); // store solution vector

            // perform gaussian elimination
            AdvancedOperation::gaussianEliminationWithPivoting(A, matrixOut, vectorOut);

            cout << "Result matrix saved in resultmatrix.txt\n";
            cout << "Result vector saved in resultvector.txt\n";
            cout << "Run: gnuplot plot.gnu\n";
        }

        else
        {
            cout << "Invalid choice\n";
        }
    }
    catch (exception &e)                    // catch runtime errors
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;                              
}