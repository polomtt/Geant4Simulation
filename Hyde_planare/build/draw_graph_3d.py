#!/usr/bin/python
import numpy as np 
import pandas as pd
import math
import matplotlib.pyplot as plt
import datetime
import sys


def draw_histo(filename,i,x_col,y_col):
    df = pd.read_csv(filename,sep=";")

    x = df[x_col]
    y = df[y_col]

    z = []

    for det,tot in zip(df["det_ev"],df["tot_ev"]):
        z.append(det/tot)

    ax.plot_trisurf(x, y, z,cmap='viridis', edgecolor='none')

    ax.set_xlabel("Hole distance [$\mu$m]",fontsize=13)
    ax.set_ylabel("Hole radius [$\mu$m]",fontsize=13)
    ax.set_zlabel("Intrinsic efficiency [%]",fontsize=13)
    plt.grid()

if __name__ == "__main__":

    x_col = "distance_from_hole"
    y_col = "radius_hole"

    plt.figure("plot")
    ax = plt.axes(projection='3d')
    if len(sys.argv)<2:
        print("Inserire nome file")

    for i in range(1, len(sys.argv)):
        print('argument:', i, 'value:', sys.argv[i])
        draw_histo(sys.argv[i],i,x_col,y_col)
        # plt.savefig("figura_{}.png".format(i))

    plt.show()





