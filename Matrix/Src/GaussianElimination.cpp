#include "../Include/GaussianElimination.hpp"
#include <cstdlib>
using namespace std;

void GaussianElimination::gaussianEliminationWithPivoting(
    Matrix &matrix,
    ofstream &matrixOut,
    ofstream &vectorOut)
{
    int n=matrix.getRows(); // number of equations
    auto &A=matrix.getData(); // reference matrix

    // forward elimination
    for(int k=0;k<n-1;k++)
    {
        int maxRow=k;

        for(int i=k+1;i<n;i++) // find pivot
            if(abs(A[i][k])>abs(A[maxRow][k]))
                maxRow=i;

        swap(A[k],A[maxRow]); // swap rows

        for(int i=k+1;i<n;i++)
        {
            double factor=A[i][k]/A[k][k];

            for(int j=k;j<n+1;j++)
                A[i][j]-=factor*A[k][j]; // elimination
        }
    }

    // save answer matrix
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n+1;j++)
            matrixOut<<A[i][j]<<" ";
        matrixOut<<endl;
    }

    vector<double> x(n); // solution vector

    // back substitution
    for(int i=n-1;i>=0;i--)
    {
        x[i]=A[i][n];

        for(int j=i+1;j<n;j++)
            x[i]-=A[i][j]*x[j];

        x[i]/=A[i][i];
    }

    // save vector
    ofstream plot("plot.dat");

    for(int i=0;i<n;i++)
    {
        vectorOut<<x[i]<<endl;
        plot<<i+1<<" "<<x[i]<<endl;
    }

    plot.close();

    // generate graph using gnuplot
    system("gnuplot -e \"plot 'plot.dat' with linespoints\"");
}