#include "../Include/PlotHelper.hpp" // include plot helper header
#include <fstream>                   // for ofstream
#include <cstdlib>                   // for system()
using namespace std;                              // use standard namespace

void PlotHelper::saveAndPlot(                     // define function
    const vector<double> &x,                      // execute statement
    const string &datFile,                        // declare variable
    const string &gnuFile,                        // declare variable
    const string &pngFile,                        // declare variable
    const string &title)                          // declare variable
{
    ofstream dat(datFile);                        // call function or method
    for (int i = 0; i < (int)x.size(); i++)       // iterate over elements
        dat << (i+1) << " " << x[i] << "\n";      // check equality or comparison
    dat.close();                                  // call function or method

    ofstream gnu(gnuFile);                        // call function or method
    gnu << "set terminal png size 800,600\n";     // check equality or comparison
    gnu << "set output '" << pngFile << "'\n";    // check equality or comparison
    gnu << "set title '" << title << "'\n";       // check equality or comparison
    gnu << "set xlabel 'Variable Index'\n";       // check equality or comparison
    gnu << "set ylabel 'Value'\n";                // check equality or comparison
    gnu << "set grid\n";                          // check equality or comparison
    gnu << "plot '" << datFile                    // stream input/output operation
        << "' with linespoints lw 2 pt 7 title 'x'\n";  // check equality or comparison
    gnu.close();                                  // call function or method

    system(("gnuplot " + gnuFile).c_str());       // call function or method
}