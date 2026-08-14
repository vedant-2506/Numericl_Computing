# Lagrange Interpolation - Weather Station Analysis

## Overview
This C++ project uses **Lagrange polynomial interpolation** to estimate temperature at unequal time intervals based on weather station data. The program reads data from a file, performs interpolation calculations, and generates visualizations using GnuPlot.

## Project Structure
```
Lab-test/
├── Include/              # Header files (.hpp)
│   ├── Lagrange.hpp
│   ├── WeatherDataHandler.hpp
│   └── GnuplotHelper.hpp
├── Src/                 # Implementation files (.cpp)
│   ├── Lagrange.cpp
│   ├── WeatherDataHandler.cpp
│   └── GnuplotHelper.cpp
├── Input/               # Input data files
│   └── weather_data.txt
├── Output/              # Output results and graphs
├── main.cpp             # Main program
├── Makefile            # Build configuration
└── README.md           # This file
```

## Requirements
- C++17 or later
- GCC or Clang compiler
- GnuPlot (for visualization)
- Standard Linux utilities (make)

## Features

### 1. **Object-Oriented Design**
   - `Lagrange` class: Core interpolation functionality
   - `WeatherDataHandler` class: File I/O operations
   - `GnuplotHelper` class: Visualization generation

### 2. **File Input/Output**
   - **Input**: `Input/weather_data.txt` - Weather station data
   - **Output Files**:
     - `lagrange_detailed_results.txt` - Detailed calculations and basis polynomials
     - `interpolation_summary.txt` - Summary report
     - `weather_data.txt` - Original data (for plotting)
     - `interpolation_curve.txt` - Polynomial curve points
     - `lagrange_plot.gnu` - GnuPlot script
     - `lagrange_interpolation.png` - Generated plot

### 3. **Calculation Output**
   - All L_0, L_1, L_2, ... L_n basis polynomial values
   - Individual contributions (y_i * L_i(x))
   - Sum verification (Σ L_i = 1.0)
   - Final interpolated temperature value

## Compilation

### Using Make
```bash
cd Lab-test
make              # Compile the project
make run          # Compile and run
make clean        # Remove compiled files
make help         # Show help
```

### Manual Compilation
```bash
g++ -std=c++17 -I Include -o lagrange_interpolation \
    Src/Lagrange.cpp \
    Src/WeatherDataHandler.cpp \
    Src/GnuplotHelper.cpp \
    main.cpp
```

## Usage

### Step 1: Run the Program
```bash
./lagrange_interpolation
```

### Step 2: Enter Query Point
When prompted, enter the time (in hours) where you want to estimate temperature:
- Example: `26.00` for 2:00 AM (if starting from 18.00 = 6 PM)
- The program expects decimal hours

### Step 3: View Results
The program outputs:
- Console: Summary of interpolated temperature
- `Output/lagrange_detailed_results.txt`: Complete calculation details
- `Output/interpolation_summary.txt`: Summary report

### Step 4: Generate Graph
```bash
cd Output
gnuplot lagrange_plot.gnu
# Or manually in GnuPlot:
# gnuplot> load 'lagrange_plot.gnu'
```

This generates `lagrange_interpolation.png` showing:
- Original data points (blue)
- Lagrange polynomial curve (red)
- Query point (green)

## Mathematical Background

### Lagrange Interpolation Formula
For n data points (x_i, y_i), the interpolating polynomial is:

```
P(x) = Σ y_i * L_i(x)    where i = 0 to n-1
```

### Basis Polynomials
Each basis polynomial L_i(x) is:

```
L_i(x) = Π (x - x_j) / (x_i - x_j)    where j ≠ i
```

### Key Properties
1. L_i(x_i) = 1 and L_i(x_j) = 0 for j ≠ i
2. Σ L_i(x) = 1 for all x (partition of unity)

## Example

### Input
```
Time (Hour): 26.00
```

### Output
```
Estimated Temperature at 2:00 AM (26.00 hours): 21.875 °C
```

### Basis Polynomials at x = 26.00
```
Index | L_i(x) Value
------|-------------------
 0    | 0.0000000001
 1    | 0.0000000003
 ...
25    | 0.4523456789
26    | 0.3217654321
...
49    | 0.0000000002
```

Sum of all L_i = 1.0 ✓

## Data Format

The `weather_data.txt` file format:
```
<number_of_points>
<time_1> <temperature_1>
<time_2> <temperature_2>
...
<time_n> <temperature_n>
```

Example:
```
50
18.00 32.4
18.18 32.1
...
31.00 19.6
```

## Error Handling

The program checks for:
- ✓ File not found
- ✓ Invalid input data
- ✓ Duplicate x-values (not allowed for Lagrange)
- ✓ Invalid query points
- ✓ Division by zero
- ✓ Output file creation errors

## Notes

1. **Data Points**: More data points increase polynomial degree but may cause oscillations (Runge's phenomenon)
2. **Accuracy**: Better for points within the data range (interpolation) rather than outside (extrapolation)
3. **Computational Cost**: O(n²) for basis polynomial evaluation
4. **Precision**: Results shown with 6 decimal places

## Time Interpretation

In the weather dataset:
- 18.00 = 6:00 PM
- 24.00 = Midnight (next day)
- 26.00 = 2:00 AM
- 31.00 = 7:00 AM

## Author Notes

This implementation demonstrates:
- Proper OOP design with separated concerns
- Exception handling and error checking
- File I/O operations
- Scientific computation
- Data visualization integration

## License

Educational use - University Numerical Computing Lab

---

**Questions?** Check the detailed results in `Output/lagrange_detailed_results.txt`
