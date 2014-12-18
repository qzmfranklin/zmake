#! /usr/bin/env python3

import matplotlib.pyplot as plt; plt.rcdefaults()
import numpy as np
import sys

# C sytle commandline argument
argc=len(sys.argv)-1
argv=sys.argv

if argc<1:
    print("Usage: plot_hull.py [fname]")
    exit(1)

raw = np.genfromtxt(argv[1], skip_header=1)
x  = raw[:,0]
y  = raw[:,1]
plt.plot(x,y,'ro-')
plt.margins(0.2)

plt.show()
