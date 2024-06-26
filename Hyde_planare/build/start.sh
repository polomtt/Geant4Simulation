make
rm efficenza
now=$(date +"%T")
echo "Current time : $now"

dimensione_pixel=1000
# 1->Litio, 2->Boro
materiale=2
dim_buco=1.5
dis_buco=2
echo $dis_buco $dim_buco $dimensione_pixel $materiale > hole_dimension;
python3 create_mac_file.py $dimensione_pixel
./exampleB1 run_n_v2.mac

now=$(date +"%T")
echo "Current time : $now"

root root_analysis.cxx
