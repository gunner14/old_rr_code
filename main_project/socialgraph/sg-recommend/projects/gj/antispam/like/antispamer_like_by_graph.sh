#!/bin/sh
delta_mills=`date +3600000*%H+60000*%M | bc`
time_in_mills=`date +%s000`
pre_date_mills=`echo "$time_in_mills - $delta_mills" | bc`
time_in_mills=`date +%s000 -d "-1 days ago"`
next_date_mills=`echo "$time_in_mills - $delta_mills" | bc`
time_in_date=`date +%Y%m%d `
if [ -d $time_in_date ]; then
   rm $time_in_date -rf
fi
mkdir $time_in_date
mcmd="mongoexport  --csv --host db1.jie.d.xiaonei.com --port 62166 -d sns_online -u gj -p dec2011online4nb -c LikeRecord -q '{\"likeTime\":{\$gt:$pre_date_mills},\"likeTime\":{\$lt:$next_date_mills}}' -f \"userId,goodsId,ownerId,likeTime\" | sed '1d;\$d' > $time_in_date/likes.csv"
sh_file="dump_db.$time_in_date.sh"
`echo $mcmd > ./$time_in_date/$sh_file`
chmod +x $time_in_date/$sh_file
./$time_in_date/$sh_file
`cat $time_in_date/likes.csv | awk -f u2u.group.awk | sort -k3 -nr > $time_in_date/u2u_group.txt`
`cat $time_in_date/u2u_group.txt | awk -f spammer_tree.awk > $time_in_date/u2u_spammer.txt`
`cat $time_in_date/u2u_spammer.txt | sort -nr --key=2,3 | awk '{if($2 >=5 ) print $1}' > $time_in_date/spammers.txt`
jcmd="/data/java/bin/java -jar /data/ranking/stat/guangjie-data-repair_fat.jar -u -f `pwd`/$time_in_date/spammers.txt -n 10"
chmod 777 $time_in_date/spammers.txt
echo $jcmd > $time_in_date/check.sh
chmod +x $time_in_date/check.sh
./$time_in_date/check.sh
#`$jcmd`
#`cat $time_in_date/spammers.txt | awk '{print "java -jar user_query_utils.jar -f userAbsInfo.fansCount -u "$1}' > check.sh`
