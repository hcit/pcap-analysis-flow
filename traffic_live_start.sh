#!/bin/bash

scripthome=`dirname $0`

datetime=`date '+%Y%m%d-%H%M%S'`

justniffer -i eth1 -F -c $scripthome/justniffer/justniffer-light.conf > wifi_justniffer_$datetime &

tstat -i eth1 -l -N $scripthome/tstat.netconf -s wifi_tstat_$datetime &

#$scripthome/pcapDPI/pcapReader -i eth1 -w wifi_dpi_$datetime &

exit 0;
