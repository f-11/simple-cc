#!/usr/bin/env python3

import sys
argv = sys.argv

if len(argv) < 2:
    print('no input file name.')
    sys.exit(1)

local = {}
linenum = 0
fasm = open(argv[1])

for line in fasm:
    line = line.strip()
    if line != '' and line[0] != '/':
        x = line.rfind('//')
        if x != -1:
            line = line[:x]
        if line[0] == '.':
            local[line.strip()] = linenum
        else:
            linenum = linenum + 1

print(local)
fasm.seek(0)
linenum = 0
for line in fasm:
    line = line.strip()
    if line != '' and line[0] != '/':
        x = line.rfind('//')
        if x != -1:
            line = line[:x]
        s = line.split()
        if line[0] != '.':
            if len(s) > 1 and s[1][0:1] == '.':
                print(s[0] + " " + str(local[s[1]] - linenum - 1))
            else:
                print(line)
            linenum = linenum + 1

fasm.close()
