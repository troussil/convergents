# gnuplot -persist tools/plop1.gp 

# -- Export png 
set terminal png size 1200,800
set output 'fig/4-exi/ch/exi-ch-sommet.png'

# Radius	predicate	time	# Vertices
set title ' temps calcul en fonction du Rayon - Échelle log'

# Fitting
b = 0.666667

f1(x) = a1*x**(b)            # define the function to be fit
a1 = 1;				             # initial guess for a1 and b1


fit f1(x) '../../outcome/ch-full.txt' using 2:3 via a1

set grid;

## ÉCHELLE

# Logarithme
set logscale;

# -- X --
set xrange [32:269000000];
set xtics 10;
set xlabel ' Rayon (unité)';

# -- Y --
set yrange [30:6620000];
set ytics 10;
set ylabel ' # sommets (unité), temps (ms)';


# Légende
set key on inside center top
     

plot '../../outcome/ch-full.txt' u 2:4:5 w filledcu notitle lt rgb "#00CFFC", a1*x**b title 'fit - x**(2/3)' lt rgb "#FF0006", '../../outcome/ch-full.txt' u 2:3 w l title 'Nombre de sommets de l enveloppe convexe' lt rgb "#2053BF", '../../outcome/ch-full.txt' u 2:7:8 w filledcu notitle lt rgb "#E4A1FF", '../../outcome/ch-full.txt' u 2:6 w l title 'Nombre de points sur le bord de l enveloppe convexe' lt rgb "#9120BF"

