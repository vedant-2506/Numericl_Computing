#include "../Include/GraphHelper.hpp"
#include "../Include/TrapezoidalRule.hpp"
#include "../Include/Simpsons13Rule.hpp"
#include "../Include/Simpsons38Rule.hpp"

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

void GraphHelper::generatePowerMethodGraph(const PowerMethodResult &result) const
{
    ensureDirectory("Graphs");
    const string dataFile = "Output/power_method_" + fs::path(result.inputFile).stem().string() + ".dat";
    const string scriptFile = "Output/power_method_" + fs::path(result.inputFile).stem().string() + ".gnu";
    ofstream data(dataFile);
    ofstream script(scriptFile);
    if (!data || !script)
        throw runtime_error("Cannot create Power Method graph files.");

    data << scientific << setprecision(12);
    for (const PowerIteration &sample : result.iterations)
        data << sample.iteration << ' ' << sample.eigenvalue << '\n';
    data.close();

    script << "set terminal pngcairo size 1000,650 enhanced font 'Helvetica,13'\n"
           << "set output '" << result.graphFile << "'\n"
           << "set title 'Power Method - Eigenvalue Convergence' font 'Helvetica,15'\n"
           << "set xlabel 'Iteration'\n"
           << "set ylabel 'Estimated Eigenvalue (lambda_k)'\n"
           << "set grid\n"
           << "set key outside\n"
           << "plot '" << dataFile << "' using 1:2 with linespoints pt 7 ps 1.0 lw 2 "
              "title 'Estimated Eigenvalue'\n";
    script.close();

    const int status = system(("gnuplot \"" + scriptFile + "\"").c_str());
    fs::remove(scriptFile);
    if (status != 0 || !fs::exists(result.graphFile))
        throw runtime_error("gnuplot failed for " + result.graphFile);
    cout << "[OUTPUT] Graph -> " << result.graphFile << endl;
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

void GraphHelper::generateTrapezoidalGraph(const TrapezoidalResult &result) const
{
    ensureDirectory("Output");
    ensureDirectory("Graphs");

    const string scriptFile = "Output/TrapezoidalRule.gnu";
    ofstream script(scriptFile);
    if (!script)
        throw runtime_error("Cannot create graph script: " + scriptFile);

    script << "set terminal pngcairo size 1000,900 enhanced font 'Helvetica,12'\n"
           << "set output '" << result.graphFile << "'\n"
           << "set multiplot layout " << result.functionResults.size() << ",1 title 'Trapezoidal Rule'\n"
           << "set xlabel 'x'\nset ylabel 'f(x)'\nset xrange [0:1]\n"
           << "set grid lc rgb '#cccccc' lw 1\nset key outside\n";

    vector<string> dataFiles;
    vector<string> curveFiles;
    for (size_t functionIndex = 0; functionIndex < result.functionResults.size(); ++functionIndex)
    {
        const IntegrationFunctionResult &functionResult = result.functionResults[functionIndex];
        const IntegrationFunctionSpec &spec = functionResult.spec;
        const string dataFile = "Output/TrapezoidalRule_" + to_string(functionIndex) + "_trapezoids.dat";
        const string curveFile = "Output/TrapezoidalRule_" + to_string(functionIndex) + "_curve.dat";
        dataFiles.push_back(dataFile);
        curveFiles.push_back(curveFile);

        ofstream data(dataFile);
        ofstream curve(curveFile);
        if (!data || !curve)
            throw runtime_error("Cannot create trapezoidal graph data files.");

        const int graphIntervals = 16;
        const double h = (spec.upperBound - spec.lowerBound) / graphIntervals;
        data << scientific << setprecision(12);
        curve << scientific << setprecision(12);
        for (int index = 0; index <= 400; ++index)
        {
            const double x = spec.lowerBound + (spec.upperBound - spec.lowerBound) * index / 400.0;
            curve << x << ' ' << spec.function(x) << '\n';
        }
        for (int index = 0; index < graphIntervals; ++index)
        {
            const double leftX = spec.lowerBound + h * index;
            const double rightX = leftX + h;
            data << leftX << ' ' << spec.function(leftX) << '\n'
                 << rightX << ' ' << spec.function(rightX) << '\n'
                 << rightX << " 0\n"
                 << leftX << " 0\n\n";
        }

        script << "set title 'f(x) = " << spec.expression << " on ["
               << spec.lowerBound << ", " << spec.upperBound << "]'\n"
               << "plot '" << dataFile << "' using 1:2 with filledcurves x1 lc rgb '#f4b183' fs transparent solid 0.45 notitle, "
               << "'" << dataFile << "' using 1:2 with lines lc rgb '#d2691e' lw 1 notitle, "
               << "'" << curveFile << "' using 1:2 with lines lc rgb '#1f4e79' lw 2 title 'Original function', "
               << "0 with lines lc rgb '#333333' lw 1 title 'x-axis'\n";
    }

    script << "unset multiplot\n";
    script.close();
    const int status = system(("gnuplot \"" + scriptFile + "\"").c_str());
    remove(scriptFile.c_str());
    for (const string &file : dataFiles)
        remove(file.c_str());
    for (const string &file : curveFiles)
        remove(file.c_str());
    if (status != 0)
        throw runtime_error("gnuplot failed for " + result.graphFile);

    cout << "[OUTPUT] Graph -> " << result.graphFile << endl;
}

void GraphHelper::generateSimpsonsGraph(const string &methodName,
                                        const string &graphFile,
                                        const string &fileStem,
                                        const vector<IntegrationFunctionResult> &functionResults) const
{
    ensureDirectory("Output");
    ensureDirectory("Graphs");

    const string scriptFile = "Output/" + fileStem + ".gnu";
    ofstream script(scriptFile);
    if (!script)
        throw runtime_error("Cannot create graph script: " + scriptFile);

    script << "set terminal pngcairo size 1000,900 enhanced font 'Helvetica,12'\n"
           << "set output '" << graphFile << "'\n"
           << "set multiplot layout " << functionResults.size() << ",1 title \"" << methodName << "\"\n"
           << "set xlabel 'x'\nset ylabel 'f(x)'\nset xrange [0:1]\n"
           << "set grid lc rgb '#cccccc' lw 1\nset key outside\n";

    vector<string> dataFiles;
    vector<string> curveFiles;
    vector<string> pointFiles;
    for (size_t functionIndex = 0; functionIndex < functionResults.size(); ++functionIndex)
    {
        const IntegrationFunctionResult &functionResult = functionResults[functionIndex];
        const IntegrationFunctionSpec &spec = functionResult.spec;
        const int graphIntervals = functionResult.samples.back().intervals;
        const double h = (spec.upperBound - spec.lowerBound) / graphIntervals;
        const string dataFile = "Output/" + fileStem + "_" + to_string(functionIndex) + "_approx.dat";
        const string curveFile = "Output/" + fileStem + "_" + to_string(functionIndex) + "_curve.dat";
        const string pointFile = "Output/" + fileStem + "_" + to_string(functionIndex) + "_points.dat";
        dataFiles.push_back(dataFile);
        curveFiles.push_back(curveFile);
        pointFiles.push_back(pointFile);

        ofstream data(dataFile);
        ofstream curve(curveFile);
        ofstream points(pointFile);
        if (!data || !curve || !points)
            throw runtime_error("Cannot create Simpson graph data files.");

        data << scientific << setprecision(12);
        curve << scientific << setprecision(12);
        points << scientific << setprecision(12);
        for (int index = 0; index <= 400; ++index)
        {
            const double x = spec.lowerBound + (spec.upperBound - spec.lowerBound) * index / 400.0;
            curve << x << ' ' << spec.function(x) << '\n';
        }
        for (int index = 0; index <= graphIntervals; ++index)
        {
            const double x = spec.lowerBound + h * index;
            points << x << ' ' << spec.function(x) << '\n';
        }

        const int groupSize = fileStem == "Simpsons13Rule" ? 2 : 3;
        for (int groupStart = 0; groupStart < graphIntervals; groupStart += groupSize)
        {
            const double x0 = spec.lowerBound + h * groupStart;
            const double x1 = x0 + h;
            const double x2 = x0 + 2.0 * h;
            if (groupSize == 2)
            {
                const double y0 = spec.function(x0);
                const double y1 = spec.function(x1);
                const double y2 = spec.function(x2);
                for (int sample = 0; sample <= 30; ++sample)
                {
                    const double t = static_cast<double>(sample) / 30.0;
                    const double x = x0 + 2.0 * h * t;
                    const double value = y0 * (t - 0.5) * (t - 1.0) * 2.0
                                       + y1 * (-4.0) * t * (t - 1.0)
                                       + y2 * t * (t - 0.5) * 2.0;
                    data << x << ' ' << value << '\n';
                }
            }
            else
            {
                const double x3 = x0 + 3.0 * h;
                const double y0 = spec.function(x0);
                const double y1 = spec.function(x1);
                const double y2 = spec.function(x2);
                const double y3 = spec.function(x3);
                for (int sample = 0; sample <= 30; ++sample)
                {
                    const double t = static_cast<double>(sample) / 30.0;
                    const double x = x0 + 3.0 * h * t;
                    const double value = y0 * (-(t - 1.0) * (t - 2.0) * (t - 3.0) / 6.0)
                                       + y1 * (t * (t - 2.0) * (t - 3.0) / 2.0)
                                       + y2 * (-t * (t - 1.0) * (t - 3.0) / 2.0)
                                       + y3 * (t * (t - 1.0) * (t - 2.0) / 6.0);
                    data << x << ' ' << value << '\n';
                }
            }
            data << '\n';
        }

        script << "set title 'f(x) = " << spec.expression << " on ["
               << spec.lowerBound << ", " << spec.upperBound << "]'\n"
               << "plot '" << dataFile << "' using 1:2 with lines lc rgb '#d2691e' lw 1.5 title 'Simpson approximation', "
               << "'" << curveFile << "' using 1:2 with lines lc rgb '#1f4e79' lw 2 title 'Original function', "
               << "'" << pointFile << "' using 1:2 with points pt 7 ps 0.8 lc rgb '#c00000' title 'Integration points'\n";
    }

    script << "unset multiplot\n";
    script.close();
    const int status = system(("gnuplot \"" + scriptFile + "\"").c_str());
    remove(scriptFile.c_str());
    for (const string &file : dataFiles)
        remove(file.c_str());
    for (const string &file : curveFiles)
        remove(file.c_str());
    for (const string &file : pointFiles)
        remove(file.c_str());
    if (status != 0)
        throw runtime_error("gnuplot failed for " + graphFile);

    cout << "[OUTPUT] Graph -> " << graphFile << endl;
}

void GraphHelper::generateSimpsons13Graph(const Simpsons13Result &result) const
{
    generateSimpsonsGraph(result.methodName, result.graphFile, "Simpsons13Rule", result.functionResults);
}

void GraphHelper::generateSimpsons38Graph(const Simpsons38Result &result) const
{
    generateSimpsonsGraph(result.methodName, result.graphFile, "Simpsons38Rule", result.functionResults);
}

void GraphHelper::generateSimpsonsErrorGraph(
    const string &methodName,
    const string &graphFile,
    const string &fileStem,
    const vector<IntegrationFunctionResult> &functionResults) const
{
    ensureDirectory("Output");
    ensureDirectory("Graphs");

    const string scriptFile = "Output/" + fileStem + "_Error_vs_N.gnu";
    ofstream script(scriptFile);
    if (!script)
        throw runtime_error("Cannot create graph script: " + scriptFile);

    bool useLogScale = false;
    bool allErrorsPositive = true;
    double minimumPositiveError = numeric_limits<double>::max();
    double maximumError = 0.0;
    for (const IntegrationFunctionResult &functionResult : functionResults)
    {
        for (const IntegrationSample &sample : functionResult.samples)
        {
            if (sample.absoluteError > 0.0)
            {
                minimumPositiveError = min(minimumPositiveError, sample.absoluteError);
                maximumError = max(maximumError, sample.absoluteError);
            }
            else
            {
                allErrorsPositive = false;
            }
        }
    }
    if (allErrorsPositive && minimumPositiveError < numeric_limits<double>::max() &&
        maximumError / minimumPositiveError >= 100.0)
        useLogScale = true;

    script << "set terminal pngcairo size 1000,650 enhanced font 'Helvetica,13'\n"
           << "set output '" << graphFile << "'\n"
           << "set title \"" << methodName << " - Absolute Error vs Number of Intervals\" font 'Helvetica,15'\n"
           << "set xlabel 'Number of Intervals (n)'\n"
           << "set ylabel 'Absolute Error'\n"
           << "set grid lc rgb '#cccccc' lw 1\n"
           << "set key outside\n";
    if (useLogScale)
        script << "set logscale y\n";

    vector<string> dataFiles;
    for (size_t functionIndex = 0; functionIndex < functionResults.size(); ++functionIndex)
    {
        const IntegrationFunctionResult &functionResult = functionResults[functionIndex];
        const string dataFile = "Output/" + fileStem + "_" + to_string(functionIndex) + "_error_vs_n.dat";
        dataFiles.push_back(dataFile);

        ofstream data(dataFile);
        if (!data)
            throw runtime_error("Cannot create Simpson error graph data file: " + dataFile);

        data << scientific << setprecision(15);
        data << "# n AbsoluteError\n";
        for (const IntegrationSample &sample : functionResult.samples)
            data << sample.intervals << ' ' << sample.absoluteError << '\n';

        script << (functionIndex == 0 ? "plot " : "     , ")
               << "'" << dataFile << "' using 1:2 with linespoints pt "
               << (functionIndex + 7) << " ps 1.2 lw 2 title '" << functionResult.spec.expression << "'"
               << (functionIndex + 1 == functionResults.size() ? "\n" : " \\\n");
    }

    script.close();
    const int status = system(("gnuplot \"" + scriptFile + "\"").c_str());
    remove(scriptFile.c_str());
    for (const string &file : dataFiles)
        remove(file.c_str());
    if (status != 0)
        throw runtime_error("gnuplot failed for " + graphFile);

    cout << "[OUTPUT] Graph -> " << graphFile << endl;
}

void GraphHelper::generateSimpsons13ErrorGraph(const Simpsons13Result &result) const
{
    generateSimpsonsErrorGraph(result.methodName, result.errorGraphFile,
                                "Simpsons13Rule", result.functionResults);
}

void GraphHelper::generateSimpsons38ErrorGraph(const Simpsons38Result &result) const
{
    generateSimpsonsErrorGraph(result.methodName, result.errorGraphFile,
                                "Simpsons38Rule", result.functionResults);
}
