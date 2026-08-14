#include "../Include/GnuplotHelper.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cmath>

using namespace std;

// Constructor
GnuplotHelper::GnuplotHelper(const string& outDir) : outputDir(outDir) {}

// Destructor
GnuplotHelper::~GnuplotHelper() {}

// Write data file for plotting
bool GnuplotHelper::writeDataFile(const string& filename,
                                 const vector<double>& xData,
                                 const vector<double>& yData)
{
    ofstream outfile(outputDir + "/" + filename);
    
    if (!outfile.is_open())
    {
        cerr << "Error: Cannot write to " << filename << endl;
        return false;
    }
    
    for (size_t i = 0; i < xData.size(); i++)
    {
        outfile << fixed << setprecision(6) 
                << xData[i] << " " << yData[i] << "\n";
    }
    
    outfile.close();
    return true;
}

// Generate main Lagrange interpolation visualization
void GnuplotHelper::generateLagrangeVisualization(
    const vector<double>& xData,
    const vector<double>& yData,
    const vector<double>& basisValues,
    double queryPoint,
    double interpolatedValue)
{
    // Write data files
    writeDataFile("weather_data.txt", xData, yData);
    
    // Create interpolated points for smooth curve
    vector<double> xInterp, yInterp;
    int numInterpolationPoints = 500;
    double xMin = xData[0];
    double xMax = xData[xData.size() - 1];
    
    for (int i = 0; i <= numInterpolationPoints; i++)
    {
        double x = xMin + (xMax - xMin) * i / numInterpolationPoints;
        xInterp.push_back(x);
        
        // Simple interpolation using basis values (simplified for visualization)
        double y = 0.0;
        for (size_t j = 0; j < xData.size(); j++)
        {
            double xi = xData[j];
            double yi = yData[j];
            double Li = 1.0;
            
            for (size_t k = 0; k < xData.size(); k++)
            {
                if (k == j) continue;
                double xk = xData[k];
                Li *= (x - xk) / (xi - xk);
            }
            y += yi * Li;
        }
        yInterp.push_back(y);
    }
    
    writeDataFile("interpolation_curve.txt", xInterp, yInterp);
    
    // Generate GnuPlot script
    ofstream gpScript(outputDir + "/lagrange_plot.gnu");
    
    gpScript << "#!/usr/bin/gnuplot\n\n";
    gpScript << "set terminal png size 1200,700\n";
    gpScript << "set output 'Output/lagrange_interpolation.png'\n\n";
    
    gpScript << "set title 'Weather Station Temperature - Lagrange Interpolation' font 'arial,14'\n";
    gpScript << "set xlabel 'Time (hours)' font 'arial,12'\n";
    gpScript << "set ylabel 'Temperature (°C)' font 'arial,12'\n";
    gpScript << "set grid\n";
    gpScript << "set style data points\n";
    gpScript << "set pointsize 1.5\n\n";
    
    gpScript << "# Plot original data and interpolation curve\n";
    gpScript << "plot 'Output/weather_data.txt' using 1:2 with points pt 7 ps 1.2 "
             << "linecolor rgb 'blue' title 'Original Data', \\\n";
    gpScript << "     'Output/interpolation_curve.txt' using 1:2 with lines "
             << "linecolor rgb 'red' linewidth 2 title 'Lagrange Polynomial', \\\n";
    gpScript << "     '-' using 1:2 with points pt 5 ps 2 linecolor rgb 'green' "
             << "title sprintf('Query Point (%.2f, %.2f)'," << queryPoint << "," 
             << interpolatedValue << ")\n";
    gpScript << queryPoint << " " << interpolatedValue << "\n";
    gpScript << "e\n\n";
    
    gpScript << "pause -1 'Press Enter to close...'\n";
    
    gpScript.close();
    
    cout << "\n✓ GnuPlot script generated: Output/lagrange_plot.gnu\n";
    cout << "✓ Data files generated:\n";
    cout << "  - Output/weather_data.txt\n";
    cout << "  - Output/interpolation_curve.txt\n";
}

