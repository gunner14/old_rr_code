#!/bin/bash
#source /data/script/key/init.sh
LOGPATH="/data/yinxin/scripts/monitor_guide_access/"
echo "Merge Hour Logs"
for i in `seq 16 23`
do
    cat ${LOGPATH}stat.ret.txt.hour.$i >> ${LOGPATH}stat.ret.txt.all.1623
    cat ${LOGPATH}stat.ret.txt.35.hour.$i >> ${LOGPATH}stat.ret.txt.35.all.1623
    cat ${LOGPATH}stat.ret.txt.36.hour.$i >> ${LOGPATH}stat.ret.txt.36.all.1623
done
echo "Merge End"
echo "Processing..."
cat ${LOGPATH}stat.ret.txt.all.1623 | awk -F"|" -f ${LOGPATH}proc_merge_scount.awk | sort -nr  > ${LOGPATH}stat.ret.txt.all.1623.merged 
cat ${LOGPATH}stat.ret.txt.35.all.1623 | awk -F"|" -f ${LOGPATH}proc_merge.awk | sort -nr  > ${LOGPATH}stat.ret.txt.35.all.1623.merged
cat ${LOGPATH}stat.ret.txt.36.all.1623 | awk -F"|" -f ${LOGPATH}proc_merge.awk | sort -nr  > ${LOGPATH}stat.ret.txt.36.all.1623.merged
:> ${LOGPATH}stat.ret.txt.all.1623
:> ${LOGPATH}stat.ret.txt.35.all.1623
:> ${LOGPATH}stat.ret.txt.36.all.1623
echo "Process End"
python ${LOGPATH}send_23.py

