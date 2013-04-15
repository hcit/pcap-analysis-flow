#!/bin/bash
# README
#
# by chenxm
# 2013-04

function printinfo(){
	echo "Usage: $0 <pcap> <subfolder_in_rrd_data>"
	echo "		pcap    The pcap file to be processed"
	echo "		subfolder	folder to store rrd data base of spicified pcap file"
}

## Parse options
if [ $# -lt 2 ]; then
	printinfo && exit 1;
else
	pcapfile=$1
	subfolder=$2
	if [ -z $pcapfile ]; then
		echo "Input file cannot be empty"
		printinfo && exit 1;
	fi

	if [ !-d $subfolder ]; then
		echo "$subfolder does not exist. We will make it for you..."
		mkdir -p $subfolder
	fi
fi

## Process pcap file
scripthome=`dirname $0`
netconf="$scripthome/tstat.netconf"
rrdconf="$scripthome/tstat.rrdconf"

tstat -tSL -N $netconf -R $rrdconf -r $subfolder $pcapfile

echo -e "\nDONE!"

exit 0;
