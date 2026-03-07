#include "../Include/BasicOperation.hpp"
using namespace std;

// default constructor
BasicOperation::BasicOperation():Matrix(){}

// parameterized constructor
BasicOperation::BasicOperation(int r,int c):Matrix(r,c){}

// matrix addition
BasicOperation BasicOperation::add(const BasicOperation &other) const
{
    if(rows!=other.rows || cols!=other.cols) // check dimension
        throw runtime_error("Dimension mismatch");

    BasicOperation result(rows,cols); // create result matrix

    for(int i=0;i<rows;i++) // loop rows
        for(int j=0;j<cols;j++) // loop columns
            result.data[i][j]=data[i][j]+other.data[i][j]; // add elements

    return result; // return result
}

// matrix subtraction
BasicOperation BasicOperation::subtract(const BasicOperation &other) const
{
    if(rows!=other.rows || cols!=other.cols)
        throw runtime_error("Dimension mismatch");

    BasicOperation result(rows,cols);

    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            result.data[i][j]=data[i][j]-other.data[i][j]; // subtract elements

    return result;
}

// matrix multiplication
BasicOperation BasicOperation::multiply(const BasicOperation &other) const
{
    if(cols!=other.rows)
        throw runtime_error("Invalid multiplication");

    BasicOperation result(rows,other.cols);

    for(int i=0;i<rows;i++)
        for(int j=0;j<other.cols;j++)
            for(int k=0;k<cols;k++)
                result.data[i][j]+=data[i][k]*other.data[k][j]; // multiplication logic

    return result;
}

// scalar division
BasicOperation BasicOperation::scalarDivide(double value) const
{
    BasicOperation result(rows,cols);

    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            result.data[i][j]=data[i][j]/value; // divide each element

    return result;
}

// transpose matrix
BasicOperation BasicOperation::transpose() const
{
    BasicOperation result(cols,rows);

    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            result.data[j][i]=data[i][j]; // swap rows and columns

    return result;
}

// determinant using recursive expansion
double BasicOperation::determinant() const
{
    if(rows!=cols)
        throw runtime_error("Not square matrix");

    if(rows==1)
        return data[0][0];

    if(rows==2)
        return data[0][0]*data[1][1]-data[0][1]*data[1][0];

    double det=0;

    for(int p=0;p<cols;p++)
    {
        BasicOperation sub(rows-1,cols-1);

        for(int i=1;i<rows;i++)
        {
            int colIndex=0;

            for(int j=0;j<cols;j++)
            {
                if(j==p) continue;

                sub.data[i-1][colIndex]=data[i][j];
                colIndex++;
            }
        }

        det+=pow(-1,p)*data[0][p]*sub.determinant();
    }

    return det;
}

// write matrix to file
void BasicOperation::displayToFile(ofstream &fout) const
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
            fout<<data[i][j]<<" ";
        fout<<endl;
    }
}