import sys

energy = float(sys.argv[1])

file1 = open('run_multiple_gamma.mac', 'w')

L=["/control/verbose 1\n","/run/verbose 1\n","/tracking/verbose 0\n","/run/initialize\n", \
"/gps/particle gamma\n", \
"/gps/pos/type Plane\n", \
"/gps/pos/shape Square\n", \
"/gps/pos/halfx 0.50 mm\n"
"/gps/pos/halfy 0.50 mm\n", \
"/gps/pos/centre 0. 0. 0.3 mm\n", \
"/gps/ang/type planar\n", \
"/gps/energy {:.1f} keV\n".format(energy), \
"/analysis/setFileName histo_gamma.root\n", \
"/run/beamOn 100000\n"]

file1.writelines(L)

file1.close()
  

