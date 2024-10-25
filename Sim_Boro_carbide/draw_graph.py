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
    
    n_Ev=1
    
    try:
        n_Ev=np.mean(df["ev_sim"])
    except:
        pass
    
    for a,b in zip(df["alfa"],df["litio"]):
        y_to_plot.append((a+b)/n_Ev)
    
    line_stype="-"

    line_wif = 3
    
    norm = 1.0
        
    plt.plot(df[x_col],y_to_plot/np.mean(norm),label=filename,ls=line_stype,lw=line_wif)
    plt.ylabel("Neutron capture efficiency [-]",fontsize=13)
    plt.xlabel("Number of layer [-]",fontsize=11)
    plt.grid()
    plt.xticks()
    plt.legend(fontsize=11)
    
    print(np.max(y_to_plot))

if __name__ == "__main__":

    x_col = "id"
    y_col = "-"

    max_value = 1.0
    
    plt.figure("plot")
    for i in range(1, len(sys.argv)):
        print('argument:', i, 'value:', sys.argv[i])
        draw_histo(sys.argv[i],i,x_col,y_col)
        plt.savefig("figura.png".format(i))
    # plt.yscale('log')
    plt.show()





