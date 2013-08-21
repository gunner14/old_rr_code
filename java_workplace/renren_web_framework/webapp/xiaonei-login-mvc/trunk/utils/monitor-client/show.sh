#!/bin/sh

WORK_DIR="/opt/monitor-client"
cd ${WORK_DIR}

TMP_DIR=".tmp"
GADGETS_SUFIX="_ppg.sh" # passport gadget
INV_TIME="60"

showTable()
{
	echo -e "\033[32;1m"`date`"\033[0m"
	echo -e "\033[32;1m===============================\033[0m"
	echo -e "\033[31;1m||name\t|\tresult\033[0m"
	for gadget in `ls $TMP_DIR/*.swp `;
	do
		ggName=`basename  "${gadget}" ".swp"`
		ggRt=`cat ${gadget}`
		ggf=`dirname $gadget`"/"`basename $gadget ".swp"`
		if test -n "$ggRt"
		then
			#echo -e "||${ggName}: ${ggRt}"
			cp $gadget $ggf
		fi
		ggRt=`cat ${ggf}`
		echo -e "||${ggName}: "
		echo -e " \t ${ggRt}"
	done;
	echo -e "\033[32;1m===============================\033[0m"
}

startGadgets()
{
	for gadget in `ls *${GADGETS_SUFIX}*`;
	do
		sec=`echo -e "${gadget}"|sed 's/'^.*${GADGETS_SUFIX}'//g'|sed 's/\.//g'`
		if test -z "$sec"
		then
			sec="$INV_TIME"
		fi
		ggName=`echo -e "${gadget}"|sed 's/'${GADGETS_SUFIX}.*'$//g'`
		screen -S monitor -d -m watch -n $sec sh ${gadget} $TMP_DIR/$ggName".swp"
	done;
}

sh stopMonitor.sh
startGadgets;
while true
do
	clear;
	showTable;
	sleep 20;
done
