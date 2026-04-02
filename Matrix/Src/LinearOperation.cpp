#include "../Include/LinearOperation.hpp" // include header
#include <stdexcept>                       // for runtime_error
using namespace std;                              // use standard namespace

string normalizeInputPath(const string &path)         // declare and initialize object
{
    if (path.rfind("Input", 0) == 0 && path.size() > 5 && path[5] != '/')  // check condition
        return "Input/" + path.substr(5);         // return result from function
    return path;                                  // return result from function
}

LinearOperation::LinearOperation() : Matrix() {}  // define function
LinearOperation::LinearOperation(int r, int c) : Matrix(r,c){}  // define function

ostream& operator<<(ostream &out, const LinearOperation &lo)  // stream input/output operation
{
    out << "LinearOperation(" << lo.rows << "x" << lo.cols << "):\n";  // check equality or comparison
    for (int i = 0; i < lo.rows; i++)             // iterate over elements
    {
        for (int j = 0; j < lo.cols; j++) out << lo.data[i][j] << " ";  // iterate over elements
        out << "\n";                              // check equality or comparison
    }
    return out;                                   // return result from function
}

void LinearOperation::generateAugmentedMatrixFile(  // define function
    const string &leftFile, const string &rightFile, const string &outputFile)  // declare variable
{
    string leftPath  = normalizeInputPath(leftFile);  // declare and initialize object
    string rightPath = normalizeInputPath(rightFile);  // declare and initialize object

    ifstream finL(leftPath);                      // handle file stream operation
    ifstream finR(rightPath);                     // handle file stream operation
    ofstream fout(outputFile);                    // call function or method
    if (!finL || !finR || !fout)                  // check condition
        throw runtime_error("File error: cannot open A='" + leftPath +  // raise exception with error
                            "', b='" + rightPath +  // execute statement
                            "', out='" + outputFile + "'");  // assign value to variable
    int rL, cL, rR, cR;                           // declare variable
    finL >> rL >> cL;                             // check equality or comparison
    finR >> rR >> cR;                             // check equality or comparison
    if (rL != rR) throw runtime_error("Row mismatch");  // check condition
    fout << rL << " " << cL+1 << "\n";            // check equality or comparison
    for (int i = 0; i < rL; i++)                  // iterate over elements
    {
        for (int j = 0; j < cL; j++)              // iterate over elements
            { double v; finL >> v; fout << v << " "; }  // declare variable
        double rhs; finR >> rhs;                  // declare variable
        fout << rhs << "\n";                      // check equality or comparison
    }
}