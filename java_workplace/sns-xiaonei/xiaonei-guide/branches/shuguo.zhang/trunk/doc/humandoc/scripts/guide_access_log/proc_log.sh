#!/etc/bin
#cat the_hour_access.log | awk -F"?" '{print $1" "$2" "$3}' | awk '{time[$2]+=$1;count[$2]++}END{for(item in time){print count[item] "\t" time[item]/count[item] "\t" item}}'  | sort -nr 
LOGPATH="/data/yinxin/scripts/monitor_guide_access/"
THEHOUR=`date +%H`
cat ${LOGPATH}the_hour_access.log.10.3.17.35 ${LOGPATH}the_hour_access.log.10.3.17.36 | awk -F"?" '{print $1" "$2" "$3}' | awk '{ {time[$2]+=$1;count[$2]++;t=$1*1000;} \
if(t<=100) {sub_count[$2"_100"]++} \
if(t>100 && t<=300) {sub_count[$2"_300"]++} \
if(t>300 && t<=1000) {sub_count[$2"_1000"]++} \
if(t>1000) {sub_count[$2"_1001"]++} \
} END \
{for(i in count) \
{print count[i] "|" i "|" time[i]"|" count[i] "|" sub_count[i"_100"] "|" sub_count[i"_300"] "|" sub_count[i"_1000"] "|" sub_count[i"_1001"] "|"}}' | sort -nr > ${LOGPATH}stat.ret.txt 
cp ${LOGPATH}stat.ret.txt ${LOGPATH}stat.ret.txt.hour.${THEHOUR}

cat ${LOGPATH}the_hour_access.log.10.3.17.35 | awk -F"?" '{print $1" "$2" "$3}' | awk '{ {time[$2]+=$1;count[$2]++;t=$1*1000;} \
if(t<=100) {sub_count[$2"_100"]++} \
if(t>100 && t<=300) {sub_count[$2"_300"]++} \
if(t>300 && t<=1000) {sub_count[$2"_1000"]++} \
if(t>1000) {sub_count[$2"_1001"]++} \
} END \
{for(i in count) \
{print time[i] "|" i "|" time[i]"|" count[i] "|" sub_count[i"_100"] "|" sub_count[i"_300"] "|" sub_count[i"_1000"] "|" sub_count[i"_1001"] "|"}}' | sort -nr | head -n40 > ${LOGPATH}stat.ret.txt.35
cp ${LOGPATH}stat.ret.txt.35 ${LOGPATH}stat.ret.txt.35.hour.${THEHOUR}

cat ${LOGPATH}the_hour_access.log.10.3.17.36 | awk -F"?" '{print $1" "$2" "$3}' | awk '{ {time[$2]+=$1;count[$2]++;t=$1*1000;} \
if(t<=100) {sub_count[$2"_100"]++} \
if(t>100 && t<=300) {sub_count[$2"_300"]++} \
if(t>300 && t<=1000) {sub_count[$2"_1000"]++} \
if(t>1000) {sub_count[$2"_1001"]++} \
} END \
{for(i in count) \
{print time[i] "|" i "|" time[i]"|" count[i] "|" sub_count[i"_100"] "|" sub_count[i"_300"] "|" sub_count[i"_1000"] "|" sub_count[i"_1001"] "|"}}' | sort -nr | head -n40 > ${LOGPATH}stat.ret.txt.36
cp ${LOGPATH}stat.ret.txt.36 ${LOGPATH}stat.ret.txt.36.hour.${THEHOUR}
