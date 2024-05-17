set xlabel 'Число потоков'
set term png
set output 'quicksort_tasks.png'
set ylabel 'Результат'

plot 'data.dat' using 1:($2) with linespoints title 'quicksort_tasks', \
     'line.dat' using 1:($2) with linespoints title 'линейное'