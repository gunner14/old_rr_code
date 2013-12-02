#!/bin/bash

if [ $# -lt 2 ];then

  echo "Usage: $0 Time ResultFile"
  exit
fi

source ~/Stat/bin/init

Time=$1
ResultFile=$2

mkdir -p tmp
TempFileName="./tmp/remote_log_temp_file"
#ResultFile="./result/remote_log_result_file.$Time"


#Cmd="cat /data/xce/LogData/logger/xce/tripod/tripod_use.$Time | awk '{print "'$3'"}' | awk 'BEGIN{FS=\"|\"};{print "'$1'"\" \""'$4'"}' | sort | uniq -c"
#Cmd="cat /data/xce/LogData/logger/xce/tripod/tripod_use.2012-06-29-*"

Cmd="cat /data/xce/LogData/logger/xce/tripod.$Time | awk '{print "'$1'"\" \""'$4'"}' | sed \"s/^CacheManagerImpl::\\(.*\\)() business_id:\\(.*\\)$/\\1 \\2/\" | sort | uniq -c"

RemoteLogMachine="dp0 w1"

#if [ -f $TempFileName ];then
#  rm -f $TempFileName
#fi

#source ~/Stat/bin/init
for m in $RemoteLogMachine
do
  echo "Getting statistic data from $m----------" 
  ssh xce@$m.xce.d.xiaonei.com $Cmd > $TempFileName.$m.$Time &
done

wait

cat $TempFileName.*.$Time | awk '{count[$3" "$2]+=$1};END{for(i in count){print i" "count[i]}}' > $ResultFile 
