#ifndef PLOT_HELPER_HPP         // Guard against double inclusion
#define PLOT_HELPER_HPP

#include <vector>                  // For storing data points
#include <string>                  // For file paths

class PlotHelper              // Utility class to generate plots using Gnuplot
{
public:
    void saveAndPlot(            // Save data and create plot
        const std::vector<double> &x,   // Data points to plot
        const std::string &datFile,     // Output data file path
        const std::string &gnuFile,     // Gnuplot script file path
        const std::string &pngFile,     // Output PNG image file path
        const std::string &title);      // Plot title
};

#endif