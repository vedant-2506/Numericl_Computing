set terminal png size 800,600
set output 'Output/gaussian_pivot_graph.png'
set title 'Gaussian Elimination WITH Pivoting'
set xlabel 'Variable Index'
set ylabel 'Value'
set grid
plot 'Output/gaussian_pivot_plot.dat' with linespoints lw 2 pt 7 title 'x'
