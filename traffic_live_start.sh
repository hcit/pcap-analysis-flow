#!/bin/bash
pid=
trap '[[ $pid ]] && echo "killing timer..." && kill $pid && kill_processes' 1 2 3 6 9 15
export PATH=/usr/loca/bin:/usr/bin:/bin:$PATH

kill_processes(){
	echo "killing children processes..."
        kill -9 `cat $justpid`
        kill -2 `cat $tstatpid`
        kill -2 `cat $dpipid`
}

scripthome=`dirname $0`
justpid="/var/run/mypid-just"
tstatpid="/var/run/mypid-tstat"
dpipid="/var/run/mypid-dpi"
datetime=`date '+%Y%m%d-%H%M'`

## Parse options
if [ $# -ne 1 ]; then
        echo "Usage: $0 <outputfolder>" && exit 1;
else
        outfolder=$1
fi

mkdir -p $outfolder
/usr/local/bin/tstat -i eth1 -l -N $scripthome/tstat.netconf -s $outfolder/wifilog_tstat &
echo $! > $tstatpid

mkdir -p $outfolder/wifilog_justniffer
/usr/bin/justniffer -i eth1 -F -c $scripthome/justniffer/justniffer-light.conf > $outfolder/wifilog_justniffer/$datetime.jn.out &
echo $! > $justpid

mkdir -p $outfolder/wifilog_ndpi
$scripthome/pcapDPI/pcapReader -i eth1 -w $outfolder/wifilog_ndpi/$datetime.dpi.out &
echo $! > $dpipid

sleep 59m && kill_processes & pid=$!
wait

exit 0;
