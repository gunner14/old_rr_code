#!/bin/bash
source /data/gj_log/.bashrc
cd /data/gj_log/antispam/xiong.hua/like 
#echo "input action type: i.e. like"
#read ACTION
ACTION="like"
echo "your actions is [$ACTION]"
#echo "input date: i.e. 20120321"
#read ADATE
ADATE=`date +%Y%m%d`
echo "yuor date is [$ADATE]"
#echo "input upbound: i.e. 10"
#read AUP
AUP=`date +%H/10*5"+"5 | bc`
echo "yuor up bound is [$AUP]"

if [ ! -d $ADATE ]; then
   mkdir $ADATE
fi
`printf "BEGIN{\n" >> $ADATE/final.tmp1`
`printf "\tFS=\",,\"\n" >> $ADATE/final.tmp1`
`printf "}\n{\n\tif($""2 in ips)\n\t{\n\t">> $ADATE/final.tmp3`
`echo "printf(\"%d\\n\",$""3);" >> $ADATE/final.tmp3`
`printf "\n\t}\n}\n" >> $ADATE/final.tmp3`
`printf "END{\n}\n" >> $ADATE/final.tmp3`

`sed 's/ACTION_TO_MODIFY/"'$ACTION'"/g' first.awk.ips> first.awk.tmp`
`sed 's/ACTION_UP_BOUND/'$AUP'/g' first.awk.tmp > $ADATE/first.ip.awk`
`rm first.awk.tmp`
`sed 's/ACTION_TO_MODIFY/"'$ACTION'"/g' first.awk.goods> first.awk.tmp`
`sed 's/ACTION_UP_BOUND/'$AUP'/g' first.awk.tmp > $ADATE/first.goods.awk`
`rm first.awk.tmp`
#cat first.awk
echo "generating $ACTION action ips"
`awk -f $ADATE/first.ip.awk /data/gj_log/data/pv.log.$ADATE* > $ADATE/res.ip`
echo "generating $ACTION action goods"
`awk -f $ADATE/first.goods.awk /data/gj_log/data/pv.log.$ADATE* | sort -k 1 > $ADATE/res.ip_goods`
`cat $ADATE/res.ip_goods | awk '{print $2}' | sort | uniq -u > $ADATE/spammers_goods_ids.txt`
echo "java -jar `pwd`/spammerUtils.jar -g -f `pwd`/$ADATE/spammers_goods_ids.txt" > $ADATE/check.sh
chmod +x $ADATE/check.sh
./$ADATE/check.sh

echo "generating final awk script"
`cat $ADATE/final.tmp1 $ADATE/res.ip $ADATE/final.tmp3> $ADATE/final.awk`
echo "use final.awk to deal with logs"
`awk -f $ADATE/final.awk /data/gj_log/data/pv.log.$ADATE* | sort | uniq > $ADATE/res.uid`
echo "done, ids are in file 'result'"

`rm $ADATE/final.*`
`rm $ADATE/first.ip.awk`
`rm $ADATE/first.goods.awk`
