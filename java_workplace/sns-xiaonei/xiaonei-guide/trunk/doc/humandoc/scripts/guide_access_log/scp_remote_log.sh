#!/bin/bash
LOGPATH="/data/yinxin/scripts/monitor_guide_access/"
for item in `cat ${LOGPATH}file_list`
do
        IP="`echo $item | awk -F":" '{print $1}'`"
        echo "IP:$IP" 
        ssh root@$IP "cp ${LOGPATH}the_hour_access.log ${LOGPATH}the_hour_access.log.temp"
        ssh root@$IP ":> ${LOGPATH}the_hour_access.log"
        scp root@$item.temp ${LOGPATH}the_hour_access.log.$IP 
        #echo ssh root@$IP 
        #ssh root@$IP "grep  \"Ice.Timeout\" $LOGF -A2 |awk -F \"]\" '{print \$2}' |grep -vE \"(Ice.TimeoutException)|(IceInternal.Outgoing.invoke)\" |sort |uniq -c |sort -nr | grep \"at \"" >> .mail.tmp

done
