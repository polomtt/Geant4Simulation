#!/bin/bash
rm efficienza

ev_sim=100000

for i in  1 4 10 15 20 30 35 50 80 100
do 
rm  run_gamma.mac
echo "/control/verbose 0
      /run/verbose 0
      /tracking/verbose 0
      /run/initialize
      /gps/ang/type planar
      /gps/particle gamma
      /gps/pos/type Plane
      /gps/pos/shape Circle
      /gps/pos/radius 200 um
      /gps/pos/centre 0.0 0.0 2 mm
      /gps/energy $i keV
      /analysis/setFileName histo_gamma.root
      /run/printProgress 100000
      /run/beamOn $ev_sim" >> run_gamma.mac
      ./exampleB1 run_gamma.mac
root -q "root_analysis.cxx($i,$ev_sim)"
done
# rm -r *.root