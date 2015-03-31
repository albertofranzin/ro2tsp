#!/usr/bin/python -w

import subprocess

list_of_files = subprocess.Popen('ls ./bbres/*.txt', stdout=subprocess.PIPE, shell=True)

(output, err) = list_of_files.communicate()

output = output.rstrip().split('\n')

instances = []
ubs = []
finubs = []
lbs = []
ingap = []
fingap = []
timeel = []
edgesleft = []

for f in output:
    with open(f, "r") as resfile:
        # print f.rstrip().split('/')[-1].split('.')[0]
        instances.append(f.rstrip().split('/')[-1].split('.')[0])
        lines = resfile.readlines()
        remedges = 0
        totedges = 0
        finalgap = "0.0%"
        timeelapsed = "2000.0"
        for line in lines:
            lineitems = line.rstrip('\n').split('=')
            # print lineitems
            if len(lineitems) == 1:
                toks = lineitems[0].split(' ')
                if len(toks) == 1: # 'End'
                    pass
                elif len(toks) == 2: # ub
                    ubs.append(toks[0].split('.')[0])
                elif len(toks) == 4: # ingap
                    ingap.append(toks[3])
                elif len(toks) == 6: # remedges 1
                    remedges = remedges + int(toks[5])
                elif len(toks) == 17: # no finished
                    finubs.append(int(toks[8].split('.')[0]))
                    finalgap = toks[12]
            elif len(lineitems) == 2:
                first  = lineitems[0].strip(' ')
                second = lineitems[1].strip(' ')
                # print first, second
                if first == "removed":
                    remedges = remedges + int(second)
                elif first == "final upper bound":
                    finubs.append(int(second.split('.')[0]))
                elif first == "time":
                    timeelapsed = second
                elif first == "lower bound":
                    lbs.append(second)
                elif first == "num edges":
                    totedges = int(second)
        # print instances[len(instances)-1], totedges
        if totedges == 0:
            edgesleft.append("ciao")
        else:
            edgesleft.append((totedges - remedges) / float(totedges) * 100)
        fingap.append(finalgap)
        timeel.append(timeelapsed)

#with open("constrainedbnb.tex", "w") as outfile:
for i in xrange(0, len(instances)):
    outresult = instances[i] + " & " + \
            ubs[i] + " & " + \
            lbs[i] + " & " + \
            str(finubs[i]) + " & " + \
            ingap[i] + " & " + \
            fingap[i] + " & " + \
            timeel[i] + " & " + \
            str(edgesleft[i]) + " \\\\"
    print outresult