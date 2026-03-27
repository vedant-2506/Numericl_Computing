#include "../Include/LinearOperation.hpp" // include header
#include <stdexcept>                       // for runtime_error
using namespace std;                       // use standard namespace

static string normalizeInputPath(const string &path)
{
    // Accept common typo: "Input49l.txt" -> "Input/49l.txt"
    if (path.rfind("Input", 0) == 0 && path.size() > 5 && path[5] != '/')
        return "Input/" + path.substr(5);
    return path;
}

// ---- CONSTRUCTORS ----
LinearOperation::LinearOperation() : Matrix() {}                // default constructor
LinearOperation::LinearOperation(int r, int c) : Matrix(r,c){} // parameterized constructor

// ---- FRIEND OPERATOR (friend = can access protected members) ----
ostream& operator<<(ostream &out, const LinearOperation &lo)    // print to stream
{
    out << "LinearOperation(" << lo.rows << "x" << lo.cols << "):\n"; // access protected
    for (int i = 0; i < lo.rows; i++)                           // loop rows
    {
        for (int j = 0; j < lo.cols; j++) out << lo.data[i][j] << " "; // access protected data
        out << "\n";                                            // newline
    }
    return out;                                                 // return stream
}

// ---- AUGMENTED MATRIX BUILDER ----
void LinearOperation::generateAugmentedMatrixFile(
    const string &leftFile, const string &rightFile, const string &outputFile)
{
    string leftPath  = normalizeInputPath(leftFile);
    string rightPath = normalizeInputPath(rightFile);

    ifstream finL(leftPath);                // open matrix A file
    ifstream finR(rightPath);               // open vector b file
    ofstream fout(outputFile);             // open output augmented file
    if (!finL || !finR || !fout)           // check all files opened
        throw runtime_error("File error: cannot open A='" + leftPath +
                            "', b='" + rightPath +
                            "', out='" + outputFile + "'");
    int rL, cL, rR, cR;
    finL >> rL >> cL;                      // read A dimensions
    finR >> rR >> cR;                      // read b dimensions
    if (rL != rR) throw runtime_error("Row mismatch"); // rows must match
    fout << rL << " " << cL+1 << "\n";    // write augmented dimensions
    for (int i = 0; i < rL; i++)           // loop rows
    {
        for (int j = 0; j < cL; j++)      // loop A columns
            { double v; finL >> v; fout << v << " "; } // read and write A element
        double rhs; finR >> rhs;           // read b element
        fout << rhs << "\n";             // append b to row
    }
}