cd build
make
rm efficenza

dimensione_pixel=55
# 1->Litio, 2->Boro

for i in 0.5 1 2 5 10 15 20 25 30 35 40 45 50 55 60 70 80 100; do
		echo $i $dimensione_pixel 1 > hole_dimension
		./exampleB1 ../run_alpha.mac
		root -q root_analysis.cxx
done;


mv efficenza eff_li

for i in 0.5 1 2 5 10 15 20 25 30 35 40 45 50 55 60 70 80 100; do
		echo $i $dimensione_pixel 2 > hole_dimension
		./exampleB1 ../run_alpha.mac
		root -q ../root_analysis.cxx
done;
mv efficenza eff_boro

cd ..

