#ifndef GNUPLOT_HELPER_HPP
#define GNUPLOT_HELPER_HPP

#include <vector>
#include <string>

class GnuplotHelper
{
private:
    std::string outputDir;
    
public:
    // Constructor
    GnuplotHelper(const std::string& outDir = "Output");
    
    // Destructor
    ~GnuplotHelper();
    
    // Generate GnuPlot script for visualization
    void generateLagrangeVisualization(
        const std::vector<double>& xData,
        const std::vector<double>& yData,
        const std::vector<double>& basisValues,
        double queryPoint,
        double interpolatedValue);
    
    // Create basis polynomial plot
    void generateBasisPolynomialPlot(
        const std::vector<double>& xData,
        const std::vector<double>& yData,
        const std::vector<std::vector<double>>& allBasisPolynomials,
        double queryPoint);
    
    // Generate heatmap visualization
    void generateHeatmap(
        const std::vector<double>& xData,
        const std::vector<double>& yData,
        double queryPoint,
        double interpolatedValue);
    
    // Write data file for plotting
    bool writeDataFile(const std::string& filename,
                      const std::vector<double>& xData,
                      const std::vector<double>& yData);
};

#endif
