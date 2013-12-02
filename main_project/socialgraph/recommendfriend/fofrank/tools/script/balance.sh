#!/bin/sh

if [ -f ~/.bash_profile ]; then
        . ~/.bash_profile
fi

echo $*
echo $$ > balance.pid

while true
do
	hadoop jar fof.jar com.renren.datamining.friendsoffriends.RegionBalancer -libjars /home/hbase/hbase/hbase-0.94.1.jar,/home/hbase/hbase/lib/zookeeper-3.4.3.jar $*
	sleep 60s
done

 
