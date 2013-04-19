#!/usr/bin/env python
# Script to generate flow type for each line in merged *.tcp.out
# using panabit syslog
#
# By chenxm 
import sys

def print_usage():
    print "Usage: program <tcp_out> <panabit_out>"

# Options
if len(sys.argv) != 3:
    print_usage()
    exit(-1)

tcpoutname = sys.argv[1]
panalogname = sys.argv[2]
tcpoutfile = open(tcpoutname, 'rb')
panalogfile = open(panalogname, 'rb')
mergedfile = open(tcpoutname.split('.')[0]+'.tcp.dpi.out', 'wb')

def demux_panalog(one_log):
    slices = one_log.strip().split(' ')
    protocol = slices[0]
    host_a, host_b = slices[2].split('-')
    (bytes_a, bytes_b) = (int(slices[3]), int(slices[4]))
    socket = sorted([host_a, host_b])
    reverse = False
    if socket != [host_a, host_b]:
        reverse = True
    if reverse:
        return '-'.join(socket), (bytes_b, bytes_a, protocol)
    else:
        return '-'.join(socket), (bytes_a, bytes_b, protocol)

hash_table = {}
# Read panalog
print 'Reading panalog ...'
for line in panalogfile:
    line = line.strip()
    if line.split(' ', 1)[0].split('.')[1] != 'tcp':
        continue
    key, val = demux_panalog(line)
    if key not in hash_table:
        hash_table[key] = [val]
    else:
        hash_table[key].extend([val])
print 'Reading panalog done!'

def demux_tcpout(one_log):
    slices = one_log.strip().split(' ')
    host_a = '%s:%s' % (slices[0], slices[2])
    host_b = '%s:%s' % (slices[1], slices[3])
    bytes_a = int(slices[20])
    bytes_b = int(slices[21])
    socket = sorted([host_a, host_b])
    reverse = False
    if socket != [host_a, host_b]:
        reverse = True
    if reverse:
        return '-'.join(socket), (bytes_b, bytes_a)
    else:
        return '-'.join(socket), (bytes_a, bytes_b)

# Read (merged) tcp log
print 'Reading tcplog ...'
total_detected = 0
total_unknown = 0
for line in tcpoutfile:
    line = line.strip()
    key, val = demux_tcpout(line)
    found = False
    found_item = None
    if key in hash_table:

        if len(hash_table[key]) == 1:
            # unique match
            found = True
            found_item = hash_table[key][0]
        else:
            found = True
            found_item = hash_table[key][-1]
            # for item in hash_table[key]:
            #     if (val[0]-item[0]==0) and (val[1]-item[1]==0):
            #         found = True
            #         found_item = item
            #         break
                
    if found and found_item != None:
        total_detected += 1
        mergedfile.write(found_item[2]+'\n')
    else:
        total_unknown += 1
        mergedfile.write('N/A\n')
print 'Reading tcplog done!'


# Close opened files
tcpoutfile.close()
panalogfile.close()
mergedfile.flush() and mergedfile.close()

print 'Total: %d' % (total_detected+total_unknown)
print '\tdetected: %d' % total_detected
print '\tunknown: %d' % total_unknown
print 'DONE!'