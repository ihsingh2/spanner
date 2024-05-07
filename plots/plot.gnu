set terminal pngcairo enhanced font 'Verdana,12'

# ------------------------------------------------ 2k-1 spanner ------------------------------------------------
# ---------------- EXECUTION TIME ---------------
set output 'plots/2k-1-spanner/execution_time.png'
set title 'Total execution time'
set ylabel 'Runtime (in ms)'
set xlabel 'Size of the graph, |E(G)|'
set xrange [0:2500000]
set format x "%.0s*10^{%T}"
set grid

plot 'plots/2k-1-spanner/execution_time.txt' using 1:2 with linespoints title 't = 3', \
     '' using 1:3 with linespoints title 't = 4', \
     '' using 1:4 with linespoints title 't = 5'

# ----------------- SPANNER SIZE ----------------
set output 'plots/2k-1-spanner/spanner_size.png'
set title 'Size of spanner'
set ylabel 'Size of the spanner, |E(S)|'
set xlabel 'Order of the graph, |V(G)|'
set xrange [0:500000]
set format x "%.0s*10^{%T}"
set grid

plot 'plots/2k-1-spanner/spanner_size.txt' using 1:2 with linespoints title 't = 3', \
     '' using 1:3 with linespoints title 't = 4', \
     '' using 1:4 with linespoints title 't = 5', \
     '' using 1:4 with linespoints title 't = 6'

# ---------------- STEPWISE TIME ----------------
set output 'plots/2k-1-spanner/stepwise_time.png'
set title 'Stepwise execution time for t = 5'
set ylabel 'Runtime (in ms)'
set xlabel 'Size of the graph, |E(G)|'
set xrange [0:2000000]
set format x "%.0s*10^{%T}"
set grid

plot 'plots/2k-1-spanner/stepwise_time.txt' using 1:2 with linespoints title 'Sampling cluster centres', \
     '' using 1:3 with linespoints title 'Finding neighbouring clusters', \
     '' using 1:4 with linespoints title 'Adding edges to the spanner', \
     '' using 1:5 with linespoints title 'Removing intra cluster edges', \
     '' using 1:6 with linespoints title 'Cluster joining'

# -------------------------------------------------- 3 spanner -------------------------------------------------
# ---------------- EXECUTION TIME ---------------
set output 'plots/3-spanner/execution_time.png'
set title 'Total execution time'
set ylabel 'Runtime (in ms)'
set xlabel 'Size of the graph, |E(G)|'
set xrange [0:2500000]
set format x "%.0s*10^{%T}"
set grid

plot 'plots/3-spanner/execution_time.txt' using 1:2 with linespoints title ''

# ----------------- SPANNER SIZE ----------------
set output 'plots/3-spanner/spanner_size.png'
set title 'Size of spanner'
set ylabel 'Size of the spanner, |E(S)|'
set xlabel 'Order of the graph, |V(G)|'
set xrange [0:500000]
set format x "%.0s*10^{%T}"
set grid

plot 'plots/3-spanner/spanner_size.txt' using 1:2 with linespoints title ''

# ---------------- STEPWISE TIME ----------------
set output 'plots/3-spanner/stepwise_time.png'
set title 'Stepwise execution time'
set ylabel 'Runtime (in ms)'
set xlabel 'Size of the graph, |E(G)|'
set xrange [0:2000000]
set format x "%.0s*10^{%T}"
set grid

plot 'plots/3-spanner/stepwise_time.txt' using 1:2 with linespoints title 'Sampling cluster centres', \
     '' using 1:3 with linespoints title 'Formation of clusters', \
     '' using 1:4 with linespoints title 'Segregating inter-cluster edges', \
     '' using 1:5 with linespoints title 'Joining vertices with neighboring clusters'
