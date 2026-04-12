#include "../Include/MatrixUserWorkflow.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cerrno>
#include <sys/stat.h>

#include "../Include/BasicOperation.hpp"
#include "../Include/Eigenvalue.hpp"
#include "../Include/GaussianElimination.hpp"
#include "../Include/Iterativemethod.hpp"
#include "../Include/LUDecomposition.hpp"
#include "../Include/Lagrange.hpp"
#include "../Include/LinearOperation.hpp"
#include "../Include/Matrix.hpp"

using namespace std;

bool MatrixUserWorkflow::ensureOutputDirectory()
{
    if (mkdir("Output", 0777) == 0)
        return true;

    if (errno == EEXIST)
        return true;

    cout << "Output error: cannot create Output directory." << endl;
    return false;
}

bool MatrixUserWorkflow::loadBasic(const string &path, BasicOperation &out)
{
    string err;
    Matrix loaded;
    if (!loaded.isLoadFile(path, &err) || !loaded.isLoaded())
    {
        cout << "Input error: " << err << endl;
        return false;
    }

    out = BasicOperation(loaded.getRows(), loaded.getCols());
    for (int i = 0; i < loaded.getRows(); i++)
        for (int j = 0; j < loaded.getCols(); j++)
            out.set(i, j, loaded.get(i, j));
    return true;
}

bool MatrixUserWorkflow::loadMatrixData(const string &path, Matrix &out)
{
    string err;
    if (!out.isLoadFile(path, &err) || !out.isLoaded())
    {
        cout << "Input error: " << err << endl;
        return false;
    }
    return true;
}

int MatrixUserWorkflow::handleBasicOps(int choice)
{
    if (!ensureOutputDirectory())
        return 0;

    string file1, file2;
    cout << "Enter first matrix file: " << flush;
    cin >> file1;
    cout << "Enter second matrix file: " << flush;
    cin >> file2;

    BasicOperation A, B;
    if (!loadBasic(file1, A) || !loadBasic(file2, B))
        return 0;

    BasicOperation result;
    if (choice == 1)
    {
        if (A.getRows() != B.getRows() || A.getCols() != B.getCols())
        {
            cout << "Input error: addition needs same dimensions." << endl;
            return 0;
        }
        result = A.add(B);
    }
    else if (choice == 2)
    {
        if (A.getRows() != B.getRows() || A.getCols() != B.getCols())
        {
            cout << "Input error: subtraction needs same dimensions." << endl;
            return 0;
        }
        result = A.subtract(B);
    }
    else
    {
        if (A.getCols() != B.getRows())
        {
            cout << "Input error: multiplication needs cols(A) = rows(B)." << endl;
            return 0;
        }
        result = A.multiply(B);
    }

    ofstream fout("Output/result.txt");
    if (!fout)
    {
        cout << "Output error: cannot open Output/result.txt" << endl;
        return 0;
    }
    fout << result.getRows() << " " << result.getCols() << endl;
    result.displayToFile(fout);
    cout << "[OUTPUT] Result -> Output/result.txt" << endl;
    return 0;
}

int MatrixUserWorkflow::runMatrixAddition()
{
    return handleBasicOps(1);
}

int MatrixUserWorkflow::runMatrixSubtraction()
{
    return handleBasicOps(2);
}

int MatrixUserWorkflow::runMatrixMultiplication()
{
    return handleBasicOps(3);
}

int MatrixUserWorkflow::runScalarDivision()
{
    if (!ensureOutputDirectory())
        return 0;

    string file;
    double value = 0.0;
    cout << "Enter matrix file: " << flush;
    cin >> file;
    cout << "Enter scalar value: " << flush;
    if (!(cin >> value))
    {
        cout << "Invalid input: scalar value must be numeric." << endl;
        return 0;
    }

    BasicOperation A;
    if (!loadBasic(file, A))
        return 0;

    try
    {
        BasicOperation result = A.scalarDivide(value);
        ofstream fout("Output/result_scalar.txt");
        if (!fout)
        {
            cout << "Output error: cannot open Output/result_scalar.txt" << endl;
            return 0;
        }
        fout << A.getRows() << " " << A.getCols() << endl;
        result.displayToFile(fout);
        cout << "[OUTPUT] Result -> Output/result_scalar.txt" << endl;
    }
    catch (exception &e)
    {
        cout << "Operation error: " << e.what() << endl;
    }
    return 0;
}

