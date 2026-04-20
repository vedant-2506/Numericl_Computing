#include "../Include/LeastSquare.hpp"
#include <cmath>
#include <iomanip>
#include <stdexcept>

using namespace std;

LeastSquare::LeastSquare() : CurveFitting() {}

LeastSquare::LeastSquare(const Matrix &m) : CurveFitting(m) {}

vector<double> LeastSquare::solveNormalEquation(Matrix augmented) const
{
    int n = augmented.getRows();
    auto &A = augmented.getData();

    for (int k = 0; k < n - 1; k++)
    {
        int pivot = k;
        for (int i = k + 1; i < n; i++)
            if (fabs(A[i][k]) > fabs(A[pivot][k]))
                pivot = i;

        if (pivot != k)
            swap(A[k], A[pivot]);

        if (fabs(A[k][k]) < 1e-12)
            throw runtime_error("LeastSquare: singular normal equation (invalid degree or duplicate data)."
            );

        for (int i = k + 1; i < n; i++)
        {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j <= n; j++)
                A[i][j] -= factor * A[k][j];
        }
    }

    if (fabs(A[n - 1][n - 1]) < 1e-12)
        throw runtime_error("LeastSquare: singular normal equation at back substitution.");

    vector<double> coefficients(n, 0.0);
    for (int i = n - 1; i >= 0; i--)
    {
        coefficients[i] = A[i][n];
        for (int j = i + 1; j < n; j++)
            coefficients[i] -= A[i][j] * coefficients[j];
        coefficients[i] /= A[i][i];
    }

    return coefficients;
}

vector<double> LeastSquare::fit(int degree, ofstream &fout)
{
    validateSamplePoints();

    if (degree < 0)
        throw invalid_argument("LeastSquare: polynomial degree must be non-negative.");
    if (degree >= rows)
        throw invalid_argument("LeastSquare: degree must be less than number of points.");

    int n = rows;
    int m = degree + 1;

    Matrix design(n, m);
    for (int i = 0; i < n; i++)
    {
        double x = data[i][0];
        double power = 1.0;
        for (int j = 0; j < m; j++)
        {
            design.set(i, j, power);
            power *= x;
        }
    }

    Matrix y(n, 1);
    for (int i = 0; i < n; i++)
        y.set(i, 0, data[i][1]);

    Matrix xt = design.transpose();
    Matrix xtx = xt * design;
    Matrix xty = xt * y;

    Matrix normalAugmented(m, m + 1);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
            normalAugmented.set(i, j, xtx.get(i, j));
        normalAugmented.set(i, m, xty.get(i, 0));
    }

    vector<double> coefficients = solveNormalEquation(normalAugmented);

    // Detailed output similar to Lagrange
    fout << fixed << setprecision(6);
    fout << "========================================\n";
    fout << "     Least Squares Curve Fitting\n";
    fout << "========================================\n\n";
    fout << "Number of data points (n) = " << n << "\n";
    fout << "Polynomial degree = " << degree << "\n";
    fout << "Number of coefficients (m) = " << m << "\n\n";

    printSamplePoints(fout);

    // Detailed computation table
    fout << "Computation Table:\n";
    fout << "====================================================================\n";
    fout << "  i  |    x_i    |    y_i    |   f_i(x)  | y_i - f_i |  (y_i-f_i)^2\n";
    fout << "-----+-----+-----+-----+-----+-----------+-----------+-----------\n";

    double sumSquaredResiduals = 0.0;
    for (int i = 0; i < n; i++)
    {
        double xi = data[i][0];
        double yi = data[i][1];
        
        double fi = 0.0;
        double power = 1.0;
        for (int j = 0; j < m; j++)
        {
            fi += coefficients[j] * power;
            power *= xi;
        }
        
        double residual = yi - fi;
        double residualSq = residual * residual;
        sumSquaredResiduals += residualSq;

        fout << setw(3) << i << " | "
             << setw(9) << xi << " | "
             << setw(9) << yi << " | "
             << setw(9) << fi << " | "
             << setw(9) << residual << " | "
             << setw(9) << residualSq << "\n";
    }
    fout << "====================================================================\n\n";

    fout << "Design Matrix X (" << n << " x " << m << "):\n";
    fout << "  (Each row: 1, x, x^2, ..., x^" << (m-1) << ")\n\n";
    design.displayToFile(fout);
    fout << "\n";

    fout << "Normal Equation Matrix X^T * X:\n";
    xtx.displayToFile(fout);
    fout << "\n";

    fout << "Right-hand side X^T * y:\n";
    xty.displayToFile(fout);
    fout << "\n";

    fout << "Augmented Normal System [X^T*X | X^T*y]:\n";
    normalAugmented.displayToFile(fout);
    fout << "\n";

    fout << "========================================\n";
    fout << "Fitted Polynomial Coefficients:\n";
    fout << "========================================\n";
    for (int i = 0; i < m; i++)
        fout << "a_" << i << " = " << coefficients[i] << "\n";
    fout << "\n";

    fout << "Polynomial form: f(x) = ";
    for (int i = 0; i < m; i++)
    {
        if (i > 0 && coefficients[i] >= 0)
            fout << "+ ";
        fout << coefficients[i];
        if (i > 0)
            fout << "*x^" << i;
        if (i < m - 1)
            fout << " ";
    }
    fout << "\n\n";

    // Error statistics
    double rss = residualSumOfSquares(coefficients);
    double mse = rss / n;
    double rmse = sqrt(mse);

    fout << "========================================\n";
    fout << "Error Statistics:\n";
    fout << "========================================\n";
    fout << "Residual Sum of Squares (RSS) = " << rss << "\n";
    fout << "Mean Squared Error (MSE) = " << mse << "\n";
    fout << "Root Mean Squared Error (RMSE) = " << rmse << "\n\n";

    return coefficients;
}

