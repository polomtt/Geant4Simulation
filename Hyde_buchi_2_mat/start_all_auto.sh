cd build
make
rm efficenza

dimensione_pixel=500
radius_L=0

for dist in 1 1.5 2 2.5 3 3.5 4 4.5 5; do
	for radius_B in 1 1.5 2 2.5 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24; do
		echo $dist $radius_B $radius_L $dimensione_pixel 1 > hole_dimension
		./exampleB1 run_neutron.mac
		root -q ../root_analysis.cxx
	done;
done

mv efficenza eff_li




