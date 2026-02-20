#include "../Src/Gaussian.hpp"


// default constructor
Matrix::Matrix()
{
    this->rowCount = 0;
    this->colCount = 0;
}


// parameterized constructor
Matrix::Matrix(int r, int c)
{
    if(r <= 0 || c <= 0)
        throw invalid_argument("Matrix size must be positive.");

    this->rowCount = r;
    this->colCount = c;

    // Create r*c matrix and initialize with 0
    this->data.resize(r, vector<double>(c, 0));
}


// copy constructor
Matrix::Matrix(const Matrix &obj)
{
    this->rowCount = obj.rowCount;
    this->colCount = obj.colCount;
    this->data = obj.data;   // deep copy
}


// Read matrix 
void Matrix::inputData()
{
    cout << "Enter elements:\n";

    for(int i = 0; i < this->rowCount; i++)
        for(int j = 0; j < this->colCount; j++)
            cin >> this->data[i][j];
}


// Print matrix
void Matrix::printData()
{
    for(int i = 0; i < this->rowCount; i++)
    {
        for(int j = 0; j < this->colCount; j++)
            cout << this->data[i][j] << " ";
        cout << endl;
    }
}


// Matrix addition
Matrix Matrix::addMatrix(const Matrix &obj)
{
    if(this->rowCount != obj.rowCount ||
       this->colCount != obj.colCount)
        throw logic_error("Addition not possible.");

    Matrix result(this->rowCount, this->colCount);

    for(int i = 0; i < this->rowCount; i++)
        for(int j = 0; j < this->colCount; j++)
            result.data[i][j] = this->data[i][j] + obj.data[i][j];

    return result;
}


// Matrix Subtraction
Matrix Matrix::subMatrix(const Matrix &obj)
{
    if(this->rowCount != obj.rowCount ||
       this->colCount != obj.colCount)
        throw logic_error("Subtraction not possible.");

    Matrix result(this->rowCount, this->colCount);

    for(int i = 0; i < this->rowCount; i++)
        for(int j = 0; j < this->colCount; j++)
            result.data[i][j] = this->data[i][j] - obj.data[i][j];

    return result;
}


// Gaussian elimination 
void Matrix::performGaussian()
{
    int n = this->rowCount;

    for(int k = 0; k < n; k++)
    {
        if(this->data[k][k] == 0)
            throw runtime_error("Zero pivot error.");

        for(int i = k + 1; i < n; i++)
        {
            double factor = this->data[i][k] / this->data[k][k];

            for(int j = k; j < this->colCount; j++)
                this->data[i][j] -= factor * this->data[k][j];
        }
    }

    cout << "\nRow Echelon Form:\n";
    this->printData();
}


// back substitution to find solution
void Matrix::solveBack()
{
    int n = this->rowCount;
    vector<double> solution(n);

    for(int i = n - 1; i >= 0; i--)
    {
        solution[i] = this->data[i][n];

        for(int j = i + 1; j < n; j++)
            solution[i] -= this->data[i][j] * solution[j];

        solution[i] /= this->data[i][i];
    }

    cout << "\nSolution:\n";
    for(int i = 0; i < n; i++)
        cout << "x" << i+1 << " = " << solution[i] << endl;
}
