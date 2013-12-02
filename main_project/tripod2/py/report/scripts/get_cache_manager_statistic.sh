#!/bin/bash

if [ $# -lt 2 ];then
  echo "Usage $0 Time ResultFile"
  exit
fi

source ~/Stat/bin/init

Time=$1
ResultFile=$2

mkdir -p tmp
TempFile="./tmp/cache_manager_temp_file.$Time"
#ResultFile="./result/cache_manager_result_file.$Time"



Cmd="
  for file in \`find CommonCache/log/Tripod2CacheManager* -name \"oce_log.$Time\"\`; 
  do 
    grep Invoke "'$file'" | grep -v ping | awk '{print "'$6'"\" \"substr("'$16'",index("'$16'",\":\")+1)}' | sort | uniq -c; 
  done
"

if [ -f $TempFile ]; then
  rm -f $TempFile
fi

#source ~/Stat/bin/init

for((i=0;i<15;i++))
{
  echo "Getting statistic data from cm$i----------" 
  ssh xce@cm$i.xce.d.xiaonei.com $Cmd >> $TempFile
}


cat $TempFile | awk '{count[$2" "$3]+=$1};END{for(i in count){print count[i]"\t"i}}' | grep "get" > $ResultFile 


