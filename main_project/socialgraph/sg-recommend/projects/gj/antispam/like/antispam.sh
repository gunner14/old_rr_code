#!/bin/bash
if ls final.tmp1 &> /dev/null
	then
	rm final.tmp1 #如果有的话，删除这些文件
fi
if ls final.tmp3 &> /dev/null
	then
	rm final.tmp3 #如果有的话，删除这些文件
fi



`printf "BEGIN{\n" >> final.tmp1`
`printf "\tFS=\",,\"\n" >> final.tmp1`
`printf "}\n{\n\tif($""2 in ips)\n\t{\n\t">> final.tmp3`
`echo "printf(\"%d\\n\",$""3);" >> final.tmp3`
`printf "\n\t}\n}\n" >> final.tmp3`
`printf "END{\n}\n" >> final.tmp3`


echo "input action type: i.e. like"
read ACTION
echo "your actions is [$ACTION]"
echo "input date: i.e. 20120321"
read ADATE
echo "yuor date is [$ADATE]"
echo "input upbound: i.e. 10"
read AUP
echo "yuor up bound is [$AUP]"

`sed 's/ACTION_TO_MODIFY/"'$ACTION'"/g' first.awk.ips> first.awk.tmp`
`sed 's/ACTION_UP_BOUND/'$AUP'/g' first.awk.tmp > first.ip.awk`
`rm first.awk.tmp`
`sed 's/ACTION_TO_MODIFY/"'$ACTION'"/g' first.awk.goods> first.awk.tmp`
`sed 's/ACTION_UP_BOUND/'$AUP'/g' first.awk.tmp > first.goods.awk`
`rm first.awk.tmp`
#cat first.awk
echo "generating $ACTION action ips"
`awk -f first.ip.awk /data/gj_log/data/pv.log.$ADATE* > res.ip`
echo "generating $ACTION action goods"
`awk -f first.goods.awk /data/gj_log/data/pv.log.$ADATE* | sort -k 1 > res.ip_goods`
echo "generating final awk script"
`cat final.tmp1 res.ip final.tmp3> final.awk`
echo "use final.awk to deal with logs"
`awk -f final.awk /data/gj_log/data/pv.log.$ADATE* | sort | uniq > res.uid`
echo "done, ids are in file 'result'"

`rm final.*`
`rm first.ip.awk`
`rm first.goods.awk`
