#!/usr/bin/python -w

import subprocess

out_file = open("krvj_fl417_1.csv", "w")

with open("krvj_fl417_1.txt", "r") as text_file:
    #text_file.write("{}".format(table_values))
    lines = text_file.readlines()
    i = 0
    for line in lines:
        line = line.rstrip('\n').split(" ")
        #print line
        #if (line[0] == "pr_lagrange_vj"):
            #print ",".join(line[1:])
            #if i % 3 == 0:
        out_file.write(",".join(line[0:])+"\n")
        i = i + 1

out_file.close()