int MatrixUserWorkflow::runTranspose()
{
    if (!ensureOutputDirectory())
        return 0;

    string file;
    cout << "Enter matrix file: " << flush;
    cin >> file;

    BasicOperation A;
    if (!loadBasic(file, A))
        return 0;

    BasicOperation result = A.transpose();
    ofstream fout("Output/result_transpose.txt");
    if (!fout)
    {
        cout << "Output error: cannot open Output/result_transpose.txt" << endl;
        return 0;
    }
    fout << result.getRows() << " " << result.getCols() << endl;
    result.displayToFile(fout);
    cout << "[OUTPUT] Transpose -> Output/result_transpose.txt" << endl;
    return 0;
}

int MatrixUserWorkflow::runDeterminant()
{
    if (!ensureOutputDirectory())
        return 0;

    string file;
    cout << "Enter square matrix file: " << flush;
    cin >> file;

    BasicOperation A;
    if (!loadBasic(file, A))
        return 0;
    if (A.getRows() != A.getCols())
    {
        cout << "Input error: matrix must be square." << endl;
        return 0;
    }

    double det = A.determinant();
    ofstream fout("Output/result_determinant.txt");
    if (!fout)
    {
        cout << "Output error: cannot open Output/result_determinant.txt" << endl;
        return 0;
    }
    fout << "Determinant = " << det << endl;
    cout << "Determinant = " << det << endl;
    cout << "[OUTPUT] Result -> Output/result_determinant.txt" << endl;
    return 0;
}

int MatrixUserWorkflow::runGaussianWithPivoting()
{
    return handleGaussian(true);
}

int MatrixUserWorkflow::runGaussianWithoutPivoting()
{
    return handleGaussian(false);
}

int MatrixUserWorkflow::handleGaussian(bool withPivoting)
{
    if (!ensureOutputDirectory())
        return 0;

    string leftFile, rightFile;
    cout << "Enter coefficient matrix file: " << flush;
    cin >> leftFile;
    cout << "Enter constant vector file: " << flush;
    cin >> rightFile;

    string augFile = withPivoting ? "Output/gaussian_pivot_augmented.txt" : "Output/gaussian_nopivot_augmented.txt";
    LinearOperation linearOperation;
    linearOperation.generateAugmentedMatrixFile(leftFile, rightFile, augFile);

    Matrix aug;
    if (!loadMatrixData(augFile, aug))
        return 0;

    ofstream matrixOut(withPivoting ? "Output/gaussian_pivot_matrix.txt" : "Output/gaussian_nopivot_matrix.txt");
    ofstream vectorOut(withPivoting ? "Output/gaussian_pivot_vector.txt" : "Output/gaussian_nopivot_vector.txt");
    if (!matrixOut || !vectorOut)
    {
        cout << "Output error: cannot open gaussian output files." << endl;
        return 0;
    }

    GaussianElimination gaussianElimination;
    if (withPivoting)
    {
        gaussianElimination.gaussianWithPivoting(aug, matrixOut, vectorOut);
        cout << "[OUTPUT] Augmented matrix -> Output/gaussian_pivot_augmented.txt" << endl;
        cout << "[OUTPUT] Upper triangular -> Output/gaussian_pivot_matrix.txt" << endl;
        cout << "[OUTPUT] Solution vector  -> Output/gaussian_pivot_vector.txt" << endl;
        cout << "[OUTPUT] Graph            -> Output/gaussian_pivot_graph.png" << endl;
    }
    else
    {
        gaussianElimination.gaussianWithoutPivoting(aug, matrixOut, vectorOut);
        cout << "[OUTPUT] Augmented matrix -> Output/gaussian_nopivot_augmented.txt" << endl;
        cout << "[OUTPUT] Upper triangular -> Output/gaussian_nopivot_matrix.txt" << endl;
        cout << "[OUTPUT] Solution vector  -> Output/gaussian_nopivot_vector.txt" << endl;
        cout << "[OUTPUT] Graph            -> Output/gaussian_nopivot_graph.png" << endl;
    }

    return 0;
}

