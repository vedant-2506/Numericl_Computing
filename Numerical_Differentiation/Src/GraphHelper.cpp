#include "../Include/GraphHelper.hpp"

#include <algorithm>
#include <cmath>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <filesystem>
#include <sys/stat.h>

using namespace std;
namespace fs = std::filesystem;

GraphHelper::GraphHelper() {}

// Creates the directory for output artifacts if it does not already exist.
bool GraphHelper::ensureDirectory(const string &directory) const
{
    if (mkdir(directory.c_str(), 0777) == 0)
        return true;

    if (errno == EEXIST)
        return true;

    throw runtime_error("Cannot create directory: " + directory);
}

// Removes outdated temp files before generating new graphs.
void GraphHelper::cleanupLegacyArtifacts() const
{
    error_code ignored;
    if (!fs::exists("Graphs", ignored))
        return;

    for (const fs::directory_entry &entry : fs::directory_iterator("Graphs", ignored))
    {
        if (!entry.is_regular_file())
            continue;

        const string extension = entry.path().extension().string();
        if (extension == ".gnu" || extension == ".dat")
            fs::remove(entry.path(), ignored);
    }
}

// Transforms h and calculated absolute error pairs using base-10 logarithms and writes log10(h) vs log10(Absolute Error) to data file.
void GraphHelper::writeDataFile(const string &dataFile,
                                const FunctionResult &functionResult) const
{
    ofstream out(dataFile);
    if (!out)
        throw runtime_error("Cannot create graph data file: " + dataFile);

    out << scientific << setprecision(12);
    out << "# log10(h) log10(AbsoluteError)\n";
    for (const DifferentiationSample &sample : functionResult.samples)
    {
        if (sample.h <= 0.0)
        {
            throw invalid_argument("h must be positive for logarithmic plotting.");
        }

        if (sample.absoluteError <= 0.0)
        {
            cerr << "[WARNING] Skipping non-positive absolute error (" << sample.absoluteError
                 << ") for h = " << sample.h << " in function " << functionResult.spec.name << '\n';
            continue;
        }

        const double logH = std::log10(sample.h);
        const double logError = std::log10(std::abs(sample.absoluteError));

        out << logH << ' ' << logError << '\n';
    }
}

// Writes gnuplot script for already-transformed log10 data without logscale settings.
void GraphHelper::writeScriptFile(const string &scriptFile,
                                  const string &dataFile,
                                  const string &pngFile,
                                  const FunctionResult &functionResult,
                                  const string &methodName) const
{
    ofstream out(scriptFile);
    if (!out)
        throw runtime_error("Cannot create gnuplot script: " + scriptFile);

    out << "set terminal pngcairo size 900,600 enhanced font 'Helvetica,13'\n";
    out << "set output '" << pngFile << "'\n";
    out << "set title '" << methodName << " - " << functionResult.spec.name << "' font 'Helvetica,15'\n";
    out << "set xlabel 'log10(h)'\n";
    out << "set ylabel 'log10(Absolute Error)'\n";
    out << "set xrange [-0.5:-10.5]\n";
    out << "set grid lc rgb '#cccccc' lw 1\n";
    out << "set key off\n";
    out << "plot '" << dataFile << "' using 1:2 with linespoints pt 7 ps 1.2 lw 2 lc rgb '#2980b9' title 'Absolute Error'\n";
}

// Creates data file, gnuplot script, and PNG plot for one test function.
void GraphHelper::generateSingleGraph(const string &methodPrefix,
                                      const string &methodName,
                                      const FunctionResult &functionResult) const
{
    string dataFile = "Output/" + methodPrefix + "_" + functionResult.spec.graphStem + ".dat";
    string scriptFile = "Output/" + methodPrefix + "_" + functionResult.spec.graphStem + ".gnu";
    string pngFile = "Graphs/" + methodPrefix + "_" + functionResult.spec.graphStem + ".png";

    writeDataFile(dataFile, functionResult);
    writeScriptFile(scriptFile, dataFile, pngFile, functionResult, methodName);

    int status = system(("gnuplot \"" + scriptFile + "\"").c_str());
    remove(scriptFile.c_str());
    if (status != 0)
        throw runtime_error("gnuplot failed for " + pngFile);
}

// Generates log-transformed error-vs-h plots for all test functions using gnuplot.
void GraphHelper::generateGraphs(const MethodResult &result) const
{
    ensureDirectory("Output");
    ensureDirectory("Graphs");
    cleanupLegacyArtifacts();

    for (const FunctionResult &functionResult : result.functionResults)
    {
        try
        {
            generateSingleGraph(result.graphPrefix, result.methodName, functionResult);
            cout << "[OUTPUT] Graph -> Graphs/" << result.graphPrefix << '_' << functionResult.spec.graphStem << ".png" << endl;
        }
        catch (const exception &e)
        {
            cout << "Graph generation error for " << functionResult.spec.name << ": " << e.what() << endl;
        }
    }
}

