set title 'X vs Y Solution Graph'
set xlabel 'X (Index)'
set ylabel 'Y (Solution Value)'
set grid
plot 'resultvector.txt' using 1:2 with linespoints title 'X vs Y'
pause -1
