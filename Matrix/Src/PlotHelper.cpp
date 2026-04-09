#include "../Include/PlotHelper.hpp"  // Include plot helper header
#include <fstream>                      // File output
#include <cstdlib>                      // System calls (for gnuplot)
using namespace std;

void PlotHelper::saveAndPlot(           // Generate plot from data
    const vector<double> &x,            // Data values to plot
    const string &datFile,              // Output data file path
    const string &gnuFile,              // Gnuplot script file path
    const string &pngFile,              // Output PNG image file path
    const string &title)                // Plot title
{
    ofstream dat(datFile);              // Create data file
    for (int i = 0; i < (int)x.size(); i++)  // Loop through data points
        dat << (i+1) << " " << x[i] << "\n";  // Write index and value
    dat.close();                        // Close data file

    ofstream gnu(gnuFile);              // Create gnuplot script
    gnu << "set terminal png size 800,600\n";  // PNG output settings
    gnu << "set output '" << pngFile << "'\n";  // Output file name
    gnu << "set title '" << title << "'\n";  // Set plot title
    gnu << "set xlabel 'Variable Index'\n";  // X-axis label
    gnu << "set ylabel 'Value'\n";       // Y-axis label
    gnu << "set grid\n";                // Enable grid
    gnu << "plot '" << datFile           // Plot command
        << "' with linespoints lw 2 pt 7 title 'x'\n";  // Style
    gnu.close();                        // Close script file

    system(("gnuplot " + gnuFile).c_str());  // Execute gnuplot
}