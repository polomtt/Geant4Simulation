#!/usr/bin/python
import sys

pixel_size = float(sys.argv[1])
half_sq = pixel_size/2000

f = open("run_n_v2.mac", "w")
f.write("/control/verbose 0\n\
/run/verbose 0\n\
/tracking/verbose 0\n\
/run/initialize\n\
/gps/particle neutron\n\
/gps/pos/type Plane\n\
/gps/pos/shape Square\n\
/gps/pos/halfx {:.2f} mm\n\
/gps/pos/halfy {:.2f} mm\n\
/gps/pos/centre 0. 0. 0.3 mm\n\
/gps/ang/type planar\n\
/gps/energy 0.025 eV\n\
/analysis/setFileName histo_neut.root\n\
/run/printProgress 10000\n\
/run/beamOn 100000".format(half_sq,half_sq))
f.close()


