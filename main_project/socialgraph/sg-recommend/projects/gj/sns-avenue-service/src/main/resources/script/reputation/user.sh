#!/bin/sh
time_format=`date +%y%m%d%H`
new_file="/data/ranking/data/user.dat"
back_file="/data/ranking/data/backup/user.dat.$time_format"
if [ -f $new_file ] ;then 
  `cp $new_file $back_file`
fi 
`mongoexport --host db.jie.d.xiaonei.com --port 62166 -d sns_online -u gj -p dec2011online4nb  -c User -f "userId,userAbsInfo.fansCount,userAbsInfo.followCount,userAbsInfo.picList.0" --csv  | sed '1d;$d' | awk -F"," '{for( i=1 ;i < 4 ; i ++ ) printf("%d,",$i) ; if( NF != 4 ) {print 0 ; }else{if( match($4,/_main\.gif/) > 1 ) {print 0;}else{print 1;}}}'  > /data/ranking/data/user.dat`
