#!/usr/bin/python -w

with open("tempi_lagrange.txt", "r") as infile:
	lines = infile.readlines()

i = 0
while i < len(lines):
	nameline = lines[i]
	instname = nameline.rstrip("\n").split("/")[-1].split(".")[0]
	times  = []
	values = []
	numval = []	
	ratios = []
	for j in xrange(1, 6):
		cp = lines[i+j].rstrip("\n").split(" ")
		numval.append(cp[0])
		values.append(str(cp[0]).split('.')[0])
		times.append(str(cp[1]))
	for j in xrange(6,11):
		cp = lines[i+j].rstrip("\n").split(" = ")
		cp = cp[1].split(" : ")
		ratios.append(str(cp[0]))
	#print times, values, ratios
	
	print instname + " & ",
	for j in xrange(0, 5):
		if ratios[j] == "100.00":
			ratios[j] = "\\textbf{100.00}"
		print ratios[j] + ' & ' + times[j] + ' & ',
	print str(max(numval)) + " \\\\"

	i = i + 11
