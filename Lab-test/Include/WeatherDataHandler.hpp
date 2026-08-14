#ifndef WEATHER_DATA_HANDLER_HPP
#define WEATHER_DATA_HANDLER_HPP

#include <vector>
#include <string>

class WeatherDataHandler
{
public:
    // Constructor
    WeatherDataHandler();

    // File I/O methods
    bool readDataFromFile(const std::string& filename, 
                         std::vector<double>& xData, 
                         std::vector<double>& yData);
    
    void printDataToConsole(const std::vector<double>& xData, 
                           const std::vector<double>& yData);
    
    // Getters for query point
    double getQueryPointFromUser();
    
    // Destructor
    ~WeatherDataHandler();
};

#endif
