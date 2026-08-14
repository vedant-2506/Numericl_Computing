#include "Include/Lagrange.hpp"
#include "Include/WeatherDataHandler.hpp"
#include "Include/GnuplotHelper.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        string inputFile = (argc > 1) ? argv[1] : "Input/weather_data.txt";

        // Display banner
        cout << "\n";
        cout << "╔════════════════════════════════════════════════════════════╗\n";
        cout << "║    LAGRANGE INTERPOLATION - WEATHER STATION ANALYSIS       ║\n";
        cout << "║  Estimating Temperature at Unequal Time Intervals          ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n";
        
        // 1. Load data from file
        WeatherDataHandler dataHandler;
        vector<double> xData, yData;
        
        if (!dataHandler.readDataFromFile(inputFile, xData, yData))
        {
            cerr << "Fatal error: Could not load data file." << endl;
            return 1;
        }
        
        // 2. Display loaded data
        dataHandler.printDataToConsole(xData, yData);
        
        // 3. Create Lagrange interpolator
        Lagrange lagrange(xData, yData);
        
        // 4. Get query point from user
        double queryPoint = dataHandler.getQueryPointFromUser();
        
        if (queryPoint < 0)
        {
            cerr << "Error: Invalid query point." << endl;
            return 1;
        }
        
        // 5. Perform interpolation with detailed output
        ofstream outputFile("Output/lagrange_detailed_results.txt");
        
        if (!outputFile.is_open())
        {
            cerr << "Error: Cannot create output file." << endl;
            return 1;
        }
        
        cout << "\n⏳ Performing Lagrange interpolation...\n";
        
        double interpolatedValue = lagrange.interpolate(queryPoint, outputFile);
        
        outputFile.close();
        
        cout << "✓ Interpolation completed successfully!\n";
        
        // 6. Display results in console
        cout << "\n========================================\n";
        cout << "           INTERPOLATION RESULT\n";
        cout << "========================================\n";
        cout << "Time Query Point: " << fixed << setprecision(2) << queryPoint << " hours\n";
        cout << "Estimated Temperature: " << fixed << setprecision(6) 
             << interpolatedValue << " °C\n";
        cout << "========================================\n";
        
        // 7. Generate visualization
        GnuplotHelper gnuplotGen("Output");
        
        vector<double> basisValues = lagrange.allBasisValues(queryPoint);
        
        cout << "\n⏳ Generating visualization files...\n";
        gnuplotGen.generateLagrangeVisualization(xData, yData, basisValues, 
                                               queryPoint, interpolatedValue);
        
        // Generate heatmap
        gnuplotGen.generateHeatmap(xData, yData, queryPoint, interpolatedValue);
        
        // 8. Save summary to file
        ofstream summaryFile("Output/interpolation_summary.txt");
        
        summaryFile << "╔════════════════════════════════════════════════════════════╗\n";
        summaryFile << "║        LAGRANGE INTERPOLATION SUMMARY REPORT               ║\n";
        summaryFile << "╚════════════════════════════════════════════════════════════╝\n\n";
        
        summaryFile << "DATA INFORMATION:\n";
        summaryFile << "  Total Data Points: " << xData.size() << "\n";
        summaryFile << "  Time Range: " << fixed << setprecision(2) 
                    << xData[0] << " to " << xData[xData.size()-1] << " hours\n";
        summaryFile << "  Temperature Range: " << setprecision(1) 
                    << *min_element(yData.begin(), yData.end()) << " to " 
                    << *max_element(yData.begin(), yData.end()) << " °C\n\n";
        
        summaryFile << "INTERPOLATION REQUEST:\n";
        summaryFile << "  Query Point: " << fixed << setprecision(2) << queryPoint << " hours\n";
        summaryFile << "  Interpolation Method: Lagrange Polynomial\n";
        summaryFile << "  Polynomial Degree: " << (xData.size() - 1) << "\n\n";
        
        summaryFile << "RESULT:\n";
        summaryFile << "  Estimated Temperature: " << fixed << setprecision(6) 
                    << interpolatedValue << " °C\n\n";
        
        summaryFile << "BASIS POLYNOMIAL VALUES AT x = " << setprecision(2) 
                    << queryPoint << ":\n";
        summaryFile << "Index | L_i(x) Value\n";
        summaryFile << "------|-------------------\n";
        for (size_t i = 0; i < basisValues.size(); i++)
        {
            summaryFile << setw(5) << i << " | " 
                       << setw(17) << setprecision(10) << basisValues[i] << "\n";
        }
        
        summaryFile << "\nVerification: Sum of all L_i = " << setprecision(10);
        double sumL = 0.0;
        for (double val : basisValues)
            sumL += val;
        summaryFile << sumL << " (should be 1.0)\n";
        
        summaryFile << "\nOUTPUT FILES GENERATED:\n";
        summaryFile << "  - lagrange_detailed_results.txt (detailed calculation)\n";
        summaryFile << "  - interpolation_summary.txt (this file)\n";
        summaryFile << "  - weather_data.txt (original data)\n";
        summaryFile << "  - interpolation_curve.txt (polynomial curve)\n";
        summaryFile << "  - lagrange_plot.gnu (GnuPlot visualization script)\n";
        summaryFile << "  - lagrange_interpolation.png (generated plot)\n";
        
        summaryFile.close();
        
        // 9. Final message
        cout << "\n✓ All output files generated in the Output/ directory:\n";
        cout << "  - lagrange_detailed_results.txt\n";
        cout << "  - interpolation_summary.txt\n";
        cout << "  - lagrange_plot.gnu\n";
        cout << "  - heatmap.gnu\n";
        cout << "  - Data files for visualization\n";
        
        cout << "\nTo generate the graphs, run:\n";
        cout << "  cd Output && gnuplot lagrange_plot.gnu\n";
        cout << "  cd Output && gnuplot heatmap.gnu\n\n";
        
        cout << "╔════════════════════════════════════════════════════════════╗\n";
        cout << "║              Analysis Complete - Success!                  ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n\n";
        
        return 0;
    }
    catch (const exception& e)
    {
        cerr << "\n❌ Error: " << e.what() << endl;
        return 1;
    }
}
