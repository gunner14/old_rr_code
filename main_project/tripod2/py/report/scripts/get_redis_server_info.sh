#! /bin/bash

if [ $# -lt 1 ];then
  echo "Usage $0 ResultFile"
  exit
fi

source ~/Stat/bin/init
source ./environment

RESULT_FILE=$1
ADDRESS_FILE="./tmp/redis_address.txt"
TEMP_FILE="./tmp/redis_temp_file.txt"
REDIS_CLI="./redis-cli"
ZK_CLI="./zkCli.sh"
ZK_CACHE_PATH="/tripod2/cache"

mkdir -p tmp

if [ -f $TEMP_FILE ];then
  rm -f $TEMP_FILE
fi

CACHE_NAMESPACE_LIST=`$ZK_CLI -server $ZK_ADDRESS ls $ZK_CACHE_PATH  | tail -1 | sed "s/\[\(.*\)\]/\1/" | sed "s/,/ /g"`
echo $CACHE_NAMESPACE_LIST

for NS in $CACHE_NAMESPACE_LIST
do
  echo "----- $NS -----"
  CACHE_INS_LIST=`$ZK_CLI -server $ZK_ADDRESS ls $ZK_CACHE_PATH/$NS | tail -1 | sed "s/\[\(.*\)\]/\1/" | sed "s/,/ /g"`
  for INS in $CACHE_INS_LIST  
  do
    echo $INS >> $TEMP_FILE
  done 
done

cat $TEMP_FILE | awk 'BEGIN{FS=":"}{print $2":"$3}' | sort |  uniq > $ADDRESS_FILE

if [ -f $RESULT_FILE ];then
  rm -f $RESULT_FILE
fi

while read LINE
do 
  HOST=`echo $LINE | cut -d ":" -f 1`
  PORT=`echo $LINE | cut -d ":" -f 2`
  echo "Getting statistic data from redis server $HOST:$PORT ----------"
  INFO=`$REDIS_CLI -h $HOST -p $PORT info`
  USED_MEMORY_RSS=`echo "$INFO" | grep "used_memory_rss:" | awk 'BEGIN{FS=":"}{print $2}' | sed "s///g"` 
  USED_MEMORY_RSS=`echo "scale=2;$USED_MEMORY_RSS/(1024*1024)" | bc`
  TOTAL_CMD=`echo "$INFO" | grep "total_commands_processed:" | awk 'BEGIN{FS=":"}{print $2}' | sed "s///g"`
  KEY_HIT=`echo "$INFO" | grep "keyspace_hits:" | awk 'BEGIN{FS=":"}{print $2}' | sed "s///g"`
  KEY_MISS=`echo "$INFO" | grep "keyspace_misses:" | awk 'BEGIN{FS=":"}{print $2}' | sed "s///g"`

  #STATISTIC=`$REDIS_CLI -h $HOST -p $PORT info | grep "used_memory_rss:" | awk 'BEGIN{FS=":"}{ printf("%s %.2f\n",$1,$2/(1024*1024))}'`
  #echo "$LINE $STATISTIC" >> $RESULT_FILE 
  echo "$LINE used_memory_rss $USED_MEMORY_RSS" >> $RESULT_FILE 
  echo "$LINE total $TOTAL_CMD" >> $RESULT_FILE 
  echo "$LINE hit $KEY_HIT" >> $RESULT_FILE 
  echo "$LINE miss $KEY_MISS" >> $RESULT_FILE 
done < $ADDRESS_FILE
