#ifndef PLOT_HELPER_HPP                           // prevent multiple inclusion
#define PLOT_HELPER_HPP                           // define header guard

#include <vector>                        // for std::vector
#include <string>                        // for std::string

class PlotHelper                                  // declare class
{
public:
    void saveAndPlot(                             // execute statement
        const std::vector<double> &x,             // execute statement
        const std::string &datFile,               // declare variable
        const std::string &gnuFile,               // declare variable
        const std::string &pngFile,               // declare variable
        const std::string &title);                // declare variable
};

#endif                                            // end header guard