#!/bin/bash

log()
{
  Time=`date +%Y-%m-%d-%H:%M:%S`
  echo "[$Time] $1"
}

WaitTime=1800  # 等待所有checker的时间,秒

Hour=`date +%Y-%m-%d-%H`
WorkDir=`pwd`
CheckerDirs=`ls  | grep "\.checker$"`
ResultDir="./result"
mkdir -p $ResultDir
ResultFileName="checker_result.$Hour"
ResultFile="$ResultDir/$ResultFileName"

if [ -f $ResultFile ];then
  rm -f $ResultFile
fi

#一次取所有ids回来，防止多次远程登录卡住造成有些check没完成
echo "Getting all ids"
sh get_ids_onehour.sh

# 先同步调用获取各个Checker修改ID列表文件的脚本

for dir in `echo $CheckerDirs`
do
  log "Getting changed ids for $dir "
  cd $dir && sh $dir.sh getIdFile 
  cd $WorkDir
done

# 进入每个Checker目录并以后台方式启动相应脚本
for dir in `echo $CheckerDirs`
do
  log "Starting $dir checker..."
  log "cd $dir"
  cd $dir && sh $dir.sh startChecker &
  cd $WorkDir 
  #暂时不优化，打开DEBUG log查看ZKClient为什么不能正常初始化
  #sleep 2
done

# 等待Checker运行完成
log "Wating for $WaitTime seconds..."
sleep $WaitTime


# 获取各个Checker的统计结果
log "Generate report"
log "Hour: "$Hour
for dir in `echo $CheckerDirs`
do
  ServiceName=`echo $dir | cut -d "." -f1`
  CheckerResultDir="$dir/result"
  CheckerResultFileName=`ls $CheckerResultDir | grep "\.$Hour"`
  if [ "$CheckerResultFileName" == "" ];then
    # 如果 Checker没有生成统计文件，则其所有数据显示为N/A
    log "$dir has no result file"
    echo $ServiceName "N/A N/A N/A N/A" >> $ResultFile
    continue
  fi
  CheckerResultFile=$CheckerResultDir/$CheckerResultFileName
  log "Get result from $CheckerResultFile"
  echo $ServiceName
  echo $ServiceName `cat $CheckerResultFile` >> $ResultFile
done

# 发送Email
CurHour=`date +%H`
if [ "$CurHour" == "11" -o "$CurHour" == "23" ];then
  sh send_email.sh $ResultFile
fi

