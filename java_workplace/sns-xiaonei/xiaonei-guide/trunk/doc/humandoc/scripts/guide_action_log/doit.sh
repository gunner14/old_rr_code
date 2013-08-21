#!/bin/bash
TIME=`date +%T`
#DATE=`date +%F`
DATE=`date -d yesterday +%Y-%m-%d`
#DATE="2010-12-30"
for i in `ls /data/inshion/ | grep $DATE`
do
echo $i
cd /data/inshion/guide_script;java -cp .:/data/inshion/guide_script/mysql-connector-java-5.1.7.jar:/data/inshion/guide_script/zguidelog.jar inshion.test.file.guidelog.core.GuideLog /data/inshion/$i localhost myuser 12345
done 
