#!/usr/bin/python
import numpy as np 
import pandas as pd
import math
import matplotlib.pyplot as plt
import datetime
import sys


def draw_histo(filename,i,x_col,y_col):
    df = pd.read_csv(filename,sep=";")

    plt.plot(df[x_col],df[y_col],label=filename)
    plt.ylabel("Y [-]",fontsize=13)
    plt.xlabel("X [-]",fontsize=13)
    plt.grid()
    plt.xticks(rotation=30)
    plt.legend()

if __name__ == "__main__":

    plt.figure("plot")

    x_col = "#time"
    y_col = "bin"

    for i in range(1, len(sys.argv)):
        print('argument:', i, 'value:', sys.argv[i])
        draw_histo(sys.argv[i],i,x_col,y_col)
    plt.savefig("figura.png")
    plt.show()





