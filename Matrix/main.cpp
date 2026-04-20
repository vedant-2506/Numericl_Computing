#include <iostream>

#include "Include/MatrixUserWorkflow.hpp"

using namespace std;

int main()
{
    MatrixUserWorkflow workflow;

    int choice = 0;
    cout << "\n----------- MATRIX OPERATIONS MENU -----------" << endl;
    cout << "\n----------- Choose your operation-----------" << endl;
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
    cout << "13. Iterative Method (Gauss-Jacobi)" << endl;
    cout << "14. Iterative Method (Gauss-Seidel)" << endl;
    cout << "15. EigenValue (Gerschgorin Circle Theorem)" << endl;
    cout << "16. Interpolation (Lagrange Method)" << endl;
    cout << "17. Curve Fitting (Least Squares Method)" << endl;
    cout << "Enter your choice: " << flush;

    if (!(cin >> choice))
    {
        cout << "Invalid input. Please enter a numeric menu choice." << endl;
        return 0;
    }

    switch (choice)
    {
        case 1: return workflow.runMatrixAddition();
        case 2: return workflow.runMatrixSubtraction();
        case 3: return workflow.runMatrixMultiplication();
        case 4: return workflow.runScalarDivision();
        case 5: return workflow.runTranspose();
        case 6: return workflow.runDeterminant();
        case 7: return workflow.runGaussianWithPivoting();
        case 8: return workflow.runGaussianWithoutPivoting();
        case 9: return workflow.runDoolittle();
        case 10: return workflow.runCrout();
        case 11: return workflow.runCholesky();
        case 12: return workflow.runProperties();
        case 13: return workflow.runGaussJacobi();
        case 14: return workflow.runGaussSeidel();
        case 15: return workflow.runEigenvalue();
        case 16: return workflow.runLagrange();
        case 17: return workflow.runLeastSquare();
        default:
            cout << "Invalid choice" << endl;
            return 0;
    }
}