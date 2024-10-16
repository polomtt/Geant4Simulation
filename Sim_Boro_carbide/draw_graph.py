#!/usr/bin/python
import numpy as np 
import pandas as pd
import math
import matplotlib.pyplot as plt
import datetime
import sys


def draw_histo(filename,i,x_col,y_col):
    df = pd.read_csv(filename,sep=";")

    y_to_plot = []
    
    ev =0
    
    if i==1 or i==2:
        ev = 1000000
    if i==3:
        ev = 10000000
    
    for a,b in zip(df["alfa"],df["litio"]):
        y_to_plot.append((a+b)/ev)
        # ev=ev-(a+b)
    
    line_stype="-"

    line_wif = 3
    
    if i==2:
        line_stype="--"
        line_wif = 1
    plt.plot(df[x_col],y_to_plot,label=i,ls=line_stype,lw=line_wif)
    plt.ylabel("Eff [-]",fontsize=13)
    plt.xlabel("Numero layer [-]",fontsize=13)
    plt.grid()
    plt.xticks()
    plt.legend()

if __name__ == "__main__":

    x_col = "id"
    y_col = "-"

    plt.figure("plot")
    for i in range(1, len(sys.argv)):
        print('argument:', i, 'value:', sys.argv[i])
        draw_histo(sys.argv[i],i,x_col,y_col)
        plt.savefig("figura_{}.png".format(i))
    # plt.yscale('log')
    plt.show()





