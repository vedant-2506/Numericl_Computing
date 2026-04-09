#ifndef INTERPOLATION_HPP       // Guard against double inclusion
#define INTERPOLATION_HPP

#include "Matrix.hpp"                 // Base matrix storage
#include <fstream>                    // File input/output

class Interpolation : public Matrix   // Base class for interpolation methods
{
public:
    Interpolation();                  // Default constructor
    Interpolation(const Matrix &m);   // Constructor from data points matrix

    void loadDataPoints(std::ifstream &fin);             // Read (x,y) pairs from file
    void printDataPoints(std::ofstream &fout) const;     // Display data points
    void validateDataPoints() const;                     // Check data meets interpolation requirements

    virtual double interpolate(double xQuery, std::ofstream &fout) = 0;  // Pure virtual, implement in derived
    virtual ~Interpolation() {}       // Virtual destructor for polymorphism
};

#endif