int MatrixUserWorkflow::runDoolittle()
{
    if (!ensureOutputDirectory())
        return 0;

    string Afile, bfile;
    cout << "Enter coefficient matrix file: " << flush;
    cin >> Afile;
    cout << "Enter constant vector file: " << flush;
    cin >> bfile;

    LUDecomposition luDecomposition;
    luDecomposition.doolittleLU(Afile, bfile);
    cout << "[OUTPUT] Doolittle results saved in Output/" << endl;
    return 0;
}

int MatrixUserWorkflow::runCrout()
{
    if (!ensureOutputDirectory())
        return 0;

    string Afile, bfile;
    cout << "Enter coefficient matrix file: " << flush;
    cin >> Afile;
    cout << "Enter constant vector file: " << flush;
    cin >> bfile;

    LUDecomposition luDecomposition;
    luDecomposition.croutLU(Afile, bfile);
    cout << "[OUTPUT] Crout results saved in Output/" << endl;
    return 0;
}

int MatrixUserWorkflow::runCholesky()
{
    if (!ensureOutputDirectory())
        return 0;

    string Afile, bfile;
    cout << "Enter coefficient matrix file: " << flush;
    cin >> Afile;
    cout << "Enter constant vector file: " << flush;
    cin >> bfile;

    LUDecomposition luDecomposition;
    luDecomposition.choleskyDecomposition(Afile, bfile);
    cout << "[OUTPUT] Cholesky results saved in Output/" << endl;
    return 0;
}

int MatrixUserWorkflow::runProperties()
{
    if (!ensureOutputDirectory())
        return 0;

    string file;
    cout << "Enter matrix file: " << flush;
    cin >> file;

    Matrix A;
    if (!loadMatrixData(file, A))
        return 0;

    ofstream fout("Output/properties.txt");
    if (!fout)
    {
        cout << "Output error: cannot open Output/properties.txt" << endl;
        return 0;
    }

    fout << "Matrix Properties & Transforms" << endl;
    fout << "==============================" << endl << endl;
    fout << "Input Matrix (" << A.getRows() << "x" << A.getCols() << "):" << endl;
    A.displayToFile(fout);
    fout << endl;

    cout << "\n--- Property Checks ---" << endl;
    fout << "--- Property Checks ---" << endl;
    auto yn = [](bool v) { return v ? "true" : "false"; };
    cout << "isSquare            : " << yn(A.isSquare()) << endl;
    cout << "isSymmetric         : " << yn(A.isSymmetric()) << endl;
    cout << "isIdentity          : " << yn(A.isIdentity()) << endl;
    cout << "isNull              : " << yn(A.isNull()) << endl;
    cout << "isDiagonal          : " << yn(A.isDiagonal()) << endl;
    cout << "isDiagonallyDominant: " << yn(A.isDiagonallyDominant()) << endl;
    fout << "isSquare            : " << yn(A.isSquare()) << endl;
    fout << "isSymmetric         : " << yn(A.isSymmetric()) << endl;
    fout << "isIdentity          : " << yn(A.isIdentity()) << endl;
    fout << "isNull              : " << yn(A.isNull()) << endl;
    fout << "isDiagonal          : " << yn(A.isDiagonal()) << endl;
    fout << "isDiagonallyDominant: " << yn(A.isDiagonallyDominant()) << endl;
    fout << endl;

    cout << "\nEnter second matrix file to check isTranspose (or type skip): " << flush;
    string file2;
    cin >> file2;
    if (file2 != "skip")
    {
        Matrix B;
        if (!loadMatrixData(file2, B))
        {
            cout << "Input error: second matrix cannot be loaded." << endl;
        }
        else
        {
            bool iT = A.isTranspose(B);
            cout << "isTranspose(B): " << yn(iT) << endl;
            fout << "isTranspose(B): " << yn(iT) << endl;
        }
    }

    fout << endl;
    Matrix T = A.transpose();
    ofstream ft("Output/prop_transpose.txt");
    if (ft)
    {
        ft << T.getRows() << " " << T.getCols() << endl;
        T.displayToFile(ft);
    }
    fout << "--- Transpose ---" << endl;
    T.displayToFile(fout);
    fout << endl;

    if (A.isSquare())
    {
        Matrix D = A;
        D.makeDiagonallyDominant();
        ofstream fd("Output/prop_diag_dominant.txt");
        if (fd)
        {
            fd << D.getRows() << " " << D.getCols() << endl;
            D.displayToFile(fd);
        }
        fout << "--- After makeDiagonallyDominant ---" << endl;
        D.displayToFile(fout);
        fout << endl;

        try
        {
            Matrix inv = A.inverse();
            ofstream fi("Output/prop_inverse.txt");
            if (fi)
            {
                fi << inv.getRows() << " " << inv.getCols() << endl;
                inv.displayToFile(fi);
            }
            fout << "--- Inverse ---" << endl;
            inv.displayToFile(fout);
            fout << endl;
            cout << "[OUTPUT] Inverse -> Output/prop_inverse.txt" << endl;
        }
        catch (exception &e)
        {
            cout << "Inverse error: " << e.what() << endl;
        }
    }

    cout << "[OUTPUT] All properties -> Output/properties.txt" << endl;
    cout << "[OUTPUT] Transpose      -> Output/prop_transpose.txt" << endl;
    if (A.isSquare())
        cout << "[OUTPUT] DiagDominant   -> Output/prop_diag_dominant.txt" << endl;

    return 0;
}

