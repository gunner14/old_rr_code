#!bin/sh

#if [ $# -lt 3 ];then
#  echo "Usage $0 TotalChangedCount CacheWrongCount ReloadFailedCount"
#  exit
#fi


mkdir -p tmp
CheckResultHtmlFile="./tmp/check_result.html"
#EmailPropertyFile="./tmp/email_properties"
#EmailSenderClassPath="/data/xce/TripodChecker/EmailSender.jar"
#EmailSenderClass="xce.tools.EmailSender"
#Java="/data/xce/java"
EmailSenderPythonClient="/data/xce/TripodChecker/Email.py"

#TotalChangedCount=$1
#CacheWrongCount=$2
#ReloadFailedCount=$3

# send email 

#echo "<html> <head> <style>
#table { border-collapse:collapse; border:none; }
#th { border-top:solid #F79646 1.0pt; border-left:solid #F79646 1.0pt; background: #F79646; }
#td { border:solid #F79646 1.0pt; }
#</style> </head>
#<body>" > $CheckResultHtmlFile

echo "<html><body>" > $CheckResultHtmlFile

echo "
<table border=\"1\">
<tr><th>时间</th><th>服务名称</th><th>修改总数</th><th>CacheMiss</th><th>Cache错误</th><th>Reload失败</th></tr>" >> $CheckResultHtmlFile

Day=`date +%Y-%m-%d`
CurHourNum=`date +%H`
#echo $CurHourNum

for((i=0;i<=$CurHourNum;i++)){
  #echo $i
  Time=`date -d "-$i hours" +%Y-%m-%d-%H`
  ResultFile="./result/checker_result.$Time"
#----------------------------------------------------------------------
#Time=`date +%Y-%m-%d-%H`" ~ "`date -d "-1 hours" +%Y-%m-%d-%H`

  TimeTips=`date -d "-$((i+1)) hours" +%Y-%m-%d" "%H:00`" ~ "`date -d "-$i hours" +%H:00`
  #echo $TimeTips
  #echo "Lines " $Lines
 
  if [ -f $ResultFile ];then 
    Lines=`wc -l $ResultFile | cut -d " " -f 1`

    echo "<tr><td rowspan=$Lines >$TimeTips</td>" >> $CheckResultHtmlFile
    echo "Reading $ResultFile"
    FirstRow=1
    while read result
    do
      ServiceName=`echo $result | cut -d " " -f 1` 
      TotalChangedCount=`echo $result | cut -d " " -f 2` 
      CacheWrongCount=`echo $result | cut -d " " -f 3` 
      ReloadFailedCount=`echo $result | cut -d " " -f 4` 
      CacheMissCount=`echo $result | cut -d " " -f 5` 
    
      if [ -z "$ServiceName" ];then ServiceName="N/A";fi
      if [ -z "$TotalChangedCount" ];then TotalChangedCount="N/A";fi
      if [ -z "$CacheMissCount" ];then CacheMissCount="N/A";fi
      if [ -z "$CacheWrongCount" ];then CacheWrongCount="N/A";fi
      if [ -z "$ReloadFailedCount" ];then ReloadFailedCount="N/A";fi

      if [ $FirstRow -eq 1 ];then
        echo "<td>$ServiceName</td><td>$TotalChangedCount</td><td>$CacheMissCount</td><td>$CacheWrongCount</td><td>$ReloadFailedCount</td></tr>" >> $CheckResultHtmlFile
        FirstRow=0
      else 
        echo "<tr><td>$ServiceName</td><td>$TotalChangedCount</td><td>$CacheMissCount</td><td>$CacheWrongCount</td><td>$ReloadFailedCount</td></tr>" >> $CheckResultHtmlFile
      fi
    
      echo $TimeTips $ServiceName $TotalChangedCount $CacheMissCount $CacheWrongCount $ReloadFailedCount
    done < $ResultFile 
  else
    echo "<tr><td>$TimeTips</td>" >> $CheckResultHtmlFile
    echo "<td>N/A</td><td>N/A</td><td>N/A</td><td>N/A</td><td>N/A</td></tr>" >> $CheckResultHtmlFile
  fi

  
#----------------------------------------------------------------------
}
#Day=`date +%Y-%m-%d`
#for LogFile in `ls ../JavaChecker/log/user_config_checker_log.\`date +%Y-%m-%d\`* | sort` 
#do
#  # TODO 从Id file获取修改总数
#  Time=`echo $LogFile | awk 'BEGIN{FS="."}{print $NF}'`
#
#  
#  #TotalChangedCount=`wc -l ../tmp/uniq_changed_ids_one_hour.$Time | awk '{print $1}'`
#  TotalChangedCount=`grep " Check " $LogFile | wc -l`
#  CacheWrongCount=`grep " Check failed!" $LogFile | wc -l`
#  ReloadFailedCount=`grep " Reload failed!" $LogFile | wc -l`
#  echo "statistic from $LogFile"
#
#  if [ -z "$TotalChangedCount" ];then TotalChangedCount="N/A";fi
#  if [ -z "$CacheWrongCount" ];then CacheWrongCount="N/A";fi
#  if [ -z "$ReloadFailedCount" ];then ReloadFailedCount="N/A";fi
#
#  echo "<tr><td>$Time</td><td>UserConfig</td><td>$TotalChangedCount</td><td>$CacheWrongCount</td><td>$ReloadFailedCount</td></tr>" >> $CheckResultHtmlFile
#done

echo " </table> </body> </html> " >> $CheckResultHtmlFile

subject="Checker Result Statistic"
from="TripodChecker@renren-inc.com"
to="xin.huang@renren-inc.com liyong@renren-inc.com xiaofeng.liang@renren-inc.com li.li1@renren-inc.com ning.zhao@renren-inc.com"
#to="xin.huang@renren-inc.com"

#echo "email.from=monitor@renren-inc.com
#email.to=xin.huang@renren-inc.com,liyong@renren-inc.com,xiaofeng.liang@renren-inc.com,li.li1@renren-inc.com
#email.subject=UserConfig_Consistency_Check
#content.type=text/html
#content.body=$CheckResultHtmlFile
#" > $EmailPropertyFile

python $EmailSenderPythonClient "$subject" "$from" "$to" $CheckResultHtmlFile

#$Java -cp $EmailSenderClassPath $EmailSenderClass $EmailPropertyFile
