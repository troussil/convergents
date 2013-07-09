# gnuplot -persist tools/plop1.gp 

# -- Export png 
set terminal png size 1200,800
set output 'fig/4-con/nas/nas.png'

# Radius	predicate	time	# Vertices
set title ' temps calcul en fonction du Rayon - Échelle log'

# Fitting
b = 0.666667

f1(x) = a1*x**(b)            # define the function to be fit
a1 = 35;				             # initial guess for a1 and b1
 
fit f1(x) '../../outcome/as-full.txt' using 2:4 via a1


# Résultat : 
set grid;

## ÉCHELLE

# Logarithme
set logscale;

# -- X --
set xrange [32:33554432];
set xtics 50;
set xlabel ' Rayon (unité)';

# -- Y --
set yrange [170:	2640000];
set ytics 50;
set ylabel ' # sommets (unité), temps (ms)';


# Légende
set key on inside center top
     
plot a1*x**b title 'fit - a1*x**(2/3)', '../../outcome/as-full.txt' u 2:4 w l title 'Nombre de sommets de l alpha-shape'

#'../../outcome/as-full.txt' u 2:5:6 w filledcu notitle lt rgb "#006400",
