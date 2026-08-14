#include "../Include/PlotHelper.hpp"   // Include plot helper header
#include <fstream>                       // File output
#include <cstdlib>                       // System calls (for gnuplot)
#include <stdexcept>                     // runtime_error
using namespace std;

// ─────────────────────────────────────────────────────────
// Original method: index-vs-value line plot
// ─────────────────────────────────────────────────────────
void PlotHelper::saveAndPlot(
    const vector<double> &x,
    const string &datFile,
    const string &gnuFile,
    const string &pngFile,
    const string &title)
{
    ofstream dat(datFile);                          // Create data file
    for (int i = 0; i < (int)x.size(); i++)
        dat << (i + 1) << " " << x[i] << "\n";     // Write index and value
    dat.close();

    ofstream gnu(gnuFile);                          // Create gnuplot script
    gnu << "set terminal pngcairo size 800,600 enhanced font 'Helvetica,12'\n";
    gnu << "set output '" << pngFile << "'\n";
    gnu << "set title '" << title << "'\n";
    gnu << "set xlabel 'Variable Index'\n";
    gnu << "set ylabel 'Value'\n";
    gnu << "set grid\n";
    gnu << "plot '" << datFile
        << "' with linespoints lw 2 pt 7 title 'x'\n";
    gnu.close();

    int ret = system(("gnuplot \"" + gnuFile + "\"").c_str());
    if (ret != 0)
        throw runtime_error("PlotHelper::saveAndPlot: gnuplot failed.");
}

// ─────────────────────────────────────────────────────────
// New method: scatter data + fitted polynomial curve
// ─────────────────────────────────────────────────────────
void PlotHelper::plotCurveFit(
    const vector<double> &xData,
    const vector<double> &yData,
    const vector<double> &coeffs,
    const string &datFile,
    const string &gnuFile,
    const string &pngFile,
    const string &title)
{
    if (xData.size() != yData.size() || xData.empty())
        throw runtime_error("PlotHelper::plotCurveFit: xData and yData must be non-empty and equal size.");

    // ── 1. Write data points ──
    ofstream dat(datFile);
    if (!dat) throw runtime_error("PlotHelper::plotCurveFit: cannot create data file.");
    dat << "# x   y\n";
    for (size_t i = 0; i < xData.size(); i++)
        dat << xData[i] << "   " << yData[i] << "\n";
    dat.close();

    // ── 2. Build polynomial string for Gnuplot ──
    //   f(x) = c0 + c1*x + c2*x**2 + ...
    string polyExpr = "f(x) = ";
    int degree = (int)coeffs.size() - 1;
    for (int k = 0; k <= degree; k++)
    {
        if (k > 0) polyExpr += " + ";
        polyExpr += to_string(coeffs[k]);
        if (k == 1) polyExpr += "*x";
        if (k >= 2) polyExpr += "*x**" + to_string(k);
    }

    // ── 3. Determine x range ──
    double xMin = xData[0], xMax = xData[0];
    for (double v : xData)
    {
        if (v < xMin) xMin = v;
        if (v > xMax) xMax = v;
    }
    double margin = (xMax - xMin) * 0.1;

    // ── 4. Write Gnuplot script ──
    ofstream gnu(gnuFile);
    if (!gnu) throw runtime_error("PlotHelper::plotCurveFit: cannot create gnuplot script.");
    gnu << "set terminal pngcairo size 900,600 enhanced font 'Helvetica,13'\n";
    gnu << "set output '" << pngFile << "'\n";
    gnu << "set title '" << title << "' font 'Helvetica,15'\n";
    gnu << "set xlabel 'x'\n";
    gnu << "set ylabel 'y'\n";
    gnu << "set grid lc rgb '#cccccc' lw 1\n";
    gnu << "set key top left\n";
    gnu << "set xrange [" << (xMin - margin) << ":" << (xMax + margin) << "]\n";
    gnu << polyExpr << "\n\n";
    gnu << "plot '" << datFile << "' using 1:2 "
        << "with points pt 7 ps 1.5 lc rgb '#e74c3c' title 'Data Points', \\\n"
        << "     f(x) with lines lw 2.5 lc rgb '#2980b9' "
        << "title 'Fitted Curve (degree " << degree << ")'\n";
    gnu.close();

    // ── 5. Execute Gnuplot ──
    int ret = system(("gnuplot \"" + gnuFile + "\"").c_str());
    if (ret != 0)
        throw runtime_error("PlotHelper::plotCurveFit: gnuplot execution failed.");
}