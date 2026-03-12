#include "../Include/PlotHelper.hpp" // include plot helper header
#include <fstream>                   // for ofstream
#include <cstdlib>                   // for system()
using namespace std;                 // use standard namespace

void PlotHelper::saveAndPlot(
    const vector<double> &x,         // solution vector to plot
    const string &datFile,           // path for .dat data file
    const string &gnuFile,           // path for .gnu script file
    const string &pngFile,           // path for output .png image
    const string &title)             // graph title string
{
    ofstream dat(datFile);            // open data file
    for (int i = 0; i < (int)x.size(); i++)    // loop solution values
        dat << (i+1) << " " << x[i] << "\n";  // write index and value
    dat.close();                      // close data file

    ofstream gnu(gnuFile);            // open gnuplot script file
    gnu << "set terminal png size 800,600\n";       // set PNG output format
    gnu << "set output '" << pngFile << "'\n";      // set output file path
    gnu << "set title '" << title << "'\n";         // set graph title
    gnu << "set xlabel 'Variable Index'\n";          // x axis label
    gnu << "set ylabel 'Value'\n";                  // y axis label
    gnu << "set grid\n";                            // show grid lines
    gnu << "plot '" << datFile                      // plot data file
        << "' with linespoints lw 2 pt 7 title 'x'\n"; // line style
    gnu.close();                      // close script file

    system(("gnuplot " + gnuFile).c_str()); // run gnuplot to generate PNG
}