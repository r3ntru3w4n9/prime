# -*- coding: utf-8 -*-
"""
Created on Sat Jul  4 20:43:17 2020

@author: user
"""

import seaborn
import sys
import numpy as np
import matplotlib.pyplot as plt

# if len(sys.argv) != 2:
#     print("usage: python3 heatmap.py [dmd file]")

# filename = sys.argv[1]
filename = "D:\\NTU\\ICCAD\\prime\\testcase\\case1.txt.dmd"

with open(filename, "r") as f:
    data = [[int(y) for y in x.replace("\n", "").split()] for x in f.readlines()[1:]]

max_row = data[-1][0]
max_column = data[-1][1]
max_layer = data[-1][2]

heat = np.zeros((max_layer, max_row, max_column))
for i in range(max_layer):
    for j in range(max_row):
        for k in range(max_column):
            heat[i, j, k] = data[i*max_row*max_column + j*max_column + k][4]

f, axs = plt.subplots(figsize=(6, 10), nrows=max_layer)
for i in range(max_layer):
    axs[i].set_title(f"layer{i}")
    seaborn.heatmap(heat[i], ax=axs[i], vmin=0, vmax=data[i*max_row*max_column][3])