#!/usr/bin/python
import numpy as np 
import pandas as pd
import math
import matplotlib.pyplot as plt
import datetime
import sys


def draw_histo(filename,i,x_col,y_col):
    df = pd.read_csv(filename,sep=",")
    
    y_norm_by_inciden_flux = []
    
    for ang,ent in zip(df["tanang"],df["ev_riv"]):
        df_norm = pd.read_csv("mesh_output_{}".format(ang),sep=",",header=2)
        y_norm_by_inciden_flux.append(ent/df_norm[" entry"][0])

    plt.plot(df[x_col],y_norm_by_inciden_flux,label=i)
    plt.ylabel("Y [-]",fontsize=13)
    plt.xlabel("X [-]",fontsize=13)
    plt.grid()
    plt.xticks()
    plt.legend()

if __name__ == "__main__":

    x_col = "ang[deg]"
    y_col = "ev_riv"
    # tanang,ang[deg],ev_riv
    
    plt.figure("plot")
    for i in range(1, len(sys.argv)):
        print('argument:', i, 'value:', sys.argv[i])
        draw_histo(sys.argv[i],i,x_col,y_col)
        plt.savefig("figura_{}.png".format(i))
    plt.show()





