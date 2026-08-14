#include "../Include/WeatherDataHandler.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cctype>

using namespace std;

namespace
{
    string trim(const string& text)
    {
        size_t start = 0;
        while (start < text.size() && isspace(static_cast<unsigned char>(text[start])))
            ++start;

        size_t end = text.size();
        while (end > start && isspace(static_cast<unsigned char>(text[end - 1])))
            --end;

        return text.substr(start, end - start);
    }

    bool parseDecimalClock(const string& timeText, double& decimalHours)
    {
        size_t colonPos = timeText.find(':');
        if (colonPos == string::npos)
            return false;

        try
        {
            int hours = stoi(timeText.substr(0, colonPos));
            int minutes = stoi(timeText.substr(colonPos + 1));
            decimalHours = static_cast<double>(hours) + static_cast<double>(minutes) / 60.0;
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
}

// Constructor
WeatherDataHandler::WeatherDataHandler() {}

// Destructor
WeatherDataHandler::~WeatherDataHandler() {}

// Read data from file
bool WeatherDataHandler::readDataFromFile(const string& filename, 
                                         vector<double>& xData, 
                                         vector<double>& yData)
{
    ifstream infile(filename);
    
    if (!infile.is_open())
    {
        cerr << "Error: Cannot open file: " << filename << endl;
        return false;
    }
    
    vector<string> lines;
    string line;
    while (getline(infile, line))
    {
        line = trim(line);
        if (!line.empty())
            lines.push_back(line);
    }

    infile.close();

    xData.clear();
    yData.clear();

    if (lines.empty())
    {
        cerr << "Error: Empty data file." << endl;
        return false;
    }

    {
        istringstream headerStream(lines.front());
        int numPoints = 0;
        if ((headerStream >> numPoints) && headerStream.eof() && numPoints > 0)
        {
            bool looksLikeCountFormat = true;

            if (static_cast<size_t>(numPoints + 1) != lines.size())
                looksLikeCountFormat = false;

            if (looksLikeCountFormat)
            {
                for (int i = 0; i < numPoints; ++i)
                {
                    istringstream dataStream(lines[i + 1]);
                    double x = 0.0, y = 0.0;
                    if (!(dataStream >> x >> y) || !dataStream.eof())
                    {
                        looksLikeCountFormat = false;
                        break;
                    }
                    xData.push_back(x);
                    yData.push_back(y);
                }

                if (looksLikeCountFormat)
                {
                    cout << "\n✓ Successfully loaded " << numPoints << " data points from file." << endl;
                    return true;
                }

                xData.clear();
                yData.clear();
            }
        }
    }

    for (const string& record : lines)
    {
        if (record.find('|') == string::npos)
            continue;

        vector<string> columns;
        string column;
        istringstream rowStream(record);
        while (getline(rowStream, column, '|'))
            columns.push_back(trim(column));

        if (columns.size() < 3)
            continue;

        double x = 0.0;
        double y = 0.0;
        if (!parseDecimalClock(columns[1], x))
            continue;

        try
        {
            y = stod(columns[2]);
        }
        catch (...)
        {
            continue;
        }

        xData.push_back(x);
        yData.push_back(y);
    }

    if (xData.empty() || xData.size() != yData.size())
    {
        cerr << "Error: Failed to parse data file: " << filename << endl;
        return false;
    }

    cout << "\n✓ Successfully loaded " << xData.size() << " data points from file." << endl;
    return true;
}

// Print data to console
void WeatherDataHandler::printDataToConsole(const vector<double>& xData, 
                                           const vector<double>& yData)
{
    if (xData.size() != yData.size() || xData.empty())
    {
        cerr << "Error: Invalid data." << endl;
        return;
    }
    
    cout << "\n========================================\n";
    cout << "         WEATHER DATA LOADED\n";
    cout << "========================================\n";
    cout << "Total data points: " << xData.size() << "\n\n";
    cout << left << setw(10) << "Index" << setw(15) << "Time (Hour)" 
         << setw(15) << "Temperature (°C)" << "\n";
    cout << "------|-------------|------------------\n";
    
    for (size_t i = 0; i < xData.size(); i++)
    {
        cout << setw(10) << i 
             << setw(15) << fixed << setprecision(2) << xData[i]
             << setw(15) << fixed << setprecision(1) << yData[i] << "\n";
    }
    cout << "\n";
}

// Get query point from user
double WeatherDataHandler::getQueryPointFromUser()
{
    double queryPoint;
    
    cout << "\n========================================\n";
    cout << "        QUERY POINT SELECTION\n";
    cout << "========================================\n";
    cout << "Note: Time is in decimal hours (e.g., 2.00 AM = 26.00 hours)\n";
    cout << "      or use the exact hour value you want to interpolate.\n";
    cout << "Enter the time (hour) for temperature estimation: ";
    
    cin >> queryPoint;
    
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cerr << "Error: Invalid input. Please enter a numeric value." << endl;
        return -1.0;
    }
    
    return queryPoint;
}
