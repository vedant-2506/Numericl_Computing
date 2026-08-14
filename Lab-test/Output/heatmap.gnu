#!/usr/bin/gnuplot

set terminal png size 1400,700
set output 'heatmap_temperature.png'

set title 'Temperature Heatmap - Weather Station Data' font 'arial,14'
set xlabel 'Time (hours)' font 'arial,12'
set ylabel 'Data Point Index' font 'arial,12'
set cblabel 'Temperature (°C)' font 'arial,11'

set palette defined (0 'blue', 15 'cyan', 20 'green', 25 'yellow', 32 'red')
set cbrange [19:33]
set grid

plot 'heatmap_data.txt' using 1:2:3 with points pt 5 ps 3 palette title 'Temperature', \
     '-' using 1:2 with points pt 7 ps 2.5 linecolor rgb 'black' title sprintf('Query: (%.2f h, %.1f°C)',26,4.35678e+10)
26 11
e

pause -1
