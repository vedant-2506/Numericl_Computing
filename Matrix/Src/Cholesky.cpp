// Cholesky decomposition implementation
#include "../Include/LUDecomposition.hpp"
#include <cmath>
using namespace std;

void LUDecomposition::choleskyDecomposition(const string &Afile,const string &bfile)
{
    ifstream fin(Afile);

    int n;
    fin>>n;

    vector<vector<double>> A(n,vector<double>(n));

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            fin>>A[i][j];

    vector<vector<double>> L(n,vector<double>(n,0));

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<=i;j++)
        {
            double sum=0;

            for(int k=0;k<j;k++)
                sum+=L[i][k]*L[j][k];

            if(i==j)
                L[i][j]=sqrt(A[i][i]-sum);
            else
                L[i][j]=(A[i][j]-sum)/L[j][j];
        }
    }

    ofstream fout("cholesky_matrix.txt");

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
            fout<<L[i][j]<<" ";
        fout<<endl;
    }
}