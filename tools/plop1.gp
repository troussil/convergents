# gnuplot -persist tools/plop1.gp 

# -- Export png 
set terminal png size 1200,800
set output 'outcome/time.png'

# Radius	predicate	time	# Vertices
set title ' temps calcul en fonction du Rayon - Échelle log'

# Fitting
b = 0.666667

#f1(x) = a1*x**(b)            # define the function to be fit
#a1 = 35;				             # initial guess for a1 and b1

#f2(x) = a2*x**(b)            # define the function to be fit
#a2 = 80;                     # initial guess for a1 and b1

f3(x) = a3*x**(b)            # define the function to be fit
#a3 = 1;                     # initial guess for a1 and b1

#fit f1(x) 'outcome/data.txt' using 1:4 via a1
#fit f2(x) 'outcome/data.txt' using 1:3 via a2  
#fit f3(x) 'outcome/data.txt' using 1:5 via a3   

#f10(x) = a10*x**(b10)            # define the function to be fit
#a1 = 35;	b10 = 0.8;              # initial guess for a1 and b1

#f20(x) = a20*x**(b20)            # define the function to be fit
#a20 = 80;	b20 = 0.9;             # initial guess for a1 and b1



#fit f10(x) 'outcome/data.txt' using 1:4 via a10, b10
#fit f20(x) 'outcome/data.txt' using 1:3 via a20, b20 


# Résultat : 
#a10 = 6.20779; b10 = 0.843573;
#a20 = 6.75612; b20 = 0.909249;
set grid;

## ÉCHELLE

# Logarithme
set logscale;

# -- X --
set xrange [32:46001];
set xtics 50;
set xlabel ' Rayon (unité)';

# -- Y --
set yrange [6:25000];
set ytics 50;
set ylabel ' # sommets (unité), temps (ms)';


# Légende
set key on inside center top
     

#plot 'outcome/data.txt' u 1:4 w l title 'Nombre de sommets de l alpha-shape', a1*x**b title 'fit - a1*x**(2/3)', a10*x**b10 title 'fit - a10*x**(b10)', 'outcome/data.txt' u 1:3 w l title 'Temps (ms)', a2*x**b title 'fit - a2*x**(2/3)', a20*x**b20 title 'fit - a20*x**(b20)', 'outcome/data.txt' u 1:5 w l title 'Nombre de sommets de enveloppe convexe', a3*x**b title 'fit - x**(2/3)'

plot 'outcome/data.txt' u 1:4 w l title 'Nombre de sommets de l alpha-shape', a1*x**b title 'fit - a1*x**(2/3)', 'outcome/data.txt' u 1:5 w l title 'Nombre de sommets de enveloppe convexe', a3*x**b title 'fit - x**(2/3)'

