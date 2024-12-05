import numpy as np 
import pandas as pd
import math
import matplotlib.pyplot as plt
import sys


eventi_simulati = 100000


df = pd.read_csv("build/efficienza_server", sep=';')

# ;spess_li6[um];det_ev;tot_ev

unique_list = list(set(df["spess_b10[um]"]))
unique_list.sort() 

for sp_b10 in unique_list:
    print("******************")
    print(sp_b10)
    slice = df[df["spess_b10[um]"]==sp_b10]
    
    print(slice)
    
    plt.plot(slice["spess_li6[um]"],slice["det_ev"]/slice["tot_ev"],label="{}um di b10".format(sp_b10))

plt.legend()
plt.show()

# plt.plot(data["energy"],data["eff"]/np.float(eventi_simulati),lw=1.5,ms=3,marker="o")
# # plt.plot(data2["energy"],data2["eff"]/np.float(eventi_simulati),lw=1.5,ms=3,marker="o")
# plt.ylabel('Efficency [-]',fontsize=14)
# plt.xlabel('Energy [MeV]',fontsize=13)
# plt.grid()
# plt.savefig("eff_vs_energy_simulated.png",dpi=300)
# plt.show()




