#!/bin/bash
rm efficienza
rm hole_dimension

echo "tanang,ang[deg],ev_riv" >> efficienza
echo "5 5 100 2" >> hole_dimension

# for i in 0 0.1 0.2 0.3 0.4 0.5 1 1.5 2 3 4 5 10 15 
for i in  15 
do 
rm  run_neutron.mac

echo "# Define scoring mesh
      /score/create/boxMesh boxMeshPS
      /score/mesh/boxSize 10 10 137.5 um
      /score/mesh/translate/xyz 0.0 0.0 0 um
      /score/mesh/nBin 1 1 1
      #/score/quantity/passageCellCurrent boxMeshPS 
      /score/quantity/nOfStep boxMeshPS 
      /score/filter/particle neutronFilter neutron
      /score/close
      /score/list
      /control/verbose 0
      /run/verbose 0
      /tracking/verbose 0
      /run/initialize
      /gps/particle neutron
      /gps/pos/type Plane
      /gps/direction 0 $i -1
      /gps/pos/shape Square
      /gps/pos/halfx 20. um
      /gps/pos/halfy 100. um
      /gps/pos/centre 0.0 0.0 0.139 mm
      /gps/ang/type planar
      /gps/energy 0.025 eV
      /analysis/setFileName histo_neut.root
      /run/printProgress 10000
      /run/beamOn 100000
      /score/dumpQuantityToFile boxMeshPS boxMeshPS mesh_output_$i" >> run_neutron.mac
      ./exampleB1 run_neutron.mac
root -q "root_analysis.cxx($i)"
done
rm -r *.root