#!/usr/bin/python -w

import subprocess

list_of_files = subprocess.Popen('ls ../../tsplib/*.tsp', stdout=subprocess.PIPE, shell=True)

(output, err) = list_of_files.communicate()

output = output.rstrip().split('\n')

print output

table_values = ""

for i in output:
    print "\n==================\n"
    instance = i.rstrip().split('/')[-1].split('.')[0]
    print i, instance
    result_subp = subprocess.Popen(["./test", "--file", i], stdout=subprocess.PIPE)#, shell=True)
    (results, err) = result_subp.communicate()
    print results, err
    results = results.split('\n')
    values = []
    times  = []
    for res in results:
        ress = res.split(' ')
        print(ress)
        #print('a')
        if (len(ress) == 2):
            values.append(float(ress[0]))
            times.append(float(ress[1]))
    print len(results)
    if len(results) == 6:
        #print times
        times[2] = times[2] + times[1]
        times[4] = times[4] + times[3]
        #print times, "\n---\n"
        minval = min(values)
        newlist = []
        for j in xrange(0,len(values)):
            values[j] = "{:2.2f}".format(100 * (values[j] - minval) / minval)
            if values[j] == "0.00":
                values[j] = "\\textbf{"+values[j]+"}"
            times[j] = str(times[j])
            newlist.append(values[j])
            newlist.append(times[j])
            #print type(values[j])
        table_values = table_values + instance + " & " + ' & '.join(newlist) + " & " + "{:10.0f}".format(minval) + '  \\\\\n'
        #table_times  = table_times  + instance + " & " + ' & '.join(times)  + '  \\\\\n'
        print table_values#, table_times
        with open("heur_table_1.tex", "w") as text_file:
            text_file.write("{}".format(table_values))

    raw_input('continuo?')
