# gnuplot -persist tools/plop1.gp 

# -- Export png 
set terminal png size 1200,800
set output 'outcome/time-nbvertex-byradius.png'

# Radius	predicate	time	# Vertices
set title ' Nombre de sommets et temps calcul en fonction du Rayon - Échelle log'

# Fitting
f1(x) = a1*x**(b1)            # define the function to be fit
a1 = 6; b1 = 0.8;            # initial guess for a1 and b1

f2(x) = a2*x**(b2)            # define the function to be fit
a2 = 6; b2 = 0.9;            # initial guess for a1 and b1

#fit f1(x) 'outcome/data.txt' using 1:4 via a1, b1
#fit f2(x) 'outcome/data.txt' using 1:3 via a2, b2  

# Résultat : 
a1 = 6.20779; b1 = 0.843573;
a2 = 6.75612; b2 = 0.909249;
set grid;

## ÉCHELLE

# Logarithme
set logscale;

# -- X --
set xrange [2:46340];
set xtics 2;
set xlabel ' Rayon (unité)';

# -- Y --
set yrange [6:115577];
set ytics 2;
set ylabel ' # sommets (unité), temps (ms)';


# Légende
set key on inside center top
     

plot 'outcome/data.txt' u 1:4 w l title 'Nombre de sommets', a1*x**b1 title 'fit - 6.20779*x**0.843573', 'outcome/data.txt' u 1:3 w l title 'Temps (ms)', a2*x**b2 title 'fit - 6.75612*x**0.909249'


