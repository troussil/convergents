# gnuplot -persist tools/plop2.gp 

# -- Export png 
#set terminal png size 1200,800
#set output 'outcome/time-100vertices-byradius.png'

# -- Export svg
set terminal svg rounded size 1200,800
set output 'outcome/time-100vertices-byradius.svg'
set object 1 rect from screen 0, 0, 0 to screen 1, 1, 0 behind
set object 1 rect fc  rgb "white"  fillstyle solid 1.0

# Graph title 
set title ' Nombre de sommets et temps calcul en fonction du Rayon - Échelle log'

# Variables
b = 2.0/3.0

# Fitting
# fct - Time
f1(x) = a1*x**(b)            # define the function to be fit
a1 = 1;
fit f1(x) 'outcome/data-means.txt' using 1:3 via a1

# fct - Alpha-shape with infinite radius
f2(x) = a2*x**(b)            # define the function to be fit
a2 = 1;
fit f2(x) 'outcome/data-means.txt' using 1:9 via a2

# fct - Alpha-shape
f3(x) = a3*x**(b)            # define the function to be fit
a3 = 1;
fit f3(x) 'outcome/data-means.txt' using 1:12 via a3

## Scale and grid

set grid;

# Logarithme
set logscale;

# -- X --
set xrange [32:16777216];
#set xtics 10;
set xlabel ' Rayon (unité)';

# -- Y --
set yrange [32:4002468];
#set ytics 10;
set ylabel ' # sommets (unité), temps (ms)';

# Style
set style fill solid 0.25

# Trace title
set key on inside left top
   
# GnuPlot
plot 'outcome/data-means.txt' u 1:4:5 w filledcu notitle lt rgb "#006400", 'outcome/data-means.txt' u 1:3 w lp title 'Time' lt rgb "#008000", f1(x) title 'Fit - Time' lt rgb "#7FFF00", 'outcome/data-means.txt' u 1:7:8 w filledcu notitle lt rgb "#1E90FF",  'outcome/data-means.txt' u 1:6 w l title 'Enveloppe Convexe' lt rgb "#0000FF", 'outcome/data-means.txt' u 1:10:11 w filledcu notitle lt rgb "#696969",	'outcome/data-means.txt' u 1:9 w lp title 'Alpha-shape r = inf' lt rgb "#000000",	f2(x) title 'Fit - AS r = inf' lt rgb "#2F4F4F",	'outcome/data-means.txt' u 1:13:14 w filledcu notitle lt rgb "#FF4500", 'outcome/data-means.txt' u 1:12 w lp title 'Alpha-shape' lt rgb "#FF0000", 	f3(x) title 'Fit - AS' lt rgb "#DC143C"

