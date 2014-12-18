#! /usr/bin/env python3

import matplotlib.pyplot as plt; plt.rcdefaults()
import numpy as np
import sys

# C sytle commandline argument
argc=len(sys.argv)-1
argv=sys.argv

if argc<1 or argc>3:
    print("Usage: plot_mst.py [node_fname] [edge_fname = \"\"]")
    print("argc = ", argc)
    exit(1)

# Raw nodes
node_list = np.genfromtxt(argv[1], skip_header=1)
x = node_list[:,0]
y = node_list[:,1]
plt.plot(x,y,'ro')
plt.margins(0.2)

# Edges, if any
if argc == 2:
    edge_list = np.genfromtxt(argv[2])
    for e in edge_list:
        x1 = node_list[e[0]][0]
        y1 = node_list[e[0]][1]
        x2 = node_list[e[1]][0]
        y2 = node_list[e[1]][1]
        # [x1, x2] [y1, y2]
        plt.plot([x1, x2], [y1, y2], color='k', lw=2)

plt.show()
