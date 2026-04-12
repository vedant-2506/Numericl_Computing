#include "../Include/Matrix.hpp"           // Include matrix header
#include <stdexcept>                       // Exception handling
#include <string>
using namespace std;

Matrix::Matrix() : rows(0), cols(0), loaded(false) {}                  // Empty matrix constructor

Matrix::Matrix(int r, int c) : rows(r), cols(c),       // Dimensions constructor
    data(r, vector<double>(c, 0.0)), loaded(r > 0 && c > 0) {}                // Initialize with zeros

Matrix::Matrix(const Matrix &m) : rows(m.rows),      // Copy constructor
    cols(m.cols), data(m.data), loaded(m.loaded) {}  

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
            if (!(fin >> data[i][j]))
                throw runtime_error("Matrix::readFromFile: invalid or incomplete matrix data");
    loaded = true;
}

bool Matrix::loadFromFile(const string &path, string *errorMessage)
{
    ifstream fin(path);
    if (!fin)
    {
        if (errorMessage) *errorMessage = "Cannot open file: " + path;
        loaded = false;
        return false;
    }

    int r = 0, c = 0;
    if (!(fin >> r >> c) || r <= 0 || c <= 0)
    {
        if (errorMessage) *errorMessage = "Invalid matrix header in file: " + path + " (expected positive rows and cols)";
        loaded = false;
        return false;
    }

    vector<vector<double>> temp(r, vector<double>(c, 0.0));
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (!(fin >> temp[i][j]))
            {
                if (errorMessage) *errorMessage = "Invalid or incomplete matrix data in file: " + path;
                loaded = false;
                return false;
            }
        }
    }

    rows = r;
    cols = c;
    data = temp;
    loaded = true;

    if (errorMessage) errorMessage->clear();
    return true;
}

bool Matrix::isLoadFile(const string &path, string *errorMessage)
{
    return loadFromFile(path, errorMessage);
}

bool Matrix::isLoaded() const { return loaded; }

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