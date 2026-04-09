#include "../Include/LinearOperation.hpp"  // Include linear operation header
#include <stdexcept>                        // For exception handling
using namespace std;

string normalizeInputPath(const string &path)  // Helper to normalize file paths
{
    if (path.rfind("Input", 0) == 0 && path.size() > 5 && path[5] != '/')  // Check if needs Input/ prefix
        return "Input/" + path.substr(5);  // Add prefix if missing
    return path;  // Return unchanged if already has prefix
}

LinearOperation::LinearOperation() : Matrix() {}  // Default constructor
LinearOperation::LinearOperation(int r, int c) : Matrix(r,c){}  // Constructor with dimensions

ostream& operator<<(ostream &out, const LinearOperation &lo)  // Output operator
{
    out << "LinearOperation(" << lo.rows << "x" << lo.cols << "):\n";  // Header
    for (int i = 0; i < lo.rows; i++)  // Loop through rows
    {
        for (int j = 0; j < lo.cols; j++) out << lo.data[i][j] << " ";  // Print each element
        out << "\n";  // New line
    }
    return out;  // Return stream
}

void LinearOperation::generateAugmentedMatrixFile(  // Create augmented matrix from separate files
    const string &leftFile, const string &rightFile, const string &outputFile)
{
    string leftPath  = normalizeInputPath(leftFile);  // Normalize matrix file path
    string rightPath = normalizeInputPath(rightFile);  // Normalize vector file path

    ifstream finL(leftPath);  // Open coefficient matrix
    ifstream finR(rightPath);  // Open constant vector
    ofstream fout(outputFile);  // Open output file
    if (!finL || !finR || !fout)  // Check all files opened
        throw runtime_error("File error: cannot open A='" + leftPath +  // File error message
                            "', b='" + rightPath +
                            "', out='" + outputFile + "'");
    int rL, cL, rR, cR;  // Dimension variables
    finL >> rL >> cL;  // Read matrix dimensions
    finR >> rR >> cR;  // Read vector dimensions
    if (rL != rR) throw runtime_error("Row mismatch");  // Check row match
    fout << rL << " " << cL+1 << "\n";  // Write augmented dimensions
    for (int i = 0; i < rL; i++)  // Loop through rows
    {
        for (int j = 0; j < cL; j++)  // Loop through matrix columns
            { double v; finL >> v; fout << v << " "; }  // Load and write matrix element
        double rhs; finR >> rhs;  // Read vector element
        fout << rhs << "\n";  // Write augmented element
    }
}