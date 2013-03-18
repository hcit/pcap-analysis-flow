#!/usr/bin/env python
#
# Format the logs of TCP and HTTP in a general and convenient way,
# where the standards tools can process it directly.
#
# INPUT: tcp_log: merged TCP log from tcptrace and tstat outputs,
# usually with the suffix of *.tcp.out
#        http_log: output of justniffer with v1.1 configuration
# usually with the suffix of *.jn.out
# OUTPUT: three files: 
#   *   flow meta data (*.meta.csv) which records
#       flow id, src addr, dst addr, src port, dst port and first packet time
#   *   TCP features (*.tcp.csv)
#   *   HTTP features (*.http.csv)
# 
# By chenxm
# Created: 2013-03-13
#
import os
import sys
import uuid
import re


# where the key is the four-tuple, and the value
# is a list of (time_stamp, flow_id) tuples
flow_id_dict = {}

MAX_FLOW_TIME_GAP = 10 # seconds


def print_usage():
    print "Usage: program <tcp_log> <http_log> <dpi_log>"
    print "       where 'tcp_log' refers to the merged tcp info"
    print "       and 'http_log' means the justniffer output actually"
    print "       and 'dpi_log' stands for pcapDPI output file"


# Process the TCP log and add a random ID for each flow.
# The flow feature names are also added on the first line.
def process_tcp_log(filename):
    # import global variables
    global flow_id_dict, TCP_LOG_NAME, TCP_META_NAME

    print "Processing TCP log: %s" % filename
    
    # With a high view, TCP attributes are split into two parts:
    # first is the four-tuple info stored in the meta file
    # the other is the rest of attributes without four-tuple which is replaces with flow ID
    program_folder = os.path.split(sys.argv[0])[0]
    attributes = open(os.path.join(program_folder, "attributes.csv"), "rb").readline()
    attribute_parts = attributes.strip(" \r\n").split(" ", 5)
    
    meta_attributes = "{0} {1}".format("FID", ' '.join(attribute_parts[0:5]))
    tcp_log_attributes = "{0} {1}".format("FID", ' '.join(attribute_parts[4:]))
   
    # Prepare the meta file stream
    tcp_meta_of = open(TCP_META_NAME, "wb")
    tcp_meta_of.write(meta_attributes+"\n")
    
    # Prepare the TCP log file stream
    tcp_log_of = open(TCP_LOG_NAME, "wb")
    tcp_log_of.write(tcp_log_attributes+"\n")
    
    # Add an unique ID for each flow
    for line in open(filename):
        # Random uuid
        id = uuid.uuid1().hex;
        
        # Make the data in accordance with the attributes
        line_parts = line.split(" ", 5)
        tcp_meta_of.write("{0} {1}\n".format(id, ' '.join(line_parts[0:5])))
        tcp_log_of.write("{0} {1}".format(id, ' '.join(line_parts[4:])))
        
        # Extract IP addrs and ports, along with the timestamp
        src_addr = "{0}:{1}".format(line_parts[0], line_parts[2])
        dst_addr = "{0}:{1}".format(line_parts[1], line_parts[3])
        four_tuple = sorted([src_addr, dst_addr])
        flow_id_dict_key = '-'.join(four_tuple)
        if flow_id_dict_key not in flow_id_dict:
            flow_id_dict[flow_id_dict_key] = []
        flow_id_dict[flow_id_dict_key].append((id, float(line_parts[4])))
        
    # close the tcp log file
    tcp_meta_of.close()
    tcp_log_of.close()


