#!/bin/bash
pid=
trap '[[ $pid ]] && kill $pid && kill_processes' 1 2 3 6 9 15

kill_processes(){
        kill -9 `cat $justpid`
        kill -2 `cat $tstatpid`
        kill -2 `cat $dpipid`
}

scripthome=`dirname $0`
justpid="/var/run/mypid-just"
tstatpid="/var/run/mypid-tstat"
dpipid="/var/run/mypid-dpi"
datetime=`date '+%Y%m%d-%H%M%S'`

justniffer -i eth1 -F -c $scripthome/justniffer/justniffer-light.conf > wifi_justniffer_$datetime &
echo $! > $justpid

tstat -i eth1 -l -N $scripthome/tstat.netconf -s wifi_tstat_$datetime &
echo $! > $tstatpid

$scripthome/pcapDPI/pcapReader -i eth1 -w wifi_dpi_$datetime &
echo $! > $dpipid

sleep 12h & pid=$!
wait

kill_processes

exit 0;
