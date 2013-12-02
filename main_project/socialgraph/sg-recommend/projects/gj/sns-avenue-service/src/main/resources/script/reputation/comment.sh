#!/bin/sh
time_format=`date +%y%m%d%H`
new_file="/data/ranking/data/comment.dat"
back_file="/data/ranking/data/backup/comment.dat.$time_format"
if [ -f $new_file ] ;then 
  `cp $new_file $back_file`
fi 
min_comment_id=`date +%y%m%d%H%M%S0000000 -d "100 days ago"`
`mongoexport --host db.jie.d.xiaonei.com --port 62166 -d sns_online -u gj -p dec2011online4nb -c Comment -q "{state:-1,commentId:{\\$gt:$min_comment_id}}" -f "ownerId" --csv  | sed '1d;$d' | sort -n   | uniq -dc  | awk '{printf("%d,%d\n",$2,$1)}'> $new_file`

