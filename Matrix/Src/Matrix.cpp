#include "../Include/Matrix.hpp"           // Include matrix header
#include <stdexcept>                       // Exception handling
using namespace std;

Matrix::Matrix() : rows(0), cols(0) {}                  // Empty matrix constructor

Matrix::Matrix(int r, int c) : rows(r), cols(c),       // Dimensions constructor
    data(r, vector<double>(c, 0.0)) {}                // Initialize with zeros

Matrix::Matrix(const Matrix &m) : rows(m.rows),      // Copy constructor
    cols(m.cols), data(m.data) {}  

int    Matrix::getRows()  const { return rows; }     // Return row count
int    Matrix::getCols()  const { return cols; }     // Return column count

double Matrix::get(int i, int j) const { return data[i][j]; }        // Get element
void   Matrix::set(int i, int j, double v) { data[i][j] = v; }      // Set element
vector<vector<double>>& Matrix::getData() { return data; }          // Get raw data

double& Matrix::operator()(int i, int j)       { return data[i][j]; }  // Access for write
double  Matrix::operator()(int i, int j) const { return data[i][j]; }  // Access for read

void Matrix::readFromFile(ifstream &fin)                          // Read matrix from file stream
{
    for (int i = 0; i < rows; i++)  
        for (int j = 0; j < cols; j++)  
            fin >> data[i][j];  
}

void Matrix::displayToFile(ofstream &fout) const  // Write matrix to file
{
    for (int i = 0; i < rows; i++)  // Loop through rows
    {
        for (int j = 0; j < cols; j++) fout << data[i][j] << " ";  
        fout << "\n";  // New line after row
    }
}

void Matrix::display() const  // Print matrix to console
{
    for (int i = 0; i < rows; i++)  // Loop through rows
    {
        for (int j = 0; j < cols; j++) cout << data[i][j] << " ";  
        cout << "\n"; 
    }
}

istream& operator>>(istream &in, Matrix &m)         // Read from input stream
{
    for (int i = 0; i < m.rows; i++)  
        for (int j = 0; j < m.cols; j++)  
            in >> m.data[i][j];  
    return in;                                     // Return stream for chaining
}

ostream& operator<<(ostream &out, const Matrix &m)  // Write to output stream
{
    for (int i = 0; i < m.rows; i++)  
    {
        for (int j = 0; j < m.cols; j++) out << m.data[i][j] << " ";  
        out << "\n";  
    }
    return out;                                    // Return stream for chaining
} 