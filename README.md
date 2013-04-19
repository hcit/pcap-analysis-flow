pcap-analysis-flow
==================

Programs to analyze pcap file and extract flow statistics, identification, HTTP
protocol fields in a batch mode.

by chenxm

2013-03


Requirements
============

Your systems should have the standard tcptrace, tstat, justniffer installed and
their binary files called with $PATH variables (ex. /usr/bin).

New version of pcapDPI (binary file has been delivered along with the project)

Usage
==========

`pcap-workflow.sh` combines the analysis from a pcap trace to precisely formated
logs in .csv format. The workflow aggerates following processing:

|tcptrace on pcap| --> |tstat on pcap| --> |merge tcptrace and tstat outputs| -->

|justniffer on pcap| --> 

|pcapDPI on pcap| -->

|format the outputs all above|


Utilities
==========

`batch-tcpprep.sh`: a wrapper of tcpprep (an utility of tcpreplay to generate cache file used by main programs) to
generate cache files for multiple pcap files with one line command.

`batch-tcpreplay.sh`: a wrapper of tcpreplay to replay multiple pcap files with one line command. It must work after
the batch tcpprep wrapper.

`captcp.py`: a borrowed tool from: http://research.protocollabs.com/captcp/

`pcap-split-client-server.py`: split pcap traces into client and server sides by indicating client/server networks.
If only the --client network is explicit, the packets with source IP belonging to the network are classified as client
while all the others are classified as the server side. It's the same that only --server network is explicit.
If the --client and --server are both explicit, the packets with source IP belonging to the client AND destination IP
belonging to the server are classified as client; the packets with reversed direction are classified as server side; and
the left is ignored.

`pcap-split-fixed-size.sh`: a preprocessing tool for hadoop to prepare fixed-size input pcap files.
Each split file is with some fixed size, the default being 64MB.
