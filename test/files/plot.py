import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from matplotlib.pyplot import subplots, show
from numpy import genfromtxt

tablenumber=int(sys.argv[1])
timeshift=0

currentFileNumber=1
label=[]
dataAKP=[]
for i in range(tablenumber):
    filename=sys.argv[currentFileNumber+1]
    mylabel=sys.argv[tablenumber+2]
    label.append(mylabel)
    tmp=genfromtxt(filename, delimiter=',',names=True)
    dataAKP.append(tmp)
    currentFileNumber=currentFileNumber+1   

fig, ax=plt.subplots(nrows=tablenumber, ncols=1,sharex=True, sharey=False)
plt.tight_layout()
AKP=[]
AKPtime=[]
for seriesnum in range(tablenumber):
    AKP.append([])
    AKPtime.append([])
    
    for i in dataAKP[seriesnum]:
        if seriesnum >1:
                AKPtime[seriesnum].append(i[0])
        else:
                AKPtime[seriesnum].append(i[0]-timeshift)
        AKP[seriesnum].append(i[1])

for i in range(tablenumber):
    ax.grid(True)
    ax.set_ylabel("degree")   
    ax.set_xlabel("sec") 
    ax.set_xlim(5,55)
    ax.plot(AKPtime[i],AKP[i],label=label[i])
    ax.legend(loc='upper right') 

plt.show()
