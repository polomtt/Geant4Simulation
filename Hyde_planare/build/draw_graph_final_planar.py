import numpy as np 
import pandas as pd
import math
import matplotlib.pyplot as plt
from scipy.signal import savgol_filter



eventi_simulati = 100000


data = pd.read_csv("eff_boro", sep=';')
data_2 = pd.read_csv("eff_li", sep=';')

yhat = savgol_filter(100*data["det_ev"]/np.mean(data["tot_ev"]), 8, 3)
# plt.plot(data["distance_from_hole"],100*data["det_ev"]/np.mean(data["tot_ev"]),lw=1.5,ls="--",ms=3,marker="o")
plt.plot(data["distance_from_hole"],yhat,lw=1.5,color="crimson",label="$^{10}$B")

yhat = savgol_filter(100*data_2["det_ev"]/np.mean(data_2["tot_ev"]), 8, 3)
# plt.plot(data_2["distance_from_hole"],100*data_2["det_ev"]/np.mean(data_2["tot_ev"]),lw=1.5,ls="--",ms=3,marker="o")
plt.plot(data_2["distance_from_hole"],yhat,lw=1.5,color="navy",label="$^{6}$LiF")
plt.subplots_adjust(left=0.15,bottom=0.15)
plt.ylabel('Efficency [-]',fontsize=14)
plt.xlabel('Thickness of active material [$\mu$m]',fontsize=14)
plt.xticks(fontsize=13)
plt.yticks(fontsize=13)
plt.grid()
plt.legend(fontsize=14)
plt.xscale('log')
plt.savefig("eff_planare.pdf")
plt.show()




