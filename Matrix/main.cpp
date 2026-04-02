#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sys/stat.h>

#include "Include/BasicOperation.hpp"
#include "Include/LinearOperation.hpp"
#include "Include/GaussianElimination.hpp"
#include "Include/LUDecomposition.hpp"
#include "Include/Iterativemethod.hpp"
#include "Include/Eigenvalue.hpp"

using namespace std;

int main()
{
    try
    {
        mkdir("Output", 0777);                   // create Output/ folder

        int choice;
        cout << "\n========== MATRIX OPERATIONS MENU ==========" << endl;
        cout << "1.  Matrix Addition"                             << endl;
        cout << "2.  Matrix Subtraction"                          << endl;
        cout << "3.  Matrix Multiplication"                       << endl;
        cout << "4.  Scalar Division"                             << endl;
        cout << "5.  Transpose"                                   << endl;
        cout << "6.  Determinant"                                 << endl;
        cout << "7.  Gaussian Elimination (WITH Pivoting)"        << endl;
        cout << "8.  Gaussian Elimination (WITHOUT Pivoting)"     << endl;
        cout << "9.  LU Decomposition (Doolittle)"                << endl;
        cout << "10. LU Decomposition (Crout)"                    << endl;
        cout << "11. Cholesky Decomposition"                      << endl;
        cout << "12. Matrix Properties & Transforms"              << endl;
        cout << "13. Iterative Method (Gauss-Jacobi)"             << endl;
        cout << "14. Iterative Method (Gauss-Seidel)"             << endl;
        cout << "15. EigenValue (Gerschgorin Circle Theorem)"     << endl;
        cout << "Enter your choice: " << flush;
        cin >> choice;

        // ── CHOICES 1–3: Two-matrix arithmetic ──────────────────
        if (choice >= 1 && choice <= 3)
        {
            string file1, file2; int r1,c1,r2,c2;
            cout << "Enter first matrix file: "  << flush; cin >> file1;
            cout << "Enter second matrix file: " << flush; cin >> file2;
            ifstream fin1(file1); ifstream fin2(file2);
            if (!fin1||!fin2) throw runtime_error("File not found");
            fin1>>r1>>c1; fin2>>r2>>c2;
            BasicOperation A(r1,c1); BasicOperation B(r2,c2);
            A.readFromFile(fin1); B.readFromFile(fin2);
            BasicOperation result;
            if      (choice==1){if(r1!=r2||c1!=c2)throw runtime_error("Mismatch");result=A.add(B);}
            else if (choice==2){if(r1!=r2||c1!=c2)throw runtime_error("Mismatch");result=A.subtract(B);}
            else if (choice==3){if(c1!=r2)throw runtime_error("Mismatch");result=A.multiply(B);}
            ofstream fout("Output/result.txt");
            fout<<result.getRows()<<" "<<result.getCols()<<endl;
            result.displayToFile(fout);
            cout << "[OUTPUT] Result -> Output/result.txt" << endl;
        }
        else if (choice == 4)
        {
            string file; double value;
            cout<<"Enter matrix file: "<<flush; cin>>file;
            cout<<"Enter scalar value: "<<flush; cin>>value;
            ifstream fin(file); int r,c; fin>>r>>c;
            BasicOperation A(r,c); A.readFromFile(fin);
            BasicOperation result = A.scalarDivide(value);
            ofstream fout("Output/result_scalar.txt");
            fout<<r<<" "<<c<<endl; result.displayToFile(fout);
            cout<<"[OUTPUT] Result -> Output/result_scalar.txt"<<endl;
        }
        else if (choice == 5)
        {
            string file;
            cout<<"Enter matrix file: "<<flush; cin>>file;
            ifstream fin(file); int r,c; fin>>r>>c;
            BasicOperation A(r,c); A.readFromFile(fin);
            BasicOperation result = A.transpose();
            ofstream fout("Output/result_transpose.txt");
            fout<<result.getRows()<<" "<<result.getCols()<<endl;
            result.displayToFile(fout);
            cout<<"[OUTPUT] Transpose -> Output/result_transpose.txt"<<endl;
        }
        else if (choice == 6)
        {
            string file;
            cout<<"Enter square matrix file: "<<flush; cin>>file;
            ifstream fin(file); int r,c; fin>>r>>c;
            if(r!=c) throw runtime_error("Matrix must be square");
            BasicOperation A(r,c); A.readFromFile(fin);
            double det = A.determinant();
            ofstream fout("Output/result_determinant.txt");
            fout<<"Determinant = "<<det<<endl;
            cout<<"Determinant = "<<det<<endl;
            cout<<"[OUTPUT] Result -> Output/result_determinant.txt"<<endl;
        }
        else if (choice == 7)
        {
            string leftFile, rightFile;
            cout<<"Enter coefficient matrix file: "<<flush; cin>>leftFile;
            cout<<"Enter constant vector file: "   <<flush; cin>>rightFile;
            string augFile = "Output/gaussian_pivot_augmented.txt";
            LinearOperation linearOperation;           // create helper object
            linearOperation.generateAugmentedMatrixFile(leftFile, rightFile, augFile);
            ifstream fin(augFile); int r,c; fin>>r>>c;
            Matrix aug(r,c); aug.readFromFile(fin);
            ofstream matrixOut("Output/gaussian_pivot_matrix.txt");
            ofstream vectorOut("Output/gaussian_pivot_vector.txt");
            GaussianElimination gaussianElimination;   // create solver object
            gaussianElimination.gaussianWithPivoting(aug, matrixOut, vectorOut);
            cout<<"[OUTPUT] Augmented matrix -> Output/gaussian_pivot_augmented.txt"<<endl;
            cout<<"[OUTPUT] Upper triangular -> Output/gaussian_pivot_matrix.txt"   <<endl;
            cout<<"[OUTPUT] Solution vector  -> Output/gaussian_pivot_vector.txt"   <<endl;
            cout<<"[OUTPUT] Graph            -> Output/gaussian_pivot_graph.png"    <<endl;
        }
        else if (choice == 8)
        {
            string leftFile, rightFile;
            cout<<"Enter coefficient matrix file: "<<flush; cin>>leftFile;
            cout<<"Enter constant vector file: "   <<flush; cin>>rightFile;
            string augFile = "Output/gaussian_nopivot_augmented.txt";
            LinearOperation linearOperation;           // create helper object
            linearOperation.generateAugmentedMatrixFile(leftFile, rightFile, augFile);
            ifstream fin(augFile); int r,c; fin>>r>>c;
            Matrix aug(r,c); aug.readFromFile(fin);
            ofstream matrixOut("Output/gaussian_nopivot_matrix.txt");
            ofstream vectorOut("Output/gaussian_nopivot_vector.txt");
            GaussianElimination gaussianElimination;   // create solver object
            gaussianElimination.gaussianWithoutPivoting(aug, matrixOut, vectorOut);
            cout<<"[OUTPUT] Augmented matrix -> Output/gaussian_nopivot_augmented.txt"<<endl;
            cout<<"[OUTPUT] Upper triangular -> Output/gaussian_nopivot_matrix.txt"   <<endl;
            cout<<"[OUTPUT] Solution vector  -> Output/gaussian_nopivot_vector.txt"   <<endl;
            cout<<"[OUTPUT] Graph            -> Output/gaussian_nopivot_graph.png"    <<endl;
        }
        else if (choice == 9)
        {
            string Afile, bfile;
            cout<<"Enter coefficient matrix file: "<<flush; cin>>Afile;
            cout<<"Enter constant vector file: "   <<flush; cin>>bfile;
            LUDecomposition luDecomposition;          // create solver object
            luDecomposition.doolittleLU(Afile, bfile);
            cout<<"[OUTPUT] Doolittle results saved in Output/"<<endl;
        }
        else if (choice == 10)
        {
            string Afile, bfile;
            cout<<"Enter coefficient matrix file: "<<flush; cin>>Afile;
            cout<<"Enter constant vector file: "   <<flush; cin>>bfile;
            LUDecomposition luDecomposition;          // create solver object
            luDecomposition.croutLU(Afile, bfile);
            cout<<"[OUTPUT] Crout results saved in Output/"<<endl;
        }
        else if (choice == 11)
        {
            string Afile, bfile;
            cout<<"Enter coefficient matrix file: "<<flush; cin>>Afile;
            cout<<"Enter constant vector file: "   <<flush; cin>>bfile;
            LUDecomposition luDecomposition;          // create solver object
            luDecomposition.choleskyDecomposition(Afile, bfile);
            cout<<"[OUTPUT] Cholesky results saved in Output/"<<endl;
        }
        else if (choice == 12)
        {
            string file;
            cout<<"Enter matrix file: "<<flush; cin>>file;
            ifstream fin(file);
            if (!fin) throw runtime_error("File not found");
            int r,c; fin>>r>>c;
            Matrix A(r,c); A.readFromFile(fin);
            ofstream fout("Output/properties.txt");
            fout<<"Matrix Properties & Transforms"<<endl;
            fout<<"=============================="<<endl<<endl;
            fout<<"Input Matrix ("<<r<<"x"<<c<<"):"<<endl;
            A.displayToFile(fout); fout<<endl;
            cout<<"\n--- Property Checks ---"<<endl;
            fout<<"--- Property Checks ---"<<endl;
            auto yn=[](bool v){return v?"true":"false";};
            cout<<"isSquare            : "<<yn(A.isSquare())            <<endl;
            cout<<"isSymmetric         : "<<yn(A.isSymmetric())         <<endl;
            cout<<"isIdentity          : "<<yn(A.isIdentity())          <<endl;
            cout<<"isNull              : "<<yn(A.isNull())              <<endl;
            cout<<"isDiagonal          : "<<yn(A.isDiagonal())          <<endl;
            cout<<"isDiagonallyDominant: "<<yn(A.isDiagonallyDominant())<<endl;
            fout<<"isSquare            : "<<yn(A.isSquare())            <<endl;
            fout<<"isSymmetric         : "<<yn(A.isSymmetric())         <<endl;
            fout<<"isIdentity          : "<<yn(A.isIdentity())          <<endl;
            fout<<"isNull              : "<<yn(A.isNull())              <<endl;
            fout<<"isDiagonal          : "<<yn(A.isDiagonal())          <<endl;
            fout<<"isDiagonallyDominant: "<<yn(A.isDiagonallyDominant())<<endl;
            fout<<endl;
            cout<<"\nEnter second matrix file to check isTranspose (or type skip): "<<flush;
            string file2; cin>>file2;
            if (file2!="skip") {
                ifstream fin2(file2);
                if (!fin2){cout<<"Second file not found."<<endl;}
                else{int r2,c2;fin2>>r2>>c2;Matrix B(r2,c2);B.readFromFile(fin2);bool iT=A.isTranspose(B);cout<<"isTranspose(B): "<<yn(iT)<<endl;fout<<"isTranspose(B): "<<yn(iT)<<endl;}
            }
            fout<<endl;
            Matrix T=A.transpose(); ofstream ft("Output/prop_transpose.txt");
            ft<<T.getRows()<<" "<<T.getCols()<<endl; T.displayToFile(ft);
            fout<<"--- Transpose ---"<<endl; T.displayToFile(fout); fout<<endl;
            if (A.isSquare()) {
                Matrix D=A; D.makeDiagonallyDominant();
                ofstream fd("Output/prop_diag_dominant.txt");
                fd<<D.getRows()<<" "<<D.getCols()<<endl; D.displayToFile(fd);
                fout<<"--- After makeDiagonallyDominant ---"<<endl; D.displayToFile(fout); fout<<endl;
                try { Matrix inv=A.inverse(); ofstream fi("Output/prop_inverse.txt"); fi<<inv.getRows()<<" "<<inv.getCols()<<endl; inv.displayToFile(fi); fout<<"--- Inverse ---"<<endl; inv.displayToFile(fout); fout<<endl; cout<<"[OUTPUT] Inverse -> Output/prop_inverse.txt"<<endl;
                } catch(exception &e){cout<<"Inverse error: "<<e.what()<<endl;}
            }
            cout<<"[OUTPUT] All properties -> Output/properties.txt"   <<endl;
            cout<<"[OUTPUT] Transpose      -> Output/prop_transpose.txt"<<endl;
            if (A.isSquare()) cout<<"[OUTPUT] DiagDominant -> Output/prop_diag_dominant.txt"<<endl;
        }
        else if (choice == 13)
        {
            string Afile, bfile; int maxIter; double tol;
            cout<<"Enter coefficient matrix file: "   <<flush; cin>>Afile;
            cout<<"Enter constant vector file: "       <<flush; cin>>bfile;
            cout<<"Enter max iterations (e.g. 1000): " <<flush; cin>>maxIter;
            cout<<"Enter tolerance (e.g. 1e-10): "     <<flush; cin>>tol;
            IterativeMethod iterativeMethod;          // create solver object
            iterativeMethod.gaussJacobi(Afile, bfile, maxIter, tol);
        }
        else if (choice == 14)
        {
            string Afile, bfile; int maxIter; double tol;
            cout<<"Enter coefficient matrix file: "   <<flush; cin>>Afile;
            cout<<"Enter constant vector file: "       <<flush; cin>>bfile;
            cout<<"Enter max iterations (e.g. 1000): " <<flush; cin>>maxIter;
            cout<<"Enter tolerance (e.g. 1e-10): "     <<flush; cin>>tol;
            IterativeMethod iterativeMethod;          // create solver object
            iterativeMethod.gaussSeidel(Afile, bfile, maxIter, tol);
        }

        // ── CHOICE 15: Gerschgorin Circle Theorem ────────────────
        else if (choice == 15)
        {
            string Afile;
            cout << "Enter square matrix file: " << flush; cin >> Afile;
            EigenValue eigenValue;                    // create analyzer object
            eigenValue.gerschgorin(Afile);
        }

        else { cout << "Invalid choice" << endl; }
    }
    catch (exception &e) { cout << "\nError: " << e.what() << endl; }
    return 0;
}