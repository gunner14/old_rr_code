#!/bin/bash
#source /data/script/key/init.sh
LOGPATH="/data/yinxin/scripts/monitor_guide_access/"
echo "Merge Hour Logs"
for i in `seq 0 9`
do
    cat ${LOGPATH}stat.ret.txt.hour.0$i >> ${LOGPATH}stat.ret.txt.all.0015
    cat ${LOGPATH}stat.ret.txt.35.hour.0$i >> ${LOGPATH}stat.ret.txt.35.all.0015
    cat ${LOGPATH}stat.ret.txt.36.hour.0$i >> ${LOGPATH}stat.ret.txt.36.all.0015
done
for i in `seq 10 15`
do
    cat ${LOGPATH}stat.ret.txt.hour.$i >> ${LOGPATH}stat.ret.txt.all.0015
    cat ${LOGPATH}stat.ret.txt.35.hour.$i >> ${LOGPATH}stat.ret.txt.35.all.0015
    cat ${LOGPATH}stat.ret.txt.36.hour.$i >> ${LOGPATH}stat.ret.txt.36.all.0015
done
echo "Merge End"
echo "Processing..."
cat ${LOGPATH}stat.ret.txt.all.0015 | awk -F"|" -f ${LOGPATH}proc_merge_scount.awk | sort -nr  > ${LOGPATH}stat.ret.txt.all.0015.merged 
cat ${LOGPATH}stat.ret.txt.35.all.0015 | awk -F"|" -f ${LOGPATH}proc_merge.awk | sort -nr  > ${LOGPATH}stat.ret.txt.35.all.0015.merged
cat ${LOGPATH}stat.ret.txt.36.all.0015 | awk -F"|" -f ${LOGPATH}proc_merge.awk | sort -nr  > ${LOGPATH}stat.ret.txt.36.all.0015.merged
:> ${LOGPATH}stat.ret.txt.all.0015
:> ${LOGPATH}stat.ret.txt.35.all.0015
:> ${LOGPATH}stat.ret.txt.36.all.0015
echo "Process End"
python ${LOGPATH}send_15.py

