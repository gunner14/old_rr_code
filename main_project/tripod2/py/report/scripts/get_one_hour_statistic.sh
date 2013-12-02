#!/bin/bash

if [ $# -lt 1 ];then
  echo "Usage: $0 Time"
  exit
fi

log()
{
  Second=`date +%Y-%m-%d-%H:%M:%S`
  echo "[$Second] $1" >> $LogFile
}

Time=$1
LogFile="./log/log.$Time"

mkdir -p result
mkdir -p log

if [ -f $LogFile ];then
  rm -f $LogFile
fi

RemoteLogResultFile="./result/remote_log_result_file.$Time"
log "Writting modify statistic data to db ----------"
if [ ! -f $RemoteLogResultFile ];then
  bash get_remote_log_statistic.sh $Time $RemoteLogResultFile >> $LogFile 2>&1
fi
bash write_data_to_db.sh $RemoteLogResultFile $Time >> $LogFile 2>&1

Time=`date "+%Y-%m-%d-%H"`
RedisCacheResultFile="./result/redis_cache_result_file.$Time"
log "Writting redis cache statistic data to db ----------"
if [ ! -f $RedisCacheResultFile ];then
  bash get_redis_server_info.sh $RedisCacheResultFile >> $LogFile 2>&1
fi
bash write_data_to_db.sh $RedisCacheResultFile $Time >> $LogFile 2>&1
