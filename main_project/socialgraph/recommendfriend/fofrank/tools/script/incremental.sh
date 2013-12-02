#!/bin/sh

HOMEPATH="/data1/zhi.qu"
WORKING="$HOMEPATH/fofincr"

if [ -f $HOMEPATH/.bash_profile ]; then
        . $HOMEPATH/.bash_profile
fi

cd $WORKING


JAR_FILE="fofxml.jar"

TABLE_NAME="relation_20120825"
SEQ_PATH="efrt_incr"
FINAL_TABLE="fof_1000group_incr2"
SETTING="general-settings.xml"

LAST_TIME="1970-01-01 00:00:00"
NEXT_TIME=`date -d "1 hour ago" "+%Y-%m-%d %H:%M:%S"`

if [ -f "lasttime.out" ]; then
	LAST_TIME=`cat lasttime.out`
fi

echo "Last time is" $LAST_TIME

# Start balancer.
echo "Starting balancer with $TABLE_NAME"
./balance.sh -t $TABLE_NAME &
sleep 1s

# Generate table import scripts and run
echo "Generating import scripts...."
cd sqoop/bin
sh import_relation.sh tablelist.full $TABLE_NAME "$LAST_TIME"
echo "Beginning import relations..."
sh run_relation.sh
cd ../../
echo "Import finished, next timestamp will be $NEXT_TIME"
# Save current time to file
echo $NEXT_TIME > lasttime.out

# Stop balancer.
echo "Stopping balancer..."
if [ -f "balance.pid" ]; then
	kill -9 `cat balance.pid`
fi

# Run splitter to check regions size.
echo "Start splitter to check region size..."
sh splitter.sh $TABLE_NAME 200 

# Generate HBASETOSEQ
echo "Generating sequence file from the table..."
hadoop jar $JAR_FILE com.renren.datamining.friendsoffriends.ExtendHBaseToSeq -libjars /home/hbase/hbase/hbase-0.94.1.jar,/home/hbase/hbase/lib/zookeeper-3.4.3.jar -i $TABLE_NAME -o $SEQ_PATH -f $SETTING
echo "Generating finished."


# Start balancer for final table.
echo "Starting balancer with $FINAL_TABLE"
./balance.sh -t $FINAL_TABLE &
sleep 10s

# Generate final table
echo "Generating final table in hbase.."
hadoop jar $JAR_FILE com.renren.datamining.friendsoffriends.RcdFoFSeqToHBase -libjars /home/hbase/hbase/hbase-0.94.1.jar,/home/hbase/hbase/lib/zookeeper-3.4.3.jar,/home/hbase/hbase/lib/protobuf-java-2.4.0a.jar -i $SEQ_PATH -o $FINAL_TABLE -f $SETTING

# Stop balancer.
echo "Stopping balancer..."
if [ -f "balance.pid" ]; then
	kill -9 `cat balance.pid`
fi

sleep 20s

if [ "$1" = "major" ] ; then
	echo "MAJORCOMPACT BEGIN"
	echo "major_compact '$FINAL_TABLE'" | hbase shell
	echo "MAJORCOMPACT END"
fi

# Finished.
echo "RUN FINISHED."
