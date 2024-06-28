import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib import cbook, cm
from matplotlib.colors import LightSource
import matplotlib.ticker as ticker
import sys

def plot_all_graph(filename):
    # Load and format data
    df = pd.read_csv(filename,header=2)
    
    bin_dim = 0.5
    f = open(filename+"_map_xz_charge_deposited", "w")
    f.write("y[um],z[um],energy_deposited[keV],charge_deposited[-]\n")
    for y, z, e, cnt  in zip(df["# iX"],df[" iZ"],df[" total(value) [keV]"],df[" entry"]):
        
        help = 0
        
        if cnt>0 and e>0.0036:
            help = e/cnt
        
        
        f.write("{},{},{:.2f},{:.0f}\n".format(y*bin_dim,z*bin_dim,help,(help*1000/3.6)))
    f.close()

if __name__ == "__main__":
        
    filename_vect = ["result_from_server/mesh_output_d1_r25_boro_xz", \
                     "result_from_server/mesh_output_d1_r25_litio_xz",  \
                     "result_from_server/mesh_output_d1_r75_boro_xz",   \
                     "result_from_server/mesh_output_d1_r75_litio_xz",  \
                     "result_from_server/mesh_output_d5_r25_boro_xz",   \
                     "result_from_server/mesh_output_d5_r25_litio_xz",  \
                     "result_from_server/mesh_output_d5_r75_boro_xz",   \
                     "result_from_server/mesh_output_d5_r75_litio_xz"]
    
    
    for filename_xy in filename_vect: 
        plot_all_graph(filename_xy)
    
    plt.show()
