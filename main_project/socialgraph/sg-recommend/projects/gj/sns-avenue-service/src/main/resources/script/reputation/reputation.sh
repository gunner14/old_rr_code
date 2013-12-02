#!/bin/sh
## ge ugc meta
`source /data/ranking/.bashrc`
time_format=`date +%y%m%d%H`
new_file="/data/ranking/data/UgcMeta.dat"
back_file="/data/ranking/data/backup/UgcMeta.dat.$time_format"
if [ -f $new_file ] ;then
  `cp $new_file $back_file`
fi
min_comment_id=`date +%y%m%d%H%M%S0000000 -d "100 days ago"`
`mongoexport --host db.jie.d.xiaonei.com --port 62166 -d sns_online -u gj -p dec2011online4nb -c Comment -q "{state:-1,commentId:{\\$gt:$min_comment_id}}" -f "ownerId" --csv  | sed '1d;$d' | sort -n   | uniq -dc  | awk '{printf("%d,%d\n",$2,$1)}'> $new_file`

## get user
time_format=`date +%y%m%d%H`
new_file="/data/ranking/data/GjUser.dat"
back_file="/data/ranking/data/backup/GjUser.dat.$time_format"
if [ -f $new_file ] ;then
  `cp $new_file $back_file`
fi
`mongoexport --host db.jie.d.xiaonei.com --port 62166 -d sns_online -u gj -p dec2011online4nb  -c User -f "userId,userAbsInfo.fansCount,userAbsInfo.followCount,userAbsInfo.picList.0" --csv  | sed '1d;$d' | awk -F"," '{for( i=1 ;i < 4 ; i ++ ) printf("%d,",$i) ; if( NF != 4 ) {print 0 ; }else{if( match($4,/_main\.gif/) > 1 ) {print 0;}else{print 1;}}}' | awk -F"," '{printf("%s,%s,%s,%s\n",$1,$2,$4,$3)}' > $new_file`

## get goods meta
time_format=`date +%y%m%d%H`
new_file="/data/ranking/data/GoodsMeta.dat"
back_file="/data/ranking/data/backup/GoodsMeta.dat.$time_format"
if [ -f $new_file ] ;then
  `cp $new_file $back_file`
fi
`time java -classpath /data/ranking/reputation/GoodsMetadataLoader.jar com.renren.ad.sns.services.reputation.data.GoodsMetadataLoader $new_file`
