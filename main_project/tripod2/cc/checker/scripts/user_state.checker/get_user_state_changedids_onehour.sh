#!bin/sh
if [ $# -lt 1 ];then
  echo "Usage: $0 OutputUniqChangedIdFileName"
  exit
fi

source ~/Stat/bin/init
 
Operations="(insert|update)UserState"
LogTime=`date +%Y-%m-%d-%H -d "hour ago"`
AllIdFile="../ids/ids.$LogTime"

echo "Generate uniq ids from ids"
echo $AllIdFile
awk '/'"$Operations"'/ {print $2}' $AllIdFile | sort -n | uniq > $1

