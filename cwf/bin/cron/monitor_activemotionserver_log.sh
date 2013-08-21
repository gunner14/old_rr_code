#!/bin/bash

. ~/.xcerc
EXPECTED_COUNT=$1;
echo "EXPECTED_COUNT : "$EXPECTED_COUNT

check() {
	echo "--------------"
	LASTHOUR=`date -d "1 hours ago" "+%Y-%m-%d-%H"`
	LOG_COUNT=`ls -l /data/LogData/user_browse/ | grep $LASTHOUR | wc -l`
	echo $(date)
	echo $LASTHOUR" : "$LOG_COUNT

	if ((LOG_COUNT<EXPECTED_COUNT)) 
	then
		WARN="["$NICKNAME"] cwf logs(es) of "$LASTHOUR" : "$LOG_COUNT" < "$EXPECTED_COUNT
		echo $WARN
		wget -q -O /dev/null "http://10.22.198.81:2000/receiver?number=13810091260&message=$WARN"
	fi
}

while true 
do
	check
	sleep 30m
done
