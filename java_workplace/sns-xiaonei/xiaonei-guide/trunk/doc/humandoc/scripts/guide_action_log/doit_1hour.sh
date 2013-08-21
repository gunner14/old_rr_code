#!/bin/bash
TIME=`date +%T`
#DATE=`date +%F`
#DATE=`date -d yesterday +%Y-%m-%d`
DATE=`date +%Y-%m-%d -d "-1hour"`
H=`date +%k -d "-1 hour"`
#DATE="2010-12-30"
MYDATE=`date -d "-1 hours" "+%Y-%m-%d-%H"`
for i in `ls /data/inshion/ | grep $MYDATE`
do
echo $i
cd /data/inshion/guide_script;java -cp .:/data/inshion/guide_script/mysql-connector-java-5.1.7.jar:/data/inshion/guide_script/zguidelog.jar com.xiaonei.sns.guide.log.core.GuideLog /data/inshion/$i localhost myuser 12345
done 
