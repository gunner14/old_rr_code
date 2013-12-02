#!bin/sh
source ~/Stat/bin/init
LogTime=`date +%Y-%m-%d-%H -d "hour ago"`
OldLogTime=`date +%Y-%m-%d-%H -d "month ago"`
UserBase="Name|Url|Passport|State|Stage|Born|Doing|LoginCount|Right"
UserNetwork="Region|Workplace|(Elementary|(Junior|)High)School|College|University|Network"
UserConfig="Config"
Operations="(update|insert)User($UserBase|$UserConfig|$UserNetwork)"
OcePath="~/XiaoNei/log/UserLogic*"
OceLog="oce_log.$LogTime"
IdsPath=ids
IdsFile="$IdsPath/ids.$LogTime"
OldIdsFile="IdsPath/ids.$OldLogTime"
Node="c5 c9"

if [ $# -ge 1 ]; then
  LogTime=$1
elif [ $# -ge 2 ]; then
  Operations=$2
fi

mkdir -p $IdsPath
rm -f $IdsFile

#获取UserLogic中的log
for node in $Node;
do
  echo "Getting log from $node"
  ssh xce@$node "grep -aE '$Operations' $OcePath/$OceLog | awk '"'{print $6,$NF}'"';exit" >> $IdsFile
done

#获取其他log，太多，暂时搁浅
#Operations="incUserLoginCount"
#node="u27"
#OcePath="~/User/log/UserBaseReader*"
#echo "Getting log from $node"
#ssh xce@$node "grep -aE '$Operations' $OcePath/$OceLog | awk '"'{print $6,$NF}'"';exit" >> $IdsFile

rm -f $OldIdsFile
