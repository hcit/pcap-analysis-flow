#!/bin/bash
# README
#
# A simple wrapper to run tcptrace, tstat, justniffer
# one after another.
# 
# by chenxm
# 2013-01

function printinfo(){
	echo "Usage: $0 <inputpcap>"
	echo "  inputpcap    The pcap file to be processed"
}

function removeold(){
	rm -rf $1
	echo "removing privious output..."
}

function run_tcptrace(){
	echo -e "\n\n"
	echo "**********************************"
	echo "** tcptrace processing...         "
	echo "**********************************"
	echo "  Non-complete flows: no"
	echo "  UDP analysis: yes"
	tcptrace -cnul --csv $input > $tcptrace_out
	echo "Done!"
}

function run_tstat() {
	echo -e "\n\n"
	echo "**********************************"
	echo "** tstat processing...            "
	echo "**********************************"
	echo "  Non-complete flows: yes"
	echo "  UDP analysis: yes"
	tstat -N $scripthome/tstat.netconf -s $tstat_out $input
	echo "Done!"
}

function run_justniffer(){
	echo -e "\n\n"
	echo "**********************************"
	echo "** justniffer processing...            "
	echo "**********************************"
	removeold $justniffer_out
	$scripthome/justniffer/justniffer-run.sh -o $justniffer_out $input
	echo "Done!"
}

function run_pcapDPI() {
	echo -e "\n\n"
	echo "**********************************"
	echo "** nDPI processing...            "
	echo "**********************************"
	$scripthome/pcapDPI/pcapReader -i $input -w $ndpi_out
	echo "Done!"
}

function run_merge_tcptrace_tstat(){
	echo -e "\n\n"
	echo "**********************************"
	echo "** merging tcptrace and tstat out "
	echo "**********************************"
	$scripthome/merge_tcptrace_tstat_tcp.py --step $tcptrace_out $tstat_out/*.out/log_tcp_complete
	echo "Done!"
}

function run_format_logs() {
	echo -e "\n\n"
	echo "**********************************"
	echo "** formatting logs ...            "
	echo "**********************************"
	$scripthome/format_logs.py $mergedtcp_out $justniffer_out $ndpi_out
	echo "Done!"
}

function clear_all(){
	rm -rf $outfolder/*.tcp.tmp
	rm -rf $outfolder/*.udp.tmp
	#rm -rf $outfolder/*.jn.out
	#rm -rf $outfolder/*.tcp.out
	#rm -rf $outfolder/*.tt.out
	#rm -rf $outfolder/*.ts.out
	#rm -rf $outfolder/*.dpi.out
}

## Parse options
if [ $# -lt 1 ]; then
	echo "At least one pcap given"
	printinfo && exit 1;
else
	input=$1
	if [ -z $input ]; then
		echo "Input file must be specified"
		printinfo && exit 1;
	fi
fi

## Process pcap file
scripthome=`dirname $0`
outfolder="output"
mkdir $outfolder
for i in $@
do
	echo "Processing pcap file: $i"
	pcapbname=`basename $i`
	pcapbpath="$outfolder/$pcapbname"

	tcptrace_out="$pcapbpath.tt.out"
	tstat_out="$pcapbpath.ts.out"
	mergedtcp_out="$pcapbpath.tcp.out"
	justniffer_out="$pcapbpath.jn.out"
	ndpi_out="$pcapbpath.dpi.out"
	udp_out="$pcapbpath.tt.udp"

	# Work flow
	run_tcptrace
	run_tstat
	run_merge_tcptrace_tstat

	run_pcapDPI

	run_justniffer
	
	run_format_logs

	# clear all unused files
	clear_all

done

echo -e "\n\nALL DONE!"

exit 0;
