#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sys/stat.h>

#include "Include/BasicOperation.hpp"
#include "Include/LinearOperation.hpp"
#include "Include/GaussianElimination.hpp"
#include "Include/LUDecomposition.hpp"

using namespace std;

int main()
{
    try
    {
        mkdir("Output", 0777);

        int choice;

        cout << "\n========== MATRIX OPERATIONS MENU ==========" << endl;
        cout << "1.  Matrix Addition" << endl;
        cout << "2.  Matrix Subtraction" << endl;
        cout << "3.  Matrix Multiplication" << endl;
        cout << "4.  Scalar Division" << endl;
        cout << "5.  Transpose" << endl;
        cout << "6.  Determinant" << endl;
        cout << "7.  Gaussian Elimination (WITH Pivoting)" << endl;
        cout << "8.  Gaussian Elimination (WITHOUT Pivoting)" << endl;
        cout << "9.  LU Decomposition (Doolittle)" << endl;
        cout << "10. LU Decomposition (Crout)" << endl;
        cout << "11. Cholesky Decomposition" << endl;
        cout << "12. Matrix Properties & Transforms" << endl;
        cout << "Enter your choice: " << flush;
        cin >> choice;

        if (choice >= 1 && choice <= 3)
        {
            string file1, file2;
            int r1, c1, r2, c2;
            cout << "Enter first matrix file: " << flush;  cin >> file1;
            cout << "Enter second matrix file: " << flush; cin >> file2;
            ifstream fin1(file1); ifstream fin2(file2);
            if (!fin1 || !fin2) throw runtime_error("File not found");
            fin1 >> r1 >> c1; fin2 >> r2 >> c2;
            BasicOperation A(r1,c1); BasicOperation B(r2,c2);
            A.readFromFile(fin1); B.readFromFile(fin2);
            BasicOperation result;
            if (choice == 1) { if(r1!=r2||c1!=c2) throw runtime_error("Mismatch"); result=A.add(B); }
            else if (choice == 2) { if(r1!=r2||c1!=c2) throw runtime_error("Mismatch"); result=A.subtract(B); }
            else if (choice == 3) { if(c1!=r2) throw runtime_error("Mismatch"); result=A.multiply(B); }
            ofstream fout("Output/result.txt");
            fout << result.getRows() << " " << result.getCols() << endl;
            result.displayToFile(fout);
            cout << "[OUTPUT] Result -> Output/result.txt" << endl;
        }

        else if (choice == 4)
        {
            string file; double value;
            cout << "Enter matrix file: " << flush; cin >> file;
            cout << "Enter scalar value: " << flush; cin >> value;
            ifstream fin(file); int r,c; fin>>r>>c;
            BasicOperation A(r,c); A.readFromFile(fin);
            BasicOperation result = A.scalarDivide(value);
            ofstream fout("Output/result_scalar.txt");
            fout << r << " " << c << endl; result.displayToFile(fout);
            cout << "[OUTPUT] Result -> Output/result_scalar.txt" << endl;
        }

        else if (choice == 5)
        {
            string file;
            cout << "Enter matrix file: " << flush; cin >> file;
            ifstream fin(file); int r,c; fin>>r>>c;
            BasicOperation A(r,c); A.readFromFile(fin);
            BasicOperation result = A.transpose();
            ofstream fout("Output/result_transpose.txt");
            fout << result.getRows() << " " << result.getCols() << endl;
            result.displayToFile(fout);
            cout << "[OUTPUT] Transpose -> Output/result_transpose.txt" << endl;
        }

        else if (choice == 6)
        {
            string file;
            cout << "Enter square matrix file: " << flush; cin >> file;
            ifstream fin(file); int r,c; fin>>r>>c;
            if(r!=c) throw runtime_error("Matrix must be square");
            BasicOperation A(r,c); A.readFromFile(fin);
            double det = A.determinant();
            ofstream fout("Output/result_determinant.txt");
            fout << "Determinant = " << det << endl;
            cout << "Determinant = " << det << endl;
            cout << "[OUTPUT] Result -> Output/result_determinant.txt" << endl;
        }

        // ===== GAUSSIAN WITH PIVOTING (7) =====
        else if (choice == 7)
        {
            string leftFile, rightFile;
            cout << "Enter coefficient matrix file: " << flush; cin >> leftFile;
            cout << "Enter constant vector file: " << flush;    cin >> rightFile;

            string augFile = "Output/gaussian_pivot_augmented.txt";
            LinearOperation::generateAugmentedMatrixFile(leftFile, rightFile, augFile);

            ifstream fin(augFile); int r,c; fin>>r>>c;
            Matrix aug(r,c); aug.readFromFile(fin);

            ofstream matrixOut("Output/gaussian_pivot_matrix.txt");
            ofstream vectorOut("Output/gaussian_pivot_vector.txt");
            GaussianElimination::gaussianWithPivoting(aug, matrixOut, vectorOut);

            cout << "[OUTPUT] Augmented matrix -> Output/gaussian_pivot_augmented.txt" << endl;
            cout << "[OUTPUT] Upper triangular -> Output/gaussian_pivot_matrix.txt"    << endl;
            cout << "[OUTPUT] Solution vector  -> Output/gaussian_pivot_vector.txt"    << endl;
            cout << "[OUTPUT] Graph            -> Output/gaussian_pivot_graph.png"     << endl;
        }

        // ===== GAUSSIAN WITHOUT PIVOTING (8) =====
        else if (choice == 8)
        {
            string leftFile, rightFile;
            cout << "Enter coefficient matrix file: " << flush; cin >> leftFile;
            cout << "Enter constant vector file: " << flush;    cin >> rightFile;

            string augFile = "Output/gaussian_nopivot_augmented.txt";
            LinearOperation::generateAugmentedMatrixFile(leftFile, rightFile, augFile);

            ifstream fin(augFile); int r,c; fin>>r>>c;
            Matrix aug(r,c); aug.readFromFile(fin);

            ofstream matrixOut("Output/gaussian_nopivot_matrix.txt");
            ofstream vectorOut("Output/gaussian_nopivot_vector.txt");
            GaussianElimination::gaussianWithoutPivoting(aug, matrixOut, vectorOut);

            cout << "[OUTPUT] Augmented matrix -> Output/gaussian_nopivot_augmented.txt" << endl;
            cout << "[OUTPUT] Upper triangular -> Output/gaussian_nopivot_matrix.txt"    << endl;
            cout << "[OUTPUT] Solution vector  -> Output/gaussian_nopivot_vector.txt"    << endl;
            cout << "[OUTPUT] Graph            -> Output/gaussian_nopivot_graph.png"     << endl;
        }

        else if (choice == 9)
        {
            string Afile, bfile;
            cout << "Enter coefficient matrix file: " << flush; cin >> Afile;
            cout << "Enter constant vector file: " << flush;    cin >> bfile;
            LUDecomposition::doolittleLU(Afile, bfile);
            cout << "[OUTPUT] Doolittle results saved in Output/" << endl;
        }

        else if (choice == 10)
        {
            string Afile, bfile;
            cout << "Enter coefficient matrix file: " << flush; cin >> Afile;
            cout << "Enter constant vector file: " << flush;    cin >> bfile;
            LUDecomposition::croutLU(Afile, bfile);
            cout << "[OUTPUT] Crout results saved in Output/" << endl;
        }

        else if (choice == 11)
        {
            string Afile, bfile;
            cout << "Enter coefficient matrix file: " << flush; cin >> Afile;
            cout << "Enter constant vector file: " << flush;    cin >> bfile;
            LUDecomposition::choleskyDecomposition(Afile, bfile);
            cout << "[OUTPUT] Cholesky results saved in Output/" << endl;
        }

        // ===== MATRIX PROPERTIES (12) =====
        else if (choice == 12)
        {
            string file;
            cout << "Enter matrix file: " << flush; cin >> file;
            ifstream fin(file);
            if (!fin) throw runtime_error("File not found");
            int r,c; fin>>r>>c;
            Matrix A(r,c); A.readFromFile(fin);

            ofstream fout("Output/properties.txt");
            fout << "Matrix Properties & Transforms" << endl;
            fout << "==============================" << endl << endl;
            fout << "Input Matrix (" << r << "x" << c << "):" << endl;
            A.displayToFile(fout); fout << endl;

            cout << "\n--- Property Checks ---" << endl;
            fout << "--- Property Checks ---" << endl;
            cout << "isSquare            : " << (A.isSquare()            ?"true":"false") << endl;
            cout << "isSymmetric         : " << (A.isSymmetric()         ?"true":"false") << endl;
            cout << "isIdentity          : " << (A.isIdentity()          ?"true":"false") << endl;
            cout << "isNull              : " << (A.isNull()              ?"true":"false") << endl;
            cout << "isDiagonal          : " << (A.isDiagonal()          ?"true":"false") << endl;
            cout << "isDiagonallyDominant: " << (A.isDiagonallyDominant()?"true":"false") << endl;
            fout << "isSquare            : " << (A.isSquare()            ?"true":"false") << endl;
            fout << "isSymmetric         : " << (A.isSymmetric()         ?"true":"false") << endl;
            fout << "isIdentity          : " << (A.isIdentity()          ?"true":"false") << endl;
            fout << "isNull              : " << (A.isNull()              ?"true":"false") << endl;
            fout << "isDiagonal          : " << (A.isDiagonal()          ?"true":"false") << endl;
            fout << "isDiagonallyDominant: " << (A.isDiagonallyDominant()?"true":"false") << endl;
            fout << endl;

            cout << "\nEnter second matrix file to check isTranspose (or type skip): " << flush;
            string file2; cin >> file2;
            if (file2 != "skip")
            {
                ifstream fin2(file2);
                if (!fin2) { cout<<"Second file not found, skipping."<<endl; fout<<"isTranspose: not found"<<endl; }
                else
                {
                    int r2,c2; fin2>>r2>>c2; Matrix B(r2,c2); B.readFromFile(fin2);
                    bool isTr = A.isTranspose(B);
                    cout << "isTranspose(B)      : " << (isTr?"true":"false") << endl;
                    fout << "isTranspose(B)      : " << (isTr?"true":"false") << endl;
                }
            }
            else { fout << "isTranspose: skipped" << endl; }
            fout << endl;

            Matrix T = A.transpose();
            ofstream ftrans("Output/prop_transpose.txt");
            ftrans << T.getRows() << " " << T.getCols() << endl;
            T.displayToFile(ftrans);
            fout << "--- Transpose ---" << endl; T.displayToFile(fout); fout << endl;

            if (A.isSquare())
            {
                Matrix D = A; D.makeDiagonallyDominant();
                ofstream fdiag("Output/prop_diag_dominant.txt");
                fdiag << D.getRows() << " " << D.getCols() << endl; D.displayToFile(fdiag);
                fout << "--- After makeDiagonallyDominant ---" << endl; D.displayToFile(fout); fout << endl;
            }

            if (A.isSquare())
            {
                try
                {
                    Matrix inv = A.inverse();
                    ofstream finv("Output/prop_inverse.txt");
                    finv << inv.getRows() << " " << inv.getCols() << endl; inv.displayToFile(finv);
                    fout << "--- Inverse ---" << endl; inv.displayToFile(fout); fout << endl;
                    cout << "[OUTPUT] Inverse          -> Output/prop_inverse.txt" << endl;
                }
                catch (exception &e) { cout<<"Inverse error: "<<e.what()<<endl; fout<<"Inverse error: "<<e.what()<<endl; }
            }
            else { cout<<"Inverse: not applicable (not square)"<<endl; }

            cout << "[OUTPUT] All properties   -> Output/properties.txt"      << endl;
            cout << "[OUTPUT] Transpose        -> Output/prop_transpose.txt"   << endl;
            if (A.isSquare())
                cout << "[OUTPUT] DiagDominant -> Output/prop_diag_dominant.txt" << endl;
        }

        else { cout << "Invalid choice" << endl; }
    }
    catch (exception &e)
    {
        cout << "\nError: " << e.what() << endl;
    }

    return 0;
}