make
rm efficenza

dimensione_pixel=1100
dimensione_pixel=1100
# 1->Litio, 2->Boro


# for i in 1 1.5 2 2.5 3 3.5 4 4.5 5; do
# 	for j in 1 1.5 2 2.5 3 3.5 4.5 5 6 6.5 7 7.5 8 8.5 9 9.5 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24; do
# 		echo $i $j $dimensione_pixel 1 > hole_dimension
# 		./exampleB1 run_n_v2.mac
# 		root -q root_analysis.cxx
# 	done;
# done

# mv efficenza eff_li

# for i in 1 1.5 2 2.5 3 3.5 4 4.5 5; do
# 	for j in 1 1.5 2 2.5 3 3.5 4.5 5 6 6.5 7 7.5 8 8.5 9 9.5 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24; do
# 		echo $i $j $dimensione_pixel 2 > hole_dimension
# 		./exampleB1 run_n_v2.mac
# 		root -q root_analysis.cxx
# 	done;
# done

# mv efficenza eff_boro

echo 2.5 2.25 $dimensione_pixel 2 > hole_dimension
./exampleB1 run_n_v2.mac
root -q root_analysis.cxx
mv efficenza eff_boro_45_25