# Process the HTTP log
# The ID of each log is gotten from the flow id dictionary; otherwise,
# if the flow is not found, this log will be dropped.
def process_http_log(filename):
    # import global variables
    global flow_id_dict, HTTP_LOG_NAME

    print "Processing HTTP log: %s" % filename
    
    # Remove the HTTP features already recorded by the meta file
    # and replace them with the flow ID
    program_folder = os.path.split(sys.argv[0])[0]
    attributes = open(os.path.join(program_folder, "attributes-http.csv"), "rb").readline()
    attribute_parts = attributes.strip(" \r\n").split(" ", 5)
    
    http_log_attributes = "{0} {1}\n".format("FID", ' '.join(attribute_parts[4:]))
    
    # Prepare the HTTP log handler
    http_log_of = open(HTTP_LOG_NAME, "wb")
    http_log_of.write(http_log_attributes)
    
    # Dictionary to record the connection status of HTTP pairs
    # There are four types of status defined by justniffer: unique, start, continue, last
    # It has no flow starting time logged except unique and start.
    # We use this dict to match items with continue and last to their start 
    http_pair_status = {}
    
    for line in open(filename, "rb"):
        # whitespace may exit in the HTTP header fields
        # better to split the HTTP header records with " " as separation.
        parts1 = line.split(" ", 17)
        parts2 = parts1[17].split('" "')
        parts = parts1[0:17] + parts2
        
        # For a normal HTTP log, there are fifty features logged in our configuration
        if len(parts) != 50:
            continue
        
        src_socket = "{0}:{1}".format(parts[0], parts[1])
        dst_socket = "{0}:{1}".format(parts[2], parts[3])
        four_tuple = sorted([src_socket, dst_socket])
        flow_id_dict_key = "-".join(four_tuple)
        status = parts[4].strip()

        if status == "unique":
            flow_start_time = float(parts[5].strip())
        elif status == "start":
            flow_start_time = float(parts[5].strip())
            http_pair_status[flow_id_dict_key] = flow_start_time
        elif status in ["continue", "last"]:
            if flow_id_dict_key not in http_pair_status:
                continue
            flow_start_time = http_pair_status[flow_id_dict_key]
            if status == "last":
                http_pair_status.pop(flow_id_dict_key)
        else:
            print "Invalid connection status!"
            raise
                    
        if flow_id_dict_key not in flow_id_dict:
            continue
        else:
            flow_id = None
            for item in flow_id_dict[flow_id_dict_key]:
                if abs(item[1]-flow_start_time) < MAX_FLOW_TIME_GAP:   # seconds       
                    flow_id = item[0]
        
        if flow_id != None:
            http_log_of.write("{0} {1}".format(flow_id, ' '.join(parts1[4:])))
    
    # Close the stream
    http_log_of.close()


def process_dpi_log(filename):
    # import global variables
    global flow_id_dict, DPI_LOG_NAME

    print "Processing DPI log: %s" % filename

    dpi_infile_stream = open(filename, "rb")
    dpi_outfile_stream = open(DPI_LOG_NAME, "wb")

    i = 0
    for line in dpi_infile_stream:
        line == line.strip(' \r\n')
        if line == "":
            continue
        
        parts = line.split()

        if i == 0:
            dpi_outfile_stream.write("FID " + ' '.join(parts[5:]) + '\n')
        else:
            l4_protocol = parts[5]
            if l4_protocol == 'UDP':
                continue

            flow_start_time = float(parts[4])
            src_socket = "{0}:{1}".format(parts[0], parts[1])
            dst_socket = "{0}:{1}".format(parts[2], parts[3])
            four_tuple = sorted([src_socket, dst_socket])
            flow_id_dict_key = "-".join(four_tuple)

            if flow_id_dict_key not in flow_id_dict:
                continue

            flow_id = None
            for item in flow_id_dict[flow_id_dict_key]:
                if abs(item[1]-flow_start_time) < MAX_FLOW_TIME_GAP:   # seconds     
                    flow_id = item[0]
                    break

            if flow_id != None:
                dpi_outfile_stream.write("{0} {1}\n".format(flow_id, ' '.join(parts[5:])))

        i += 1

    dpi_infile_stream.close()
    dpi_outfile_stream.close()


# Main
## Parsing options
if len(sys.argv) == 4:
    input_tcp_log = sys.argv[1]
    input_http_log = sys.argv[2]
    input_dpi_log = sys.argv[3]
else:
    print_usage()
    exit(-1)

# Fixed and formated names of output
head, tail=os.path.split(input_tcp_log)
basename = tail.split(".", 1)[0]
TCP_META_NAME = os.path.join(head, basename+".meta.csv")
TCP_LOG_NAME = os.path.join(head, basename+".tcp.csv")
HTTP_LOG_NAME = os.path.join(head, basename+".http.csv")
DPI_LOG_NAME = os.path.join(head, basename+".dpi.csv")

# Do processing
process_tcp_log(input_tcp_log)
process_http_log(input_http_log)
process_dpi_log(input_dpi_log)

print "\nOutput:"
print "TCP meta file:\t%s" % TCP_META_NAME
print "TCP with FID:\t%s " % TCP_LOG_NAME
print "HTTP with FID:\t%s " % HTTP_LOG_NAME
print "DPI with FID:\t%s " % DPI_LOG_NAME
