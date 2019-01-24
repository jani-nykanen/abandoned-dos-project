#!/bin/python3
# Generates fixed-point sine values 

import math;

PREC = 128
LINE_MAX = 16

f = open("src/util/trig.h", "w")
f.write("static int FIXED_SINE[]={")

lineCount = 0
for i in range(0, 360):
    s = int(math.sin(i / 180.0 * math.pi) * PREC)
    f.write(str(s) + ",")
    lineCount = lineCount +1
    if lineCount >= LINE_MAX:
        lineCount = lineCount - LINE_MAX
        f.write("\n")

f.write("};\n")
f.close()
    