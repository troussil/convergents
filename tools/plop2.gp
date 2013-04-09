# gnuplot -persist tools/plop2.gp 

# -- Export png 
set terminal png size 1200,800
set output 'outcome/time-100vertices-byradius.png'

# Graph title 
set title ' Nombre de sommets et temps calcul en fonction du Rayon - Échelle log'

# Variables
b = 0.666667



## Scale and grid

set grid;

# Logarithme
set logscale;

# -- X --
set xrange [32:16777216];
#set xtics 10;
set xlabel ' Rayon (unité)';

# -- Y --
set yrange [10:1508785];
#set ytics 10;
set ylabel ' # sommets (unité), temps (ms)';

# Style
set style fill solid 0.25

# Trace title
set key on inside center top
   
# GnuPlot
plot 'outcome/data-means.txt' u 1:4:5 w filledcu notitle lt rgb "#006400", 'outcome/data-means.txt' u 1:3 w lp title 'Time' lt rgb "#008000", 'outcome/data-means.txt' u 1:7:8 w filledcu notitle lt rgb "#1E90FF",  'outcome/data-means.txt' u 1:6 w l title 'Enveloppe Convexe' lt rgb "#0000FF", 'outcome/data-means.txt' u 1:10:11 w filledcu notitle lt rgb "#696969",	'outcome/data-means.txt' u 1:9 w lp title 'Alpha-shape r = inf' lt rgb "#000000",	'outcome/data-means.txt' u 1:13:14 w filledcu notitle lt rgb "#FF4500", 'outcome/data-means.txt' u 1:12 w lp title 'Alpha-shape' lt rgb "#FF0000", 	x**b title '2^(2/3)' lt rgb "#4B0082"

