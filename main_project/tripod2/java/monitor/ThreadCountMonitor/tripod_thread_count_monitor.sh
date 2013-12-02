#!/bin/bash

LogFileDir="./log"
mkdir -p $LogFileDir
Time=`date +%Y-%m-%d-%H`
LogFileName="log.$Time"
LogFile=$LogFileDir/$LogFileName

log()
{
  LogTime=`date +%Y-%m-%d-%H:%M:%S`
  echo "[$LogTime] $1" >> $LogFile
}

# 获取一台机器上的icebox进程的线程数 
getThCount(){
  m=$1
  getIceBoxProcessThcountCmd=`cat getIceBoxProcessThcountCmd.txt`
  TempFile="./temp.txt"

  ssh xce@$m.xce.d.xiaonei.com "$getIceBoxProcessThcountCmd" > $TempFile
  while read line
  do
    log "$m $line"
    echo "$m $line" >> $TmpResultFile
  done < $TempFile 
}

source ~/Stat/bin/init


TmpResultFile="./tmpFile"
WarningResultFile="./WarningFile"
EmailResultFile="./EmailResultFile"

if [ -f $TmpResultFile ];then
  rm -f $TmpResultFile
fi

if [ -f $EmailResultFile ];then
  rm -f $EmailResultFile
fi

log "Start-------------------------------------------------"

Target=$(for((i=0;i<15;i++));do echo cm$i;done)
for m in $Target
do
  getThCount $m
done


#获取线程数超过警戒值的记录
cat tmpFile | awk '{if($4>500){print $0}}' > $WarningResultFile
WarningCount=`cat $WarningResultFile | wc -l`

if [ $WarningCount -eq 0 ];then
  log "No thread count warning."
else
  log "Have thread count warning."
  while read line
  do
    log "Warning: $line"
    ThreadCount=`echo $line | awk '{print $4}'`

    # 将警告或杀死进程信息写入EmailResultFile中
    if [ $ThreadCount -gt 1000 ];then
      # 杀死线程数超过危险值的进程
      log "Killing: $line"
      
      MachineName=`echo $line | cut -d " " -f 1`
      ServiceName=`echo $line | cut -d " " -f 3`
      Cmd="ps aux | grep $ServiceName | grep -v grep | awk '{print "'$2'"}' | xargs kill -9"
      #echo $Cmd
      log "ssh xce@$MachineName.xce.d.xiaonei.com \"$Cmd\""
      ssh xce@$MachineName.xce.d.xiaonei.com "$Cmd"
      echo "$line killed" >> $EmailResultFile
    else
      echo "$line warning" >> $EmailResultFile
    fi
  done < $WarningResultFile
fi

if [ -f $EmailResultFile ];then
  sh send_email.sh $EmailResultFile 
  sh send_sms.sh $EmailResultFile
fi

log "Finish------------------------------------------------"
