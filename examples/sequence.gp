reset
set ylabel 'time(sec)'
set xlabel 'size'
set key left top
set title 'time comparison(sequence input)'
set term png enhanced font 'Verdana,10'
set output 'sequence.png'

plot [:][:25] \
'insert.log' using 2:xtic(1) with linespoints linewidth 3 title 'insert', \
'merge.log' using 2:xtic(1) with linespoints linewidth 3 title 'merge' , \
'quick.log' using 2:xtic(1) with linespoints linewidth 3 title 'quick'  