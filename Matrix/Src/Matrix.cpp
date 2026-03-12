#include "../Include/Matrix.hpp" // include Matrix header
#include <stdexcept>             // for runtime_error
using namespace std;             // use standard namespace

// ---- CONSTRUCTORS ----
Matrix::Matrix() : rows(0), cols(0) {}                          // default: empty matrix

Matrix::Matrix(int r, int c) : rows(r), cols(c),                // parameterized constructor
    data(r, vector<double>(c, 0.0)) {}                          // allocate r x c with zeros

Matrix::Matrix(const Matrix &m) : rows(m.rows),                 // copy constructor
    cols(m.cols), data(m.data) {}                               // deep copy all data

// ---- GETTERS AND SETTERS ----
int    Matrix::getRows()  const { return rows; }                // return row count
int    Matrix::getCols()  const { return cols; }                // return column count
double Matrix::get(int i, int j) const { return data[i][j]; }  // get element at (i,j)
void   Matrix::set(int i, int j, double v) { data[i][j] = v; } // set element at (i,j)
vector<vector<double>>& Matrix::getData() { return data; }      // return data reference

// ---- ELEMENT ACCESS OPERATORS ----
double& Matrix::operator()(int i, int j)       { return data[i][j]; } // read/write
double  Matrix::operator()(int i, int j) const { return data[i][j]; } // read-only

// ---- FILE I/O ----
void Matrix::readFromFile(ifstream &fin)                        // read matrix from file
{
    for (int i = 0; i < rows; i++)                              // loop rows
        for (int j = 0; j < cols; j++)                         // loop columns
            fin >> data[i][j];                                 // read element
}

void Matrix::displayToFile(ofstream &fout) const                // write matrix to file
{
    for (int i = 0; i < rows; i++)                              // loop rows
    {
        for (int j = 0; j < cols; j++) fout << data[i][j] << " "; // write element
        fout << "\n";                                          // newline after row
    }
}

void Matrix::display() const                                    // print to console
{
    for (int i = 0; i < rows; i++)                              // loop rows
    {
        for (int j = 0; j < cols; j++) cout << data[i][j] << " "; // print element
        cout << "\n";                                          // newline
    }
}

// ---- FRIEND STREAM OPERATORS (friend = can access protected data directly) ----
istream& operator>>(istream &in, Matrix &m)                     // read from stream
{
    for (int i = 0; i < m.rows; i++)                            // access protected rows
        for (int j = 0; j < m.cols; j++)                       // access protected cols
            in >> m.data[i][j];                                // access protected data
    return in;                                                  // return stream
}

ostream& operator<<(ostream &out, const Matrix &m)              // write to stream
{
    for (int i = 0; i < m.rows; i++)                            // access protected rows
    {
        for (int j = 0; j < m.cols; j++) out << m.data[i][j] << " "; // access protected data
        out << "\n";                                           // newline
    }
    return out;                                                 // return stream
}