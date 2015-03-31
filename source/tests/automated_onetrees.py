#!/usr/bin/python -w

import subprocess

list_of_files = subprocess.Popen('ls ../../tsplib/fatte/*.tsp', stdout=subprocess.PIPE, shell=True)

(output, err) = list_of_files.communicate()

output = output.rstrip().split('\n')

# print output

table_values = ""

for i in output:
    #print "\n==================\n"
    instance = i.rstrip().split('/')[-1].split('.')[0]
    #print i, instance
    result_subp = subprocess.Popen(["./test", "--file", i], stdout=subprocess.PIPE)#, shell=True)
    (results, err) = result_subp.communicate()
    # print results, err
    results = results.rstrip('\n')
    print results
    table_values = table_values + instance + " & " + results + " \\\\"
    

    #raw_input('continuo?')

print table_values