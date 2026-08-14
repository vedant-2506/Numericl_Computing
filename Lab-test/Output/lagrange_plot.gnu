#!/usr/bin/gnuplot

set terminal png size 1200,700
set output 'lagrange_interpolation.png'

set title 'Weather Station Temperature - Lagrange Interpolation' font 'arial,14'
set xlabel 'Time (hours)' font 'arial,12'
set ylabel 'Temperature (°C)' font 'arial,12'
set grid
set style data points
set pointsize 1.5

# Plot original data and interpolation curve
plot 'weather_data.txt' using 1:2 with points pt 7 ps 1.2 linecolor rgb 'blue' title 'Original Data', \
     'interpolation_curve.txt' using 1:2 with lines linecolor rgb 'red' linewidth 2 title 'Lagrange Polynomial', \
     '-' using 1:2 with points pt 5 ps 2 linecolor rgb 'green' title sprintf('Query Point (%.2f, %.2f)',26,4.35678e+10)
26 4.35678e+10
e
