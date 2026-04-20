#include "../Include/CurveFitting.hpp"
#include <iomanip>
#include <stdexcept>

using namespace std;

CurveFitting::CurveFitting() : Matrix() {}

CurveFitting::CurveFitting(const Matrix &m) : Matrix(m)
{
    validateSamplePoints();
}

void CurveFitting::loadSamplePoints(ifstream &fin)
{
    if (!fin)
        throw runtime_error("CurveFitting: file stream is not open.");

    int n = 0;
    if (!(fin >> n) || n <= 0)
        throw runtime_error("CurveFitting: invalid or missing point count in file.");

    rows = n;
    cols = 2;
    data.assign(rows, vector<double>(cols, 0.0));

    for (int i = 0; i < n; i++)
    {
        if (!(fin >> data[i][0] >> data[i][1]))
            throw runtime_error("CurveFitting: unexpected end of file while reading points.");
    }

    loaded = true;
}

void CurveFitting::printSamplePoints(ofstream &fout) const
{
    validateSamplePoints();
    fout << fixed << setprecision(6);

    fout << "Input Sample Points:\n";
    for (int i = 0; i < rows; i++)
        fout << "Point " << i << " : x = " << data[i][0] << ", y = " << data[i][1] << "\n";
    fout << "\n";
}

void CurveFitting::validateSamplePoints() const
{
    if (cols != 2)
        throw logic_error("CurveFitting: data matrix must have exactly 2 columns (x, y).");
    if (rows < 2)
        throw logic_error("CurveFitting: at least 2 points are required.");
}

double CurveFitting::predict(double x, const vector<double> &coefficients) const
{
    double y = 0.0;
    double power = 1.0;
    for (size_t i = 0; i < coefficients.size(); i++)
    {
        y += coefficients[i] * power;
        power *= x;
    }
    return y;
}
