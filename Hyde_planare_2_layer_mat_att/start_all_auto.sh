cd build
make
rm efficenza

for spes_li6 in 2; do
	for spes_b10 in 2; do
			echo $spes_b10 $spes_li6 > dimension_act_mat
			./exampleB1 run_neutron.mac
			root -q ../root_analysis.cxx
			rm -r *.root
	done;
done;

cd ..

