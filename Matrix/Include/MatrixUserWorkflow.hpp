#ifndef MATRIX_USER_WORKFLOW_HPP
#define MATRIX_USER_WORKFLOW_HPP

#include <string>

class MatrixUserWorkflow
{
public:
    int runMatrixAddition();
    int runMatrixSubtraction();
    int runMatrixMultiplication();
    int runScalarDivision();
    int runTranspose();
    int runDeterminant();
    int runGaussianWithPivoting();
    int runGaussianWithoutPivoting();
    int runDoolittle();
    int runCrout();
    int runCholesky();
    int runProperties();
    int runGaussJacobi();
    int runGaussSeidel();
    int runEigenvalue();
    int runLagrange();
    int runLeastSquare();

private:
    bool ensureOutputDirectory();

    bool loadBasic(const std::string &path, class BasicOperation &out);
    bool loadMatrixData(const std::string &path, class Matrix &out);

    int handleBasicOps(int choice);
    int handleGaussian(bool withPivoting);
    int handleIterative(bool jacobi);
};

#endif
