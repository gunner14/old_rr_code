#!/bin/sh

if [ -f ~/.bash_profile ]; then
        . ~/.bash_profile
fi


root="/hbase"
table=$1
size=$2

echo $table, $size, $actsize

hadoop jar fof.jar com.renren.datamining.friendsoffriends.RegionSplitter -libjars /home/hbase/hbase/hbase-0.94.1.jar, /home/hbase/hbase/lib/zookeeper-3.4.3.jar -t $table -r $root -s $size 2>/dev/null
