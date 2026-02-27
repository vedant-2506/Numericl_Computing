#include "Include/Gaussian.hpp"

using namespace std;

int main()
{
    try
    {
        int choice;

        cout << "\n1. Matrix Addition\n";
        cout << "2. Matrix Subtraction\n";
        cout << "3. Gaussian Elimination\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // ADDITION OR SUBTRACTION
        if (choice == 1 || choice == 2)
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
                throw runtime_error("Matrix dimensions must match");

            Matrix A(r1, c1);
            Matrix B(r2, c2);

            A.readFromFile(fin1);
            B.readFromFile(fin2);

            Matrix result;

            if (choice == 1)
                result = A.add(B);
            else
                result = A.subtract(B);

            ofstream fout("result.txt");
            fout << r1 << " " << c1 << endl;
            result.displayToFile(fout);

            cout << "\nResult saved in result.txt\n";
        }

        // GAUSSIAN ELIMINATION
        else if (choice == 3)
        {
            string leftFile, rightFile, augFile;

            cout << "Enter left matrix file: ";
            cin >> leftFile;

            cout << "Enter right matrix file: ";
            cin >> rightFile;

            cout << "Enter output augmented file: ";
            cin >> augFile;

            Matrix::generateAugmentedMatrixFile(leftFile, rightFile, augFile);

            ifstream fin(augFile);

            int r, c;
            fin >> r >> c;

            Matrix A(r, c);
            A.readFromFile(fin);
            fin.close();

            ofstream matrixOut("resultmatrix.txt");
            ofstream vectorOut("resultvector.txt");

            A.gaussianEliminationWithPivoting(matrixOut, vectorOut);

            cout << "\nMatrix saved in resultmatrix.txt\n";
            cout << "Vector saved in resultvector.txt\n";
            cout << "Run: gnuplot plot.gnu\n";
        }

        else
        {
            cout << "Invalid choice\n";
        }
    }
    catch (exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}