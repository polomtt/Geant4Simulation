make
rm efficenza
rm run_neutron.mac
larghezza_alluminio=55

for larghezza_alluminio in 1000; do
    for spessore_mat_att in 1; do
        for spessore_alluminio in 1000; do
                rm run_neutron.mac
                mezzo_pixel=$(echo "scale=3; $larghezza_alluminio / 2" | bc)
                echo $larghezza_alluminio" "$mezzo_pixel

                echo    "/control/verbose 0
                        /run/verbose 0
                        /tracking/verbose 0
                        /run/initialize
                        /gps/particle neutron
                        /gps/pos/type Plane
                        /gps/pos/shape Square
                        /gps/pos/halfx $mezzo_pixel um
                        /gps/pos/halfy $mezzo_pixel um
                        /gps/pos/centre 0. 0. 5. cm
                        /gps/ang/type planar
                        /gps/energy 0.025 eV
                        /analysis/setFileName histo_neut.root
                        /run/printProgress 10000
                        /run/beamOn 100000" >> run_neutron.mac

                echo $spessore_mat_att $larghezza_alluminio $spessore_alluminio > dimensioni_sensore
                ./exampleB1 run_neutron.mac
                root -q root_analysis.cxx
        done;        
    done;
done;
