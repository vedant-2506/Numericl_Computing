#ifndef PLOT_HELPER_HPP                  // prevent multiple inclusion
#define PLOT_HELPER_HPP                  // define header guard

#include <vector>                        // for std::vector
#include <string>                        // for std::string

class PlotHelper                         // utility class for gnuplot output
{
public:                                  // accessible by everyone
    static void saveAndPlot(             // save data and generate PNG graph
        const std::vector<double> &x,   // solution vector to plot
        const std::string &datFile,     // .dat data file path
        const std::string &gnuFile,     // .gnu script file path
        const std::string &pngFile,     // .png output image path
        const std::string &title);      // graph title string
};

#endif                                   // end header guard