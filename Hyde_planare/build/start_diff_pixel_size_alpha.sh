make
rm efficenza
rm run_neutron_diff_pix_size.mac
dimensione_pixel=55
# 1->Litio, 2->Boro

for dimensione_pixel in 10000; do
    for spessore_mat_att in 1; do
            rm run_alfa_diff_pix_size.mac
            mezzo_pixel=$(echo "scale=3; $dimensione_pixel / 2" | bc)
            echo $dimensione_pixel" "$mezzo_pixel

            echo    "/control/verbose 0
                    /run/verbose 0
                    /tracking/verbose 0
                    /run/initialize
                    /gps/particle alpha
                    /gps/pos/type Plane
                    /gps/pos/shape Square
                    /gps/pos/halfx $mezzo_pixel um
                    /gps/pos/halfy $mezzo_pixel um
                    /gps/pos/centre 0. 0. 0.3 mm
                    /gps/ang/type planar
                    /gps/energy 5 MeV
                    /analysis/setFileName histo_neut.root
                    /run/printProgress 10000
                    /run/beamOn 100000" >> run_alfa_diff_pix_size.mac

            echo $spessore_mat_att $dimensione_pixel 2 > hole_dimension
            ./exampleB1 run_alfa_diff_pix_size.mac
            root -q root_analysis.cxx
            
    done;
done;


mv efficenza eff_diff_pix_size_i7