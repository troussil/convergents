# gnuplot -persist tools/plop1.gp 

# -- Export png 
set terminal png size 1200,800
set output 'outcome/time-100vertices-byradius.png'

# Radius	predicate	time	# Vertices
set title ' Nombre de sommets et temps calcul en fonction du Rayon - Échelle log'

# Fitting
b = 0.666667

set grid;

## SCALE

# Logarithme
set logscale;

# -- X --
set xrange [32:2097152];
set xtics 5;
set xlabel ' Rayon (unité)';

# -- Y --
set yrange [10:360000];
set ytics 5;
set ylabel ' # sommets (unité), temps (ms)';


# Légende
set key on inside center top
     
plot 'outcome/data-means.txt' u 1:3 w lp title 'Time', 'outcome/data-means.txt' u 1:6 w lp title 'Enveloppe Convexe', 'outcome/data-means.txt' u 1:9 w lp title 'Alpha-shape r = inf', 'outcome/data-means.txt' u 1:12 w lp title 'Alpha-shape', x**b title '2^(2/3)'

