# gnuplot -persist tools/plop1.gp 

# -- Export png 
set terminal png size 1200,800
set output 'outcome/time.png'

# Radius	predicate	time	# Vertices
set title ' temps de calcul en fonction du Rayon - Échelle log'

# Fitting
b = 0.666667


set grid;

## ÉCHELLE

# Logarithme
set logscale;

# -- X --
set xrange [32:134217728];
set xtics 10;
set xlabel ' Rayon (unité)';

# -- Y --
set yrange [6:2450000];
set ytics 10;
set ylabel ' temps (ms)';


# Légende
set key on inside center top
     

plot 'outcome/ch-time.txt' u 2:4 w l title 'Temps Har-Peled', 'outcome/ch-time.txt' u 2:3 w l title 'Temps Marche de Grahaam'

