#include "../Include/Matrix.hpp"

// default constructor
Matrix::Matrix()
{
    rows=0;
    cols=0;
}

// parameter constructor
Matrix::Matrix(int r,int c)
{
    rows=r;
    cols=c;
    data.resize(r,vector<double>(c)); // allocate matrix
}

// set value
void Matrix::set(int i,int j,double value)
{
    data[i][j]=value;
}

// get value
double Matrix::get(int i,int j) const
{
    return data[i][j];
}

// get rows
int Matrix::getRows() const
{
    return rows;
}

// get columns
int Matrix::getCols() const
{
    return cols;
}

// return matrix data
vector<vector<double>>& Matrix::getData()
{
    return data;
}

// read matrix from file
void Matrix::readFromFile(ifstream &fin)
{
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            fin>>data[i][j];
}

// display matrix
void Matrix::display() const
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
            cout<<data[i][j]<<" ";
        cout<<endl;
    }
}