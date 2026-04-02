#include "../Include/Matrix.hpp" // include Matrix header
#include <stdexcept>             // for runtime_error
using namespace std;                              // use standard namespace

Matrix::Matrix() : rows(0), cols(0) {}            // define function

Matrix::Matrix(int r, int c) : rows(r), cols(c),  // define function
    data(r, vector<double>(c, 0.0)) {}            // execute statement

Matrix::Matrix(const Matrix &m) : rows(m.rows),   // define function
    cols(m.cols), data(m.data) {}                 // execute statement

int    Matrix::getRows()  const { return rows; }  // define function
int    Matrix::getCols()  const { return cols; }  // define function
double Matrix::get(int i, int j) const { return data[i][j]; }  // define function
void   Matrix::set(int i, int j, double v) { data[i][j] = v; }  // define function
vector<vector<double>>& Matrix::getData() { return data; }  // define function

double& Matrix::operator()(int i, int j)       { return data[i][j]; }  // define function
double  Matrix::operator()(int i, int j) const { return data[i][j]; }  // define function

void Matrix::readFromFile(ifstream &fin)          // handle file stream operation
{
    for (int i = 0; i < rows; i++)                // iterate over elements
        for (int j = 0; j < cols; j++)            // iterate over elements
            fin >> data[i][j];                    // check equality or comparison
}

void Matrix::displayToFile(ofstream &fout) const  // define function
{
    for (int i = 0; i < rows; i++)                // iterate over elements
    {
        for (int j = 0; j < cols; j++) fout << data[i][j] << " ";  // iterate over elements
        fout << "\n";                             // check equality or comparison
    }
}

void Matrix::display() const                      // define function
{
    for (int i = 0; i < rows; i++)                // iterate over elements
    {
        for (int j = 0; j < cols; j++) cout << data[i][j] << " ";  // iterate over elements
        cout << "\n";                             // check equality or comparison
    }
}

istream& operator>>(istream &in, Matrix &m)       // stream input/output operation
{
    for (int i = 0; i < m.rows; i++)              // iterate over elements
        for (int j = 0; j < m.cols; j++)          // iterate over elements
            in >> m.data[i][j];                   // check equality or comparison
    return in;                                    // return result from function
}

ostream& operator<<(ostream &out, const Matrix &m)  // stream input/output operation
{
    for (int i = 0; i < m.rows; i++)              // iterate over elements
    {
        for (int j = 0; j < m.cols; j++) out << m.data[i][j] << " ";  // iterate over elements
        out << "\n";                              // check equality or comparison
    }
    return out;                                   // return result from function
}