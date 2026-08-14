set terminal pngcairo size 900,600 enhanced font 'Helvetica,13'
set output 'Output/least_square_parabola.png'
set title 'Least Squares Polynomial Fit (degree 2)' font 'Helvetica,15'
set xlabel 'x'
set ylabel 'y'
set grid lc rgb '#cccccc' lw 1
set key top left
set style data points
set xrange [-0.2:2.2]
f(x) = 1.000000 + 1.000000*x + 1.000000*x**2

plot 'Output/least_square_parabola_data.dat' using 1:2 with points pt 7 ps 1.5 lc rgb '#e74c3c' title 'Data Points', \
     f(x) with lines lw 2.5 lc rgb '#2980b9' title 'Fitted Curve (degree 2)'
