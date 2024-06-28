
echo 1 2.5 110 1 > hole_dimension;
./exampleB1 run_neutron_mesh_xz.mac
mv mesh_output mesh_output_d1_r25_litio_xz

echo 5 7.5 110 1 > hole_dimension;
./exampleB1 run_neutron_mesh_xz.mac
mv mesh_output mesh_output_d5_r75_litio_xz

echo 1 2.5 110 2 > hole_dimension;
./exampleB1 run_neutron_mesh_xz.mac
mv mesh_output mesh_output_d1_r25_boro_xz

echo 5 7.5 110 2 > hole_dimension;
./exampleB1 run_neutron_mesh_xz.mac
mv mesh_output mesh_output_d5_r75_boro_xz

rm -r *.root

