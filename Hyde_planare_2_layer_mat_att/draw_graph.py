import numpy as np 
import pandas as pd
import math
import matplotlib.pyplot as plt
import sys


eventi_simulati = 10000


data = pd.read_csv("efficenza", sep=';')
# data2 = pd.read_csv("/home/matteo/INFN/simulazioni/NaIL_build_v2/efficenza_20220609_0905",sep=";")
plt.plot(data["energy"],data["eff"]/np.float(eventi_simulati),lw=1.5,ms=3,marker="o")
# plt.plot(data2["energy"],data2["eff"]/np.float(eventi_simulati),lw=1.5,ms=3,marker="o")
plt.ylabel('Efficency [-]',fontsize=14)
plt.xlabel('Energy [MeV]',fontsize=13)
plt.grid()
plt.savefig("eff_vs_energy_simulated.png",dpi=300)
plt.show()




