for i in 200 300 500 700 1000 2000 5000; do
		echo 50 50 $i 2 > hole_dimension;
		python3 create_mac_file.py $i
		./exampleB1 run_n_v2.mac
		root -q root_analysis.cxx
done

mv efficenza eff_diff_pixel_1cpu_i7

