#! /usr/bin/env python
import re

justniffer_v10_file = "/mnt/frontdata/tcpdump/WIFI/trace_wifi/wifi_12091515-1620.pcap.js10.out"
justniffer_v11_file = "/mnt/frontdata/tcpdump/WIFI/trace_wifi/wifi_12091515-1620.pcap.js.out"

write_stream = open(justniffer_v11_file, 'wb')

for line in open(justniffer_v10_file, "rb"):
	line = line.strip("\r\n ")
	if line != "":
		[four_tuple, connection, rest] = re.split(r'[\[\]]', line, 2)
		parts = re.split(r'\[\w+\]', rest)
		flow_info = parts[0]

		all_parts = re.split('[:\-]', four_tuple.strip())
		all_parts.append(connection.strip())
		all_parts.extend(re.split('[\-]', flow_info.strip()))
		for item in parts[1:]:
			all_parts.append(item.strip())
		write_stream.write(' '.join(all_parts)+'\n')

write_stream.close()

#EOF