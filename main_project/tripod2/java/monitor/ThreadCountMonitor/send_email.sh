#!bin/sh
if [ $# -lt 1 ];then
  echo "Usage: $0 ResultFile"
  exit
fi

ResultFile=$1
  
mkdir -p tmp
HtmlFile="./tmp/message.html"
EmailSenderPythonClient="./Email.py"


BgStyle="style=\"background-color:#DDDDDD\""
echo "<html><body>" > $HtmlFile

echo "
<table $BgStyle border=\"1\">
<tr><th>机器名</th><th>服务名称</th><th>线程数</th><th>状态</th></tr>" >> $HtmlFile

while read result
do
  MachineName=`echo $result | cut -d " " -f 1`
  ServiceName=`echo $result | cut -d " " -f 2`
  ThreadCount=`echo $result | cut -d " " -f 3`
  State=`echo $result | cut -d " " -f 4`

  if [ "$State" == "warning" ];then
    Style="style=\"color:blue;\""
  else
    Style="style=\"color:red;\""
  fi

  echo "<tr><td $Style>$MachineName</td><td $Style>$ServiceName</td><td $Style>$ThreadCount</td><td $Style>$State</td></tr>" >> $HtmlFile
done < $ResultFile


echo " </body> </html> " >> $HtmlFile

subject="Tripod Thread Warning"
from="TripodMonitor@renren-inc.com"
to="xin.huang@renren-inc.com liyong@renren-inc.com xiaofeng.liang@renren-inc.com li.li1@renren-inc.com"
#to="xin.huang@renren-inc.com"

python $EmailSenderPythonClient "$subject" "$from" "$to" $HtmlFile

