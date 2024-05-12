set terminal pdf color enhanced font 'Calibri,16' size 15cm,10cm
set output 'graph.pdf'
set key inside left top font 'Calibri,16'
set colorsequence podo

set xlabel "Threads" font 'Calibri,16'
set ylabel "Acceleration" font 'Calibri,16'

set term png
set output 'graph.png'

plot 'v1.dat' using 1:($2) with linespoints title 'v1', \
     'v2.dat' using 1:($2) with linespoints title 'v2',\
     'v3.dat' using 1:($2) with linespoints title 'v3',\
     'v4.dat' using 1:($2) with linespoints title 'v4',\
     'v5.dat' using 1:($2) with linespoints title 'v5',\
     'line.dat' using 1:($2) with linespoints title 'Linear Acceleration',\