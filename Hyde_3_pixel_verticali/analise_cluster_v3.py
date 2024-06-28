import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib import cbook, cm
from matplotlib.colors import LightSource
import matplotlib.ticker as ticker
import sys

def plot_all_graph(filename,vect_sez,xlabel_name,ylabel_name):
    # Load and format data
    df = pd.read_csv(filename,header=2)

    vect_pos = ["# iX"," iY"," iZ"]
    
    n_pos_x = np.max(df[vect_sez[0]])
    n_pos_y = np.max(df[vect_sez[1]])
    energy_vec = []

    tot_ent = 1000000
    eve_oltre_soglia_pixel = []
    eve_oltre_soglia_pixel_opp = []

    for xx in range(0,n_pos_x+1):
        help_vec = []
        for yy in range(0,n_pos_y+1):
            help_df = df[(df[vect_sez[0]]==xx) & (df[vect_sez[1]]==yy)]
            # ene_pos = np.sum(help_df[" total(value) [keV]"])/np.mean(det_ev)
            ene_pos = np.sum(help_df[" total(value) [keV]"])/np.sum(help_df[" entry"])
            
            if yy==103 and ene_pos>0.05:
                eve_oltre_soglia_pixel.append(np.sum(help_df[" entry"]))
                if xx<103:
                    eve_oltre_soglia_pixel_opp.append(np.sum(help_df[" entry"]))
                    
            if ene_pos<0.05:
                ene_pos = np.nan
            help_vec.append(ene_pos)

        energy_vec.append(help_vec)

    fig, ax = plt.subplots()
    plt.subplots_adjust(top=0.969,bottom=0.143,left=0.115,right=0.965,hspace=0.2,wspace=0.322)
    numero_punti_ax_X = len(energy_vec[0])
    numero_punti_ax_Y = len(energy_vec)

    bin_size = 100/200 # in um
    xxx, yyy = np.meshgrid(np.linspace(0,275, 550),np.linspace(0,55*3,110*3))
    # cmap = ax.pcolormesh(xxx,yyy,energy_vec,vmin=0.05,shading='auto',levels=2000)
    
    cmap = ax.contourf(xxx,yyy,energy_vec,levels=500)
    
    ax.set_xlabel(xlabel_name,fontsize=15)
    ax.set_ylabel(ylabel_name,fontsize=15)
    
    str_replace = filename.replace("result_from_server/", "")
    plt.title(str_replace)
    # cmap.set_label('Energy [MeV]',fontsize=15)
    cbar= fig.colorbar(cmap)
    cbar.ax.set_ylabel('Energy deposited [keV]',fontsize=15)
    cbar.ax.yaxis.set_major_formatter(ticker.FormatStrFormatter('%0.2f'))
    

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
        plot_all_graph(filename_xy,[" iY"," iZ"],"Position Z [$\mu$m]","Position Y [$\mu$m]")
        plt.subplots_adjust(left=None, bottom=None, right=0.92, top=0.90, wspace=None, hspace=None)
        plt.savefig(filename_xy+".pdf")
    
    plt.show()
