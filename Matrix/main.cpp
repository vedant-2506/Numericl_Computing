#include "Include/Gaussian.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main()
{
    try
    {
        // Create output folder if not exists
        fs::create_directory("output");

        string fileA, fileB;

        cout << "Enter first input file (Augmented matrix for Gaussian): ";
        cin >> fileA;

        cout << "Enter second input file (for Addition/Subtraction): ";
        cin >> fileB;

        ifstream finA(fileA);
        ifstream finB(fileB);

        if (!finA || !finB)
            throw runtime_error("File opening error.");

        int r1, c1, r2, c2;

        // Reading Matrix A
        finA >> r1 >> c1;
        Matrix A(r1, c1);
        A.readFromFile(finA);

        // Reading Matrix B
        finB >> r2 >> c2;
        Matrix B(r2, c2);
        B.readFromFile(finB);

        finA.close();
        finB.close();

        // ============================
        // ADDITION & SUBTRACTION
        // ============================

        if (r1 == r2 && c1 == c2)
        {
            Matrix C = A.add(B);
            Matrix D = A.subtract(B);

            ofstream addOut("output/addition_output.txt");
            C.displayToFile(addOut);
            addOut.close();

            ofstream subOut("output/subtraction_output.txt");
            D.displayToFile(subOut);
            subOut.close();
        }
        else
        {
            cout << "Addition/Subtraction not possible (size mismatch)\n";
        }

        // ============================
        // GAUSSIAN ELIMINATION
        // ============================

        // Only perform Gaussian elimination if matrix A is augmented (n x n+1)
        if (c1 == r1 + 1)
        {
            ofstream gaussMatrixOut("output/gaussian_matrix_output.txt");
            ofstream gaussSolOut("output/gaussian_solution_output.txt");

            A.gaussianEliminationWithPivoting(gaussMatrixOut, gaussSolOut);

            gaussMatrixOut.close();
            gaussSolOut.close();

            cout << "Gaussian elimination completed.\n";
        }
        else
        {
            cout << "Skipping Gaussian elimination (not an augmented matrix).\n";
        }

        cout << "All output files saved inside 'output' folder.\n";
    }
    catch (exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}