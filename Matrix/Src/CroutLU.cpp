#include "../Include/LUDecomposition.hpp"
using namespace std;

void LUDecomposition::croutLU(const string &Afile,const string &bfile)
{
    ifstream fin(Afile);
    int n;

    fin>>n;

    vector<vector<double>> A(n,vector<double>(n));

    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            fin>>A[i][j];

    vector<vector<double>> L(n,vector<double>(n,0));
    vector<vector<double>> U(n,vector<double>(n,0));

    for(int j=0;j<n;j++)
    {
        U[j][j]=1;

        for(int i=j;i<n;i++)
        {
            double sum=0;

            for(int k=0;k<j;k++)
                sum+=L[i][k]*U[k][j];

            L[i][j]=A[i][j]-sum;
        }

        for(int i=j+1;i<n;i++)
        {
            double sum=0;

            for(int k=0;k<j;k++)
                sum+=L[j][k]*U[k][i];

            U[j][i]=(A[j][i]-sum)/L[j][j];
        }
    }
}