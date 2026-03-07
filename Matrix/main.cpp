#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Include/BasicOperation.hpp"
#include "Include/LinearOperation.hpp"
#include "Include/GaussianElimination.hpp"
#include "Include/LUDecomposition.hpp"

using namespace std;

int main()
{
    try
    {
        int choice;

        cout << "\n========== MATRIX OPERATIONS MENU ==========\n";
        cout << "1. Matrix Addition\n";
        cout << "2. Matrix Subtraction\n";
        cout << "3. Matrix Multiplication\n";
        cout << "4. Scalar Division\n";
        cout << "5. Transpose\n";
        cout << "6. Determinant\n";
        cout << "7. Gaussian Elimination\n";
        cout << "8. LU Decomposition (Doolittle)\n";
        cout << "9. LU Decomposition (Crout)\n";
        cout << "10. Cholesky Decomposition\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // ================= BASIC OPERATIONS =================

        if (choice >= 1 && choice <= 5)
        {
            string file1, file2;
            int r1, c1, r2, c2;

            if (choice <= 3)
            {
                cout << "Enter first matrix file: ";
                cin >> file1;

                cout << "Enter second matrix file: ";
                cin >> file2;

                ifstream fin1(file1);
                ifstream fin2(file2);

                if (!fin1 || !fin2)
                    throw runtime_error("File not found");

                fin1 >> r1 >> c1;
                fin2 >> r2 >> c2;

                BasicOperation A(r1, c1);
                BasicOperation B(r2, c2);

                A.readFromFile(fin1);
                B.readFromFile(fin2);

                BasicOperation result;

                if (choice == 1)
                {
                    if (r1 != r2 || c1 != c2)
                        throw runtime_error("Addition dimension mismatch");

                    result = A.add(B);
                }

                else if (choice == 2)
                {
                    if (r1 != r2 || c1 != c2)
                        throw runtime_error("Subtraction dimension mismatch");

                    result = A.subtract(B);
                }

                else if (choice == 3)
                {
                    if (c1 != r2)
                        throw runtime_error("Multiplication dimension mismatch");

                    result = A.multiply(B);
                }

                ofstream fout("result.txt");

                fout << result.getRows() << " " << result.getCols() << endl;
                result.displayToFile(fout);

                cout << "\nResult saved in result.txt\n";
            }

            // SCALAR DIVISION
            else if (choice == 4)
            {
                string file;
                double value;

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

                cout << "\nResult saved in result.txt\n";
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

                fout << result.getRows() << " " << result.getCols() << endl;
                result.displayToFile(fout);

                cout << "\nTranspose saved in result.txt\n";
            }
        }

        // ================= DETERMINANT =================

        else if (choice == 6)
        {
            string file;

            cout << "Enter square matrix file: ";
            cin >> file;

            ifstream fin(file);

            int r, c;
            fin >> r >> c;

            if (r != c)
                throw runtime_error("Matrix must be square");

            BasicOperation A(r, c);
            A.readFromFile(fin);

            double det = A.determinant();

            ofstream fout("determinant.txt");

            fout << "Determinant = " << det << endl;

            cout << "\nDeterminant saved in determinant.txt\n";
        }

        // ================= GAUSSIAN =================

        else if (choice == 7)
        {
            string leftFile, rightFile, augFile;

            cout << "Enter coefficient matrix file: ";
            cin >> leftFile;

            cout << "Enter constant vector file: ";
            cin >> rightFile;

            augFile = "augmented.txt";

            LinearOperation::generateAugmentedMatrixFile(leftFile, rightFile, augFile);

            ifstream fin(augFile);

            int r, c;
            fin >> r >> c;

            Matrix aug(r, c);
            aug.readFromFile(fin);

            ofstream matrixOut("gaussian_steps.txt");
            ofstream vectorOut("gaussian_vector.txt");

            GaussianElimination::gaussianEliminationWithPivoting(aug, matrixOut, vectorOut);

            cout << "\nGaussian elimination completed\n";
            cout << "Steps -> gaussian_steps.txt\n";
            cout << "Vector -> gaussian_vector.txt\n";
        }

        // ================= DOOLITTLE =================

        else if (choice == 8)
        {
            string Afile, bfile;

            cout << "Enter coefficient matrix file: ";
            cin >> Afile;

            cout << "Enter constant vector file: ";
            cin >> bfile;

            LUDecomposition::doolittleLU(Afile, bfile);

            cout << "\nDoolittle solution saved\n";
        }

        // ================= CROUT =================

        else if (choice == 9)
        {
            string Afile, bfile;

            cout << "Enter coefficient matrix file: ";
            cin >> Afile;

            cout << "Enter constant vector file: ";
            cin >> bfile;

            LUDecomposition::croutLU(Afile, bfile);

            cout << "\nCrout solution saved\n";
        }

        // ================= CHOLESKY =================

        else if (choice == 10)
        {
            string Afile, bfile;

            cout << "Enter coefficient matrix file: ";
            cin >> Afile;

            cout << "Enter constant vector file: ";
            cin >> bfile;

            LUDecomposition::choleskyDecomposition(Afile, bfile);

            cout << "\nCholesky solution saved\n";
        }

        else
        {
            cout << "Invalid choice\n";
        }
    }

    catch (exception &e)
    {
        cout << "\nError: " << e.what() << endl;
    }

    return 0;
}