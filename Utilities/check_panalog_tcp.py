#!/usr/bin/env python
import sys

def print_usage():
    print "Usage: program panabit_out"
    
if len(sys.argv) != 2:
    print_usage()
    exit(-1)
    
panabit_out = sys.argv[1]

sockets = {}

print 'Reading file...'
total_lines = 0
for line in open(panabit_out, 'rb'):
    total_lines += 1
    parts = line.split(' ')
    protocol = parts[0].split('.')[0]
    point_a, point_b = parts[2].split('-')
    socket = [point_a, point_b]
    socket.sort()
    key = "%s-%s" % (socket[0], socket[1])
    if key not in sockets:
        sockets[key] = [1, set([protocol])]
    else:
        sockets[key][0] += 1
        sockets[key][1].add(protocol)

print 'Generating statistics...'
total_unique = 0
total_double = 0
total_multiple = 0
double_socket_unique_protocol = 0
double_socket_multiple_protocol = 0
multiple_socket_unique_protocol = 0
multiple_socket_multiple_protocol = 0
for key in sockets.keys():
    if sockets[key][0] == 1:
        total_unique += 1
    elif sockets[key][0] == 2:
        total_double += 1
        if len(sockets[key][1]) == 1:
            double_socket_unique_protocol += 1
        else:
            double_socket_multiple_protocol += 1
    else:
        total_multiple += 1
        if len(sockets[key][1]) == 1:
            multiple_socket_unique_protocol += 1
        else:
            multiple_socket_multiple_protocol += 1
        
print 'total tcp records: %d' % total_lines
print 'total unique sockets: %d' % total_unique
print 'total duplicate sockets: %d' % total_double
print 'total multiple sockets: %d' % total_multiple
print 'unique socket unique protocol: %d' % total_unique
print 'double sockets unique protocol: %d' % double_socket_unique_protocol
print 'double sockets multiple protocols: %d' % double_socket_multiple_protocol
print 'multiple sockets unique protocol: %d' % multiple_socket_unique_protocol
print 'multiple sockets multiple protocols: %d' % multiple_socket_multiple_protocol

