#ifndef GRAPH_HELPER_HPP
#define GRAPH_HELPER_HPP

#include "NumericalDifferentiation.hpp"
#include "InterpolationData.hpp"
#include <string>

// Separates calculation data from graph generation to maintain single responsibility.
class GraphHelper
{
public:
    GraphHelper();

    // Generates log-transformed error-vs-h plots for all test functions using gnuplot.
    void generateGraphs(const MethodResult &result) const;

    // Generates a polynomial curve with original and queried interpolation points.
    void generateInterpolationGraph(const InterpolationResult &result) const;

private:
    // Creates the directory for output artifacts if it does not already exist.
    bool ensureDirectory(const std::string &directory) const;

    // Removes outdated temp files before generating new graphs.
    void cleanupLegacyArtifacts() const;

    // Creates data file, gnuplot script, and PNG plot for one test function.
    void generateSingleGraph(const std::string &methodPrefix,
                             const std::string &methodName,
                             const FunctionResult &functionResult) const;

    // Transforms h and calculated absolute error pairs using base-10 logarithms and writes log10(h) vs log10(Absolute Error) to data file.
    void writeDataFile(const std::string &dataFile,
                       const FunctionResult &functionResult) const;

    // Writes gnuplot script for already-transformed log10 data without logscale settings.
    void writeScriptFile(const std::string &scriptFile,
                         const std::string &dataFile,
                         const std::string &pngFile,
                         const FunctionResult &functionResult,
                         const std::string &methodName) const;

    void writeInterpolationData(const InterpolationResult &result,
                                const std::string &curveFile,
                                const std::string &pointsFile,
                                const std::string &queriesFile,
                                double &minimumX,
                                double &maximumX) const;

    void writeInterpolationScript(const InterpolationResult &result,
                                  const std::string &scriptFile,
                                  const std::string &curveFile,
                                  const std::string &pointsFile,
                                  const std::string &queriesFile,
                                  double minimumX,
                                  double maximumX) const;
};

#endif