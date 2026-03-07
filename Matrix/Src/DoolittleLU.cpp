#include "../Include/LUDecomposition.hpp"
using namespace std;

void LUDecomposition::doolittleLU(const string &Afile,const string &bfile)
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

    for(int i=0;i<n;i++)
    {
        for(int k=i;k<n;k++)
        {
            double sum=0;

            for(int j=0;j<i;j++)
                sum+=L[i][j]*U[j][k];

            U[i][k]=A[i][k]-sum;
        }

        for(int k=i;k<n;k++)
        {
            if(i==k)
                L[i][i]=1;
            else
            {
                double sum=0;

                for(int j=0;j<i;j++)
                    sum+=L[k][j]*U[j][i];

                L[k][i]=(A[k][i]-sum)/U[i][i];
            }
        }
    }

    ofstream fout("doolittle_matrix.txt");

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
            fout<<U[i][j]<<" ";
        fout<<endl;
    }
}