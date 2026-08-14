#ifndef PLOT_HELPER_HPP             // Guard against double inclusion
#define PLOT_HELPER_HPP

#include <vector>                   // For storing data points
#include <string>                   // For file paths

class PlotHelper                    // Utility class to generate plots using Gnuplot
{
public:
    // Original method: plots a 1D value series by index
    void saveAndPlot(
        const std::vector<double> &x,       // Data values to plot
        const std::string &datFile,          // Output data file path
        const std::string &gnuFile,          // Gnuplot script file path
        const std::string &pngFile,          // Output PNG image file path
        const std::string &title);           // Plot title

    // New method: plots (x,y) scatter + a fitted polynomial curve
    void plotCurveFit(
        const std::vector<double> &xData,    // x coordinates of data points
        const std::vector<double> &yData,    // y coordinates of data points
        const std::vector<double> &coeffs,   // Polynomial coefficients [a0,a1,...,an]
        const std::string &datFile,          // Output data file path
        const std::string &gnuFile,          // Gnuplot script file path
        const std::string &pngFile,          // Output PNG image file path
        const std::string &title);           // Plot title
};

#endif