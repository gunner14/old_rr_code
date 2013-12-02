#!/bin/sh

prefix="rr_"

rm $prefix.pid

for i in $(ls $prefix*.sh); do
	sh $i &
	pid="$!"
	echo "$pid" >> $prefix.pid
	echo "Running $i with pid $pid"
	sleep 5s
done

while read line
do
	echo "Waiting $line to finish..."
	wait $line
done < $prefix.pid


