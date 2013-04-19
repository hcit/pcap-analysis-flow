#! /usr/bin/env python
import sys

if len(sys.argv) != 2:
	print("Usage: program httplogfile")
	exit(-1)
httplogfile = open(sys.argv[1], 'rb')
uafile = open(sys.argv[1].split('.', 1)[0]+'.uaset.out', 'wb')

ua_set = set()
i = 0
for line in httplogfile:
	request = line.split('[rqs]')[1]
	fields = request.strip().split(' "')
	if len(fields) > 4:
		ua_set.add(fields[4].strip(' "'))

for item in ua_set:
	uafile.write(item+'\n')

httplogfile.close()
uafile.flush() and uafile.close()