double LeastSquare::residualSumOfSquares(const vector<double> &coefficients) const
{
    validateSamplePoints();

    double rss = 0.0;
    for (int i = 0; i < rows; i++)
    {
        double x = data[i][0];
        double yTrue = data[i][1];

        double yPred = 0.0;
        double power = 1.0;
        for (size_t j = 0; j < coefficients.size(); j++)
        {
            yPred += coefficients[j] * power;
            power *= x;
        }

        double err = yTrue - yPred;
        rss += err * err;
    }

    return rss;
}

double LeastSquare::predict(double x, const vector<double> &coefficients, ofstream &fout)
{
    int degree = (int)coefficients.size() - 1;
    int m = (int)coefficients.size();

    fout << fixed << setprecision(6);
    fout << "========================================\n";
    fout << "  Prediction using Fitted Polynomial\n";
    fout << "========================================\n";
    fout << "Query point: x = " << x << "\n\n";

    fout << "Coefficients:\n";
    for (int i = 0; i < m; i++)
        fout << "a_" << i << " = " << coefficients[i] << "\n";
    fout << "\n";

    fout << "Polynomial: f(x) = ";
    for (int i = 0; i < m; i++)
    {
        if (i > 0 && coefficients[i] >= 0)
            fout << "+ ";
        fout << coefficients[i];
        if (i > 0)
            fout << "*x^" << i;
        if (i < m - 1)
            fout << " ";
    }
    fout << "\n\n";

    fout << "Calculation of f(" << x << "):\n";
    fout << "--------------------------------------------\n";

    double result = 0.0;
    double power = 1.0;
    for (int i = 0; i < m; i++)
    {
        double term = coefficients[i] * power;
        fout << "Term " << i << ": a_" << i << " * x^" << i
             << " = " << coefficients[i] << " * " << power
             << " = " << term << "\n";
        result += term;
        power *= x;
    }

    fout << "--------------------------------------------\n";
    fout << "f(" << x << ") = " << result << "\n";

    double rss = residualSumOfSquares(coefficients);
    double rmse = sqrt(rss / rows);
    fout << "\nFit Quality (RMSE) = " << rmse << "\n";

    return result;
}
