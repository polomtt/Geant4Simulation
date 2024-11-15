#!/usr/bin/python
import numpy as np 
import pandas as pd
import math
import matplotlib.pyplot as plt
import datetime
import sys


def draw_histo(filename,i,x_col,y_col):
    df = pd.read_csv(filename,sep=",")

    plt.plot(df[x_col],(df[y_col]/df["ev_simulati"]),label=filename,lw=1,ls="--",ms=5, marker="o")
    plt.ylabel("Efficienza totale \u03b5 = \u03b5$_{intr}$ $\cdot$ \u03b5$_{geom}$ [-]",fontsize=13)
    plt.xlabel("Energia $\gamma$ [keV]",fontsize=13)
    plt.grid()

if __name__ == "__main__":

    plt.figure("plot")

    x_col = "energy[keV]"
    y_col = "ev_rivelati"
    
    for i in range(1, len(sys.argv)):
        print('argument:', i, 'value:', sys.argv[i])
        draw_histo(sys.argv[i],i,x_col,y_col)
    
    plt.title("Dim sensore: 400x400x100 $\mu$m\nDistanza sorgente $\gamma$ 2mm")
    plt.savefig("figura.png")
    plt.show()





