set xlabel 'Число потоков'
set term png
set output '4laba.png'
set ylabel 'Результат'

plot 'v1.dat' using 1:($2) with linespoints title 'одна критическая секция', \
     'atom.dat' using 1:($2) with linespoints title 'атом',\
     'nblock.dat' using 1:($2) with linespoints title 'н блок',\
     'izbvich.dat' using 1:($2) with linespoints title 'избыт выч',\
     'vector.dat' using 1:($2) with linespoints title 'вектор',\
     'line.dat' using 1:($2) with linespoints title 'линейное'