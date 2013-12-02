#!bin/sh

init()
{
  Time=`date +%Y-%m-%d-%H`
  LogFilePath="./log"
  LogFileName="user_state_checker_log.$Time"
  LogFile=$LogFilePath/$LogFileName 
  GetChangedIdsScript="get_user_state_changedids_onehour.sh"
  UniqChangedIdsFile="./input/uniq_changed_ids_one_hour.$Time"
  ResultFile="./result/user_state_result.$Time"

  Namespace="xce_user"
  BusinessId="user_state"
  Checker="/data/xce/TripodChecker/BatchChecker/batch_checker"

  MissFlag="Data_Miss"
  WrongFlag="Data_Wrong"
  LoadFailFlag="Load_Final_Fail"

  mkdir -p input 
  mkdir -p $LogFilePath
  mkdir -p result
  
}

getIdFile()
{
  echo "Getting id file ==================================" >> $LogFile
  # 获取数据改变的ID列表文件,存放在../tmp目录下
  sh $GetChangedIdsScript $UniqChangedIdsFile >> $LogFile 2>&1
}

startChecker()
{  
  echo "Start checking ==================================" >> $LogFile
  # 将改变的ID列表文件作为Java Checker的输入
  Commond="$Checker -n $Namespace -s $BusinessId -t 10 -l DEBUG -f $UniqChangedIdsFile >> $LogFile 2>&1"
  echo $Commond
  eval $Commond
  #$Checker -n $Namespace -s $BusinessId -f $UniqChangedIdsFile >> $LogFile 2>&1
  
  echo "Check done ======================================" >> $LogFile

  TotalChangedCount=`wc -l $UniqChangedIdsFile | cut -d " " -f 1`
  CacheMissCount=`grep "$MissFlag" $LogFile | wc -l`
  CacheWrongCount=`grep "$WrongFlag" $LogFile | wc -l`
  ReloadFailedCount=`grep "$LoadFailFlag" $LogFile | wc -l`
  
  echo "Result--------------------------------------------" >> $LogFile
  echo "Total Changed Count: $TotalChangedCount" >> $LogFile
  echo "Miss data Count: $CacheMissCount" >> $LogFile
  echo "Wrong data Count: $CacheWrongCount" >> $LogFile
  echo "Reload failed Count: $ReloadFailedCount" >> $LogFile
  
  
  echo "Check compelte ===================================" >> $LogFile
  echo "$TotalChangedCount $CacheWrongCount $ReloadFailedCount $CacheMissCount" > $ResultFile
}

getAndCheck(){
  getIdFile
  startChecker
}

#============================== Main Body ================================

if [ $# -lt 1 ];then
  echo "Usage: $0 Command"
  exit
fi

Command=$1

init

$Command