void GraphHelper::writeInterpolationData(const InterpolationResult &result,
                                         const string &curveFile,
                                         const string &pointsFile,
                                         const string &queriesFile,
                                         double &minimumX,
                                         double &maximumX) const
{
    if (result.xValues.empty() || result.xValues.size() != result.yValues.size())
        throw invalid_argument("Interpolation graph requires matching input data.");

    minimumX = *min_element(result.xValues.begin(), result.xValues.end());
    maximumX = *max_element(result.xValues.begin(), result.xValues.end());
    for (const InterpolationQuery &query : result.queries)
    {
        minimumX = min(minimumX, query.x);
        maximumX = max(maximumX, query.x);
    }

    const double span = maximumX - minimumX;
    const double margin = span > 0.0 ? span * 0.1 : 1.0;
    minimumX -= margin;
    maximumX += margin;

    auto evaluatePolynomial = [&result](double x) {
        double value = 0.0;
        for (auto coefficient = result.polynomialCoefficients.rbegin();
             coefficient != result.polynomialCoefficients.rend(); ++coefficient)
            value = value * x + *coefficient;
        return value;
    };

    ofstream curve(curveFile);
    ofstream points(pointsFile);
    ofstream queries(queriesFile);
    if (!curve || !points || !queries)
        throw runtime_error("Cannot create interpolation graph data files.");

    curve << scientific << setprecision(12);
    points << scientific << setprecision(12);
    queries << scientific << setprecision(12);

    const size_t sampleCount = 500;
    for (size_t sample = 0; sample <= sampleCount; ++sample)
    {
        const double x = minimumX + (maximumX - minimumX) * static_cast<double>(sample) / sampleCount;
        curve << x << ' ' << evaluatePolynomial(x) << '\n';
    }
    for (size_t index = 0; index < result.xValues.size(); ++index)
        points << result.xValues[index] << ' ' << result.yValues[index] << '\n';
    for (const InterpolationQuery &query : result.queries)
        queries << query.x << ' ' << query.y << '\n';
}

void GraphHelper::writeInterpolationScript(const InterpolationResult &result,
                                           const string &scriptFile,
                                           const string &curveFile,
                                           const string &pointsFile,
                                           const string &queriesFile,
                                           double minimumX,
                                           double maximumX) const
{
    ofstream out(scriptFile);
    if (!out)
        throw runtime_error("Cannot create graph script: " + scriptFile);

    out << "set terminal pngcairo size 1000,650 enhanced font 'Helvetica,13'\n";
    out << "set output '" << result.graphFile << "'\n";
    out << "set title '" << result.methodName << "' font 'Helvetica,16'\n";
    out << "set xlabel 'x'\n";
    out << "set ylabel 'f(x)'\n";
    out << "set xrange [" << minimumX << ":" << maximumX << "]\n";
    out << "set grid lc rgb '#cccccc' lw 1\n";
    out << "set key outside\n";
    out << "plot '" << curveFile << "' using 1:2 with lines lw 2 lc rgb '#1f77b4' title 'Interpolation Polynomial', "
        << "'" << pointsFile << "' using 1:2 with points pt 7 ps 1.5 lc rgb '#222222' title 'Input Data Points', "
        << "'" << queriesFile << "' using 1:2 with points pt 9 ps 1.8 lc rgb '#d62728' title 'Queried Point'\n";
}

void GraphHelper::generateInterpolationGraph(const InterpolationResult &result) const
{
    ensureDirectory("Output");
    ensureDirectory("Graphs");

    const string stem = result.methodName == "Lagrange Interpolation"
                            ? "LagrangeInterpolation"
                            : "NewtonInterpolation";
    const string curveFile = "Output/" + stem + "_curve.dat";
    const string pointsFile = "Output/" + stem + "_points.dat";
    const string queriesFile = "Output/" + stem + "_queries.dat";
    const string scriptFile = "Output/" + stem + ".gnu";

    double minimumX = 0.0;
    double maximumX = 0.0;
    writeInterpolationData(result, curveFile, pointsFile, queriesFile, minimumX, maximumX);
    writeInterpolationScript(result, scriptFile, curveFile, pointsFile, queriesFile, minimumX, maximumX);

    const int status = system(("gnuplot \"" + scriptFile + "\"").c_str());
    remove(scriptFile.c_str());
    remove(curveFile.c_str());
    remove(pointsFile.c_str());
    remove(queriesFile.c_str());
    if (status != 0)
        throw runtime_error("gnuplot failed for " + result.graphFile);

    cout << "[OUTPUT] Graph -> " << result.graphFile << endl;
}