int MatrixUserWorkflow::runGaussJacobi()
{
    return handleIterative(true);
}

int MatrixUserWorkflow::runGaussSeidel()
{
    return handleIterative(false);
}

int MatrixUserWorkflow::handleIterative(bool jacobi)
{
    string Afile, bfile;
    int maxIter = 0;
    double tol = 0.0;
    cout << "Enter coefficient matrix file: " << flush;
    cin >> Afile;
    cout << "Enter constant vector file: " << flush;
    cin >> bfile;
    cout << "Enter max iterations (e.g. 1000): " << flush;
    if (!(cin >> maxIter))
    {
        cout << "Invalid input: max iterations must be integer." << endl;
        return 0;
    }
    cout << "Enter tolerance (e.g. 1e-10): " << flush;
    if (!(cin >> tol))
    {
        cout << "Invalid input: tolerance must be numeric." << endl;
        return 0;
    }

    IterativeMethod iterativeMethod;
    if (jacobi)
        iterativeMethod.gaussJacobi(Afile, bfile, maxIter, tol);
    else
        iterativeMethod.gaussSeidel(Afile, bfile, maxIter, tol);

    return 0;
}

int MatrixUserWorkflow::runEigenvalue()
{
    string Afile;
    cout << "Enter square matrix file: " << flush;
    cin >> Afile;

    EigenValue eigenValue;
    eigenValue.gerschgorin(Afile);
    return 0;
}

int MatrixUserWorkflow::runLagrange()
{
    if (!ensureOutputDirectory())
        return 0;

    string pointsFile;
    cout << "Enter interpolation points file: " << flush;
    cin >> pointsFile;

    ifstream pointIn(pointsFile);
    if (!pointIn)
    {
        cout << "Input error: cannot open interpolation points file." << endl;
        return 0;
    }

    double xQuery = 0.0;
    cout << "Enter query x-value : " << flush;
    if (!(cin >> xQuery))
    {
        cout << "Invalid input: query x-value must be numeric." << endl;
        return 0;
    }

    ofstream fout("Output/lagrange_interpolation.txt");
    if (!fout)
    {
        cout << "Output error: cannot open Output/lagrange_interpolation.txt" << endl;
        return 0;
    }

    try
    {
        Lagrange lagrange;
        lagrange.loadDataPoints(pointIn);
        double result = lagrange.interpolate(xQuery, fout);
        cout << "Interpolated value P(" << xQuery << ") = " << result << endl;
        cout << "[OUTPUT] Detailed steps -> Output/lagrange_interpolation.txt" << endl;
    }
    catch (exception &e)
    {
        cout << "Interpolation error: " << e.what() << endl;
    }

    return 0;
}

