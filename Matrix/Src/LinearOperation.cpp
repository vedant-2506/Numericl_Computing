#include "../Include/LinearOperation.hpp"
using namespace std;

// create augmented matrix file [A|b]
void LinearOperation::generateAugmentedMatrixFile(
    const string &leftFile,
    const string &rightFile,
    const string &outputFile)
{
    ifstream finL(leftFile); // open matrix A file
    ifstream finR(rightFile); // open vector b file
    ofstream fout(outputFile); // output augmented matrix

    if(!finL||!finR||!fout)
        throw runtime_error("File error");

    int rL,cL,rR,cR;

    finL>>rL>>cL; // read A size
    finR>>rR>>cR; // read b size

    if(rL!=rR)
        throw runtime_error("Row mismatch");

    fout<<rL<<" "<<cL+1<<endl; // augmented column

    for(int i=0;i<rL;i++)
    {
        for(int j=0;j<cL;j++)
        {
            double val;
            finL>>val;
            fout<<val<<" "; // write A
        }

        double rhs;
        finR>>rhs;
        fout<<rhs<<endl; // append b
    }

    finL.close();
    finR.close();
    fout.close();
}