// Generate basis polynomial plot
void GnuplotHelper::generateBasisPolynomialPlot(
    const vector<double>& xData,
    const vector<double>& yData,
    const vector<vector<double>>& allBasisPolynomials,
    double queryPoint)
{
    // This is a placeholder for basis polynomial visualization
    ofstream gpScript(outputDir + "/basis_polynomials.gnu");
    
    gpScript << "#!/usr/bin/gnuplot\n\n";
    gpScript << "set terminal png size 1200,800\n";
    gpScript << "set output 'Output/basis_polynomials.png'\n\n";
    
    gpScript << "set title 'Lagrange Basis Polynomials' font 'arial,14'\n";
    gpScript << "set xlabel 'Time (hours)' font 'arial,12'\n";
    gpScript << "set ylabel 'L_i(x) value' font 'arial,12'\n";
    gpScript << "set grid\n";
    gpScript << "set key outside right\n\n";
    
    gpScript << "plot 'Output/weather_data.txt' using 1:2 with points "
             << "pt 7 ps 1.2 linecolor rgb 'black' title 'Data Points'\n";
    
    gpScript.close();
    
    cout << "✓ Basis polynomial script generated: Output/basis_polynomials.gnu\n";
}

// Generate heatmap visualization
void GnuplotHelper::generateHeatmap(
    const vector<double>& xData,
    const vector<double>& yData,
    double queryPoint,
    double interpolatedValue)
{
    // Create heatmap data file
    ofstream heatmapData(outputDir + "/heatmap_data.txt");
    
    if (!heatmapData.is_open())
    {
        cerr << "Error: Cannot write heatmap data." << endl;
        return;
    }
    
    // Create 2D grid for heatmap (time vs index with temperature as color)
    int numIntervals = 20;  // Number of time intervals
    double xMin = xData[0];
    double xMax = xData[xData.size() - 1];
    
    for (int i = 0; i < (int)xData.size(); i++)
    {
        heatmapData << fixed << setprecision(2) 
                    << xData[i] << " " << i << " " << yData[i] << "\n";
    }
    
    heatmapData.close();
    
    // Generate GnuPlot heatmap script
    ofstream gpHeatmap(outputDir + "/heatmap.gnu");
    
    gpHeatmap << "#!/usr/bin/gnuplot\n\n";
    gpHeatmap << "set terminal png size 1400,700\n";
    gpHeatmap << "set output 'heatmap_temperature.png'\n\n";
    
    gpHeatmap << "set title 'Temperature Heatmap - Weather Station Data' font 'arial,14'\n";
    gpHeatmap << "set xlabel 'Time (hours)' font 'arial,12'\n";
    gpHeatmap << "set ylabel 'Data Point Index' font 'arial,12'\n";
    gpHeatmap << "set cblabel 'Temperature (°C)' font 'arial,11'\n\n";
    
    gpHeatmap << "set palette defined (0 'blue', 15 'cyan', 20 'green', 25 'yellow', 32 'red')\n";
    gpHeatmap << "set cbrange [19:33]\n";
    gpHeatmap << "set grid\n\n";
    
    gpHeatmap << "plot 'heatmap_data.txt' using 1:2:3 with points pt 5 ps 3 "
              << "palette title 'Temperature', \\\n";
    gpHeatmap << "     '-' using 1:2 with points pt 7 ps 2.5 linecolor rgb 'black' "
              << "title sprintf('Query: (%.2f h, %.1f°C)'," << queryPoint << "," 
              << interpolatedValue << ")\n";
    gpHeatmap << queryPoint << " " << (xData.size() / 2) << "\n";
    gpHeatmap << "e\n\n";
    
    gpHeatmap << "pause -1\n";
    
    gpHeatmap.close();
    
    cout << "✓ Heatmap script generated: Output/heatmap.gnu\n";
}

