# gnuplot -persist plop1.gp 

# -- Export png 
set terminal png size 800,600
set output 'outcome/time-nbvertex-byradius.png'

# Radius	predicate	time	# Vertices
set title ' Nombre de sommets et temps calcul en fonction du Rayon '

set grid

# -- X --
set xrange [0:600]
set xtics 50
set xlabel ' Rayon '

# -- Y --
set yrange [0:2200]
set ytics 200
set ylabel ' # sommets '


# Légende
set key on inside center top

plot 'outcome/data.txt' u 1:4 w l title 'Nombre de sommets', 'outcome/data.txt' u 1:3 w l title 'Temps (ms)', x title 'y = x'

