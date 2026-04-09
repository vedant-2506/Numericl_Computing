#include "../Include/Interpolation.hpp"  // Include interpolation header
#include <stdexcept>                     // For exception handling
#include <iomanip>                       // For output formatting
#include <string>                        // For string handling

using namespace std;

Interpolation::Interpolation() : Matrix() {}                // Default constructor

Interpolation::Interpolation(const Matrix &m) : Matrix(m)  // Constructor from data
{
    validateDataPoints();  // Validate data on creation
}

void Interpolation::loadDataPoints(ifstream &fin)               // Load (x, f(x)) pairs from file
{
    if (!fin)  // Check if stream is open
        throw runtime_error("Interpolation: file stream is not open.");  

    int n;  // Number of data points
    if (!(fin >> n) || n <= 0)  // Read point count and validate
        throw runtime_error("Interpolation: invalid or missing data-point count in file.");  // Invalid count

    rows = n;  // Set number of rows
    cols = 2;  // Always 2 columns: x and y
    data.assign(rows, vector<double>(cols, 0.0));  // Initialize data storage

    for (int i = 0; i < n; i++)  // Read each data point
    {
        if (!(fin >> data[i][0] >> data[i][1]))  // Read x and y values
            throw runtime_error("Interpolation: unexpected end of file at row " + to_string(i));  // Missing data
    }
}

void Interpolation::printDataPoints(ofstream &fout) const  // Display data points to file
{
    validateDataPoints();  // Ensure valid data
    fout << fixed << setprecision(6);  // Set decimal precision

    fout << "\nData Points:\n";  // Header
    for (int i = 0; i < rows; i++)  // Loop through points
    {
        fout << "Point " << i  // Point number
             << " : x = " << data[i][0]  // x value
             << ", f(x) = " << data[i][1] << "\n";  // y value
    }
    fout << "\n";  // Blank line
}

void Interpolation::validateDataPoints() const  // Check data validity
{
    if (cols != 2)  // Check exactly 2 columns
        throw logic_error("Interpolation: data matrix must have exactly 2 columns (x and y).");  // Wrong format
    if (rows < 2)  // Need at least 2 points
        throw logic_error("Interpolation: at least 2 data points are required.");  // Insufficient data
